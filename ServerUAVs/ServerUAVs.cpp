// ServerUAVs - главный программный модуль серверной части ПО макета
#include <iostream>
#include <signal.h>
#include <thread>
#include <atomic>
#include "UdpLink.h"
#include "DataBaseUAVs.h"
#include "FiniteStateUAVs.h"

class ServerUAVs {
public:
    ServerUAVs(const std::string& serverIP, int listenPort) : processorUAVs(UAVs), isRunning(false), centerLat(55.123), centerlon(37.456)
    {
            if (!telemetryRX.bind(listenPort)) {
                throw std::runtime_error("Провал в подключении к порту");
            }
    }

    ~ServerUAVs()
    {
        stop();
    }

    void start()
    {
        std::cout << "[Сервер] Запуск..." << std::endl;
        isRunning = true;

        receiveThread = std::thread(&ServerUAVs::telemetryReceiverLoop, this);

        std::cout << "[Сервер] Ожидание..." << std::endl;
    }

    void stop()
    {
        std::cout << "[Сервер] Завершение..." << std::endl;
        isRunning = false;

        if (receiveThread.joinable()) receiveThread.join();
        if (commandThread.joinable()) commandThread.join();

        std::cout << "[Сервер] Остановлен" << std::endl;
    }

    void setFormationCenter(double lat, double lon);
    {
        centerLat = lat;
        centerLon = lon;
    }
private:
    void telemetryReceiverLoop()      // поток непрервного приёма телеметрии
    {
        telemetryRX.startReceiving([this](const std::vector<uint8_t>& data, const std::string& ip, uint16_t port)
            {
                if (data.size() >= sizeof(Protocol::TelemetryPacket)) {
                    auto packet = Protocol::TelemetryPacket::deserialize(data);
                    UAVs.updateUAV(packet, ip, port);

                    std::cout << "[Телеметрия] БВС " << packet.uav_id
                        << " | pos: (" << packet.latitude << ", " << packet.longitude
                        << ", " << packet.altitude << ") | batt: "
                        << (int)packet.battery_percent << "%" << std::endl;
                }
            });
    }
    void commandProcessorLoop()       // поток непрерывного анализа и отправки команд
    {
        while (isRunning) {
            auto commands = processor.process(centerLat, centerLon);

            for (const auto& cmd : commands) {
                auto uav = registry.getUAV(cmd.target_drone_id);
                if (uav.is_connected) {
                    auto data = cmd.serialize();
                    commandTX.send(data, drone.ip_address, uav.port);

                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    UdpLink telemetryRX;            // приём данных
    UdpLink commandTX;              // отправка команд
    DataUAVs UAVs;                  // конфигурационная БД об клиентах БВС
    UAVsStateMachine processorUAVs; // автомат конечных состояний сервера

    std::atomic<bool> isRunning;
    std::thread receiveThread;
    std::thread commandThread;

    double centerLat;
    double centerLon;
};

volatile sig_atomic_t globalRunning{ 1 };

void signalHandler(int) {
    globalRunning = 0;
}

int main() {
    signal(SIGINT, signalHandler);

    try {
        ServerUAVs server(127.0.01, 8888);
        server.start();

        while (globalRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        server.stop();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}