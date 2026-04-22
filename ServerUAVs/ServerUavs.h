// server_core.h - главный класс сервера
#pragma once
#include "UdpLink.h"
//#include "drone_registry.h"
#include "FiniteStateUAVs.h"
#include <thread>
#include <atomic>

class ServerCore {
public:
    ServerCore(int listen_port, int send_port);
    ~ServerCore();

    void start();
    void stop();
    void setFormationCenter(double lat, double lon);
private:
    void telemetryReceiverLoop();      // поток приёма телеметрии
    void commandProcessorLoop();       // поток анализа и отправки команд
    
    //UDPManager m_telemetry_receiver;   // Приём телеметрии на порту 8888
    //UDPManager m_command_sender;       // Отправка команд с порта 8889
    //DroneRegistry m_registry;
    UAVsStateMachine processorUAVs;

    std::atomic<bool> m_running;
    std::thread m_receive_thread;
    std::thread m_command_thread;

    double m_center_lat;
    double m_center_lon;
};