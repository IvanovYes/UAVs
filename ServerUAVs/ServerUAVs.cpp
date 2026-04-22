#include "ServerUAVs.h"
#include <iostream>

ServerCore::ServerCore(int telemetry_port, int command_port)
    : processorUAVs(m_registry)
    , m_running(false)
    , m_center_lat(55.123)
    , m_center_lon(37.456) {

    if (!m_telemetry_receiver.bind(telemetry_port)) {
        throw std::runtime_error("Failed to bind telemetry port");
    }

    // Командный сокет не требует привязки для отправки
}

ServerCore::~ServerCore() {
    stop();
}

void ServerCore::start() {
    std::cout << "[Server] Starting..." << std::endl;
    m_running = true;

    m_receive_thread = std::thread(&ServerCore::telemetryReceiverLoop, this);

    std::cout << "[Server] Running on telemetry port 8888, command port 8889" << std::endl;
}

void ServerCore::stop() {
    std::cout << "[Server] Stopping..." << std::endl;
    m_running = false;

    if (m_receive_thread.joinable()) m_receive_thread.join();
    if (m_command_thread.joinable()) m_command_thread.join();

    std::cout << "[Server] Stopped" << std::endl;
}

void ServerCore::telemetryReceiverLoop() {
    m_telemetry_receiver.startReceiving([this](const std::vector<uint8_t>& data,
        const std::string& ip, uint16_t port) {
            if (data.size() >= sizeof(Protocol::TelemetryPacket)) {
                auto packet = Protocol::TelemetryPacket::deserialize(data);
                m_registry.updateDrone(packet, ip, port);

                std::cout << "[Telemetry] Drone " << packet.drone_id
                    << " | pos: (" << packet.latitude << ", " << packet.longitude
                    << ", " << packet.altitude << ") | batt: "
                    << (int)packet.battery_percent << "%" << std::endl;
            }
        });
}

void ServerCore::commandProcessor() {
    while (m_running) {
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void ServerCore::setFormationCenter(double lat, double lon) {
    m_center_lat = lat;
    m_center_lon = lon;
}