#include "UdpLink.h"
#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include <cstring>

void sock_send(uint8_t msg_buf[BUFFER_LENGTH], int msg_length, sockaddr_in adress, int msg_socket)
{
	memset(msg_buf, 0, BUFFER_LENGTH);
	int msg_bytes_sent = sendto(msg_socket, msg_buf, msg_length, 0, (struct sockaddr*)&adress, sizeof(struct sockaddr_in));
	//printf("\nCMD sent: %d Len: %d\n", msg_bytes_sent, msg_length);
}

int sock_recieve(uint8_t msg_buf[BUFFER_LENGTH], sockaddr_in adress, int msg_socket, socklen_t fromlen)
{
	memset(msg_buf, 0, BUFFER_LENGTH);
	int recsize = recvfrom(msg_socket, (void*)msg_buf, BUFFER_LENGTH, 0, (struct sockaddr*)&adress, &fromlen);
	return recsize;
}

int sock_init(int& sock, struct sockaddr_in gcAddr, struct sockaddr_in locAddr, char target_ip[100])
{
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	memset(&locAddr, 0, sizeof(locAddr));
	locAddr.sin_family = AF_INET;
	locAddr.sin_addr.s_addr = INADDR_ANY;
	locAddr.sin_port = htons(14551);

	/* Bind the socket to port 14551 - necessary to receive packets MAVProxy */
	if (-1 == bind(sock, (struct sockaddr*)&locAddr, sizeof(struct sockaddr)))
	{
		perror("LOC: error bind failed");
		close(sock);
		exit(EXIT_FAILURE);
	}

	/* Attempt to make it non blocking */
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
	{
		fprintf(stderr, "error setting nonblocking: %s\n", strerror(errno));
		close(sock);
		exit(EXIT_FAILURE);
	}

	memset(&gcAddr, 0, sizeof(gcAddr));
	gcAddr.sin_family = AF_INET;
	gcAddr.sin_addr.s_addr = inet_addr(target_ip);
	gcAddr.sin_port = htons(14550);
}

ServerUDP::ServerUDP() : socket(-1), running(false) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
    socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    // Разрешаем переиспользование адреса
    int opt = 1;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

ServerUDP::~ServerUDP() {
    stopReceiving();
    if (socket >= 0) {
        close(socket);
    }
}

bool ServerUDP::bind(int port) {
    memset(&m_local_addr, 0, sizeof(m_local_addr));
    m_local_addr.sin_family = AF_INET;
    m_local_addr.sin_addr.s_addr = INADDR_ANY;
    m_local_addr.sin_port = htons(port);

    if (::bind(m_socket_fd, (struct sockaddr*)&m_local_addr, sizeof(m_local_addr)) < 0) {
        std::cerr << "Bind failed on port " << port << std::endl;
        return false;
    }
    return true;
}

void ServerUDP::startReceiving(PacketCallback _callback) {
    callback = _callback;
    running = true;
    receiveThread = std::thread(&ServerUDP::receiveLoop, this);
}

void ServerUDP::stopReceiving() {
    running = false;
    if (receiveThread.joinable()) {
        receiveThread.join();
    }
}

void ServerUDP::receiveLoop() {
    std::vector<uint8_t> buffer(65536);
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (running) {
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(socket, &read_set);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;  // 100ms таймаут для проверки m_running

        int ret = select(socket + 1, &read_set, nullptr, nullptr, &timeout);
        if (ret < 0) break;
        if (ret == 0) continue;  // Таймаут

        int n = recvfrom(socket, buffer.data(), buffer.size(), 0,
            (struct sockaddr*)&client_addr, &addr_len);
        if (n > 0) {
            std::string ip = inet_ntoa(client_addr.sin_addr);
            uint16_t port = ntohs(client_addr.sin_port);

            buffer.resize(n);
            if (callback) {
                callback(buffer, ip, port);
            }
        }
    }
}

bool UDPManager::send(const std::vector<uint8_t>& data, const std::string& ip, uint16_t port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    ssize_t sent = sendto(m_socket_fd, data.data(), data.size(), 0,
        (struct sockaddr*)&addr, sizeof(addr));
    return sent == (ssize_t)data.size();
}