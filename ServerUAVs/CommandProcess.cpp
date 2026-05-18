#include "command_processor.h"
#include <cmath>
#include <iostream>

uint32_t CommandProcessor::s_next_command_id = 1;

CommandProcessor::CommandProcessor(DroneRegistry& registry)
    : m_registry(registry)
    , m_center_lat(55.123)
    , m_center_lon(37.456)
    , m_max_deviation_meters(100.0)
    , m_no_fly_zone_active(false) {}

std::vector<Protocol::CommandPacket> CommandProcessor::process(double center_lat, double center_lon) {
    m_center_lat = center_lat;
    m_center_lon = center_lon;

    std::vector<Protocol::CommandPacket> commands;
    auto drones = m_registry.getAllActiveDrones();

    for (uint32_t id : drones) {
        auto drone = m_registry.getDrone(id);
        if (!drone.is_connected) continue;

        // Правило 1: Низкий заряд батареи -> посадка
        if (drone.telemetry.battery_percent < 15) {
            std::cout << "[CommandProcessor] Drone " << id
                << " low battery (" << (int)drone.telemetry.battery_percent
                << "%) -> LAND" << std::endl;
            commands.push_back(createLandCommand(id));
            continue;
        }

        // Правило 2: Отклонение от центра -> возврат
        double distance = calculateDistance(drone.telemetry.latitude, drone.telemetry.longitude,
            m_center_lat, m_center_lon);
        if (distance > m_max_deviation_meters) {
            std::cout << "[CommandProcessor] Drone " << id
                << " deviated " << distance << "m (max " << m_max_deviation_meters
                << "m) -> RETURN" << std::endl;
            commands.push_back(createGotoCommand(id, m_center_lat, m_center_lon, 50.0));
            continue;
        }

        // Правило 3: Вход в запретную зону
        if (m_no_fly_zone_active && isInNoFlyZone(drone.telemetry.latitude, drone.telemetry.longitude)) {
            std::cout << "[CommandProcessor] Drone " << id << " entered no-fly zone -> RETURN" << std::endl;
            commands.push_back(createGotoCommand(id, m_center_lat, m_center_lon, 50.0));
        }
    }

    return commands;
}

double CommandProcessor::calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    // Приблизительное расстояние в метрах (для макета)
    const double R = 6371000;  // Радиус Земли в метрах
    double dlat = (lat2 - lat1) * M_PI / 180.0;
    double dlon = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dlat / 2) * sin(dlat / 2) +
        cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
        sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

Protocol::CommandPacket CommandProcessor::createGotoCommand(uint32_t drone_id,
    double lat, double lon, double alt) {
    Protocol::CommandPacket cmd;
    cmd.target_drone_id = drone_id;
    cmd.command_id = s_next_command_id++;
    cmd.type = Protocol::CommandType::GOTO;
    cmd.priority = 100;
    cmd.param1 = lat;
    cmd.param2 = lon;
    cmd.param3 = alt;
    return cmd;
}

Protocol::CommandPacket CommandProcessor::createLandCommand(uint32_t drone_id) {
    Protocol::CommandPacket cmd;
    cmd.target_drone_id = drone_id;
    cmd.command_id = s_next_command_id++;
    cmd.type = Protocol::CommandType::LAND;
    cmd.priority = 200;  // Высокий приоритет
    cmd.param1 = 0;
    cmd.param2 = 0;
    cmd.param3 = 0;
    return cmd;
}

void CommandProcessor::setNoFlyZone(double lat1, double lon1, double lat2, double lon2) {
    m_no_fly_zone_active = true;
    m_nfz_lat1 = lat1;
    m_nfz_lon1 = lon1;
    m_nfz_lat2 = lat2;
    m_nfz_lon2 = lon2;
}

bool CommandProcessor::isInNoFlyZone(double lat, double lon) {
    return (lat >= m_nfz_lat1 && lat <= m_nfz_lat2 &&
        lon >= m_nfz_lon1 && lon <= m_nfz_lon2);
}