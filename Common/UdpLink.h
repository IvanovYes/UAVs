#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
//#include <sys/types.h>
#include <windows.h>
#include <winsock2.h>

#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <sys/stat.h>

#define BUFFER_LENGTH 2041

//void sock_send(mavlink_message_t u_msg_send, sockaddr_in adress, int msg_socket);
int sock_init(int& sock, struct sockaddr_in gcAddr, struct sockaddr_in locAddr, char target_ip[100]);

class ServerUDP {
public:
    using PacketCallback = std::function<void(const std::vector<uint8_t>&,
        const std::string&,
        uint16_t)>;

    ServerUDP();
    ~ServerUDP();

    bool bind(int port);                    // Привязка к порту для приёма
    bool connectTo(const std::string& ip, uint16_t port);  // Для отправки

    void startReceiving(PacketCallback callback);
    void stopReceiving();

    bool send(const std::vector<uint8_t>& data, const std::string& ip, uint16_t port);
    bool sendToAll(const std::vector<uint8_t>& data);

    void addClient(const std::string& ip, uint16_t port, uint32_t drone_id);
    void removeClient(uint32_t drone_id);

private:
    void receiveLoop();

    int socket;
    struct sockaddr_in locAddr;
    std::atomic<bool> running;
    std::thread receiveThread;
    PacketCallback callback;

    struct ClientInfo {
        std::string ip;
        uint16_t port;
        std::chrono::system_clock::time_point last_seen;
    };
    std::unordered_map<uint32_t, ClientInfo> m_clients;
    std::mutex m_clients_mutex;
};