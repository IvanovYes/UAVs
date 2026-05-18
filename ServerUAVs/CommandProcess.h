#pragma once
#include "../common/protocol.h"
#include "drone_registry.h"
#include <vector>
#include <random>

class CommandProcessor {
public:
    CommandProcessor(DroneRegistry& registry);

    // Генерация команд на основе текущего состояния дронов
    std::vector<Protocol::CommandPacket> process(double center_lat, double center_lon);

    // Настройки поведения
    void setNoFlyZone(double lat1, double lon1, double lat2, double lon2);
    void setMaxDeviation(double meters);

private:
    DroneRegistry& m_registry;

    double m_center_lat;
    double m_center_lon;
    double m_max_deviation_meters;  // Максимальное отклонение от центра
    bool m_no_fly_zone_active;
    double m_nfz_lat1, m_nfz_lon1, m_nfz_lat2, m_nfz_lon2;

    // Вспомогательные функции
    double calculateDistance(double lat1, double lon1, double lat2, double lon2);
    bool isInNoFlyZone(double lat, double lon);
    Protocol::CommandPacket createGotoCommand(uint32_t drone_id, double lat, double lon, double alt);
    Protocol::CommandPacket createLandCommand(uint32_t drone_id);

    static uint32_t s_next_command_id;
};