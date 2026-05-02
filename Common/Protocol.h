#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace Protocol {

// Команды (также события для серверной машины состояний)
enum class CommandType : uint8_t {
    NONE = 0,
    TAKEOFF = 1,
    LAND = 2,
    START_MISSION = 3,
    PAUSE_MISSION = 4,
    RETURN_TO_BASE = 5,
    CHANGE_MISSION = 6,
    CHECK_СONNECT_AND_CONDITION = 7
};

#pragma pack(push, 1)
struct CommandData {
    uint32_t command_id;
    uint32_t target_drone_id;
    CommandType type;
    double param1;
    double param2;
    double param3;
    std::string mission_data;  // Для UPLOAD_MISSION — сериализованная миссия

    std::vector<uint8_t> serialize() const;
    static CommandData deserialize(const std::vector<uint8_t>& data);
};
#pragma pack(pop)

// Один пункт полётного задания
enum class MissionItemType : uint8_t {
    WAYPOINT = 0,           // Пролететь через точку
    LOITER = 1,             // Зависание (время или количество кругов)
    ROI = 2,                // Region of Interest (смотреть на точку)
    SET_SPEED = 3,          // Установка скорости
    SET_CAMERA = 4,         // Управление камерой
    DO_JUMP = 6,            // Переход к другому пункту
    RETURN_TO_LAUNCH = 7,   // Возврат на базу
    IMAGE_CAPTURE = 8,      // Съёмка
    LANDING_PATTERN = 9,    // Посадочный манёвр
    FORMATION_CHANGE = 10   // Смена строя (группы БВС)
};

#pragma pack(push, 1)
struct MissionItem {
    uint16_t seq;                    // Порядковый номер (0-based)
    MissionItemType type;            // Тип действия
    double latitude;                 // Широта
    double longitude;                // Долгота
    float altitude;                  // Высота (метры)
    float param1;                    // Параметр 1 (зависит от типа)
    float param2;                    // Параметр 2
    float param3;                    // Параметр 3
    float param4;                    // Параметр 4
    float loiter_time;               // Время зависания (сек)
    uint16_t loiter_turns;           // Количество кругов (0 = бесконечно)
    uint8_t acceptance_radius;       // Радиус принятия (метры)
    uint8_t yaw;                     // Курс (градусы)
    bool autocontinue;               // Автоматический переход к следующему

    std::vector<uint8_t> serialize() const;
    static MissionItem deserialize(const std::vector<uint8_t>& data);
};
#pragma pack(pop)

// Полное полётное задание
#pragma pack(push, 1)
struct MissionPlan {
    uint32_t mission_id;             // Уникальный ID миссии
    uint32_t drone_id;               // Целевой дрон (0 = всем)
    std::string name;                // Название миссии
    std::vector<MissionItem> items;  // Список пунктов
    uint8_t version;                 // Версия формата
    uint32_t created_at;             // Время создания (unix timestamp)
    bool is_repeating;               // Зациклить миссию

    bool validate() const;           // Валидация миссии

    std::vector<uint8_t> serialize() const;
    static MissionPlan deserialize(const std::vector<uint8_t>& data);
};
#pragma pack(push, 1)

// Структура телеметрии (для передачи от клиента к серверу)
#pragma pack(push, 1)
struct TelemetryData {
    uint32_t drone_id;
    double latitude;
    double longitude;
    double altitude;
    double speed;
    int16_t battery_percent;
    uint8_t status;  // 0=на земле, 1=взлетает, 2=в полёте, 3=посадка
    uint64_t timestamp_ms;

    std::vector<uint8_t> serialize() const;
    static TelemetryData deserialize(const std::vector<uint8_t>& data);
};
#pragma pack(push, 1)
} // namespace Protocol