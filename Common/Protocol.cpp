#include "protocol.h"
#include <cstring>

namespace Protocol {

    std::vector<uint8_t> TelemetryData::serialize() const {
        std::vector<uint8_t> data(4 + 8 + 8 + 8 + 8 + 2 + 1 + 8); // 47 байт
        size_t offset = 0;
        memcpy(&data[offset], &drone_id, 4); offset += 4;
        memcpy(&data[offset], &latitude, 8); offset += 8;
        memcpy(&data[offset], &longitude, 8); offset += 8;
        memcpy(&data[offset], &altitude, 8); offset += 8;
        memcpy(&data[offset], &speed, 8); offset += 8;
        memcpy(&data[offset], &battery_percent, 2); offset += 2;
        memcpy(&data[offset], &status, 1); offset += 1;
        memcpy(&data[offset], &timestamp_ms, 8);
        return data;
    }

    TelemetryData TelemetryData::deserialize(const std::vector<uint8_t>& data) {
        TelemetryData td;
        size_t offset = 0;
        memcpy(&td.drone_id, &data[offset], 4); offset += 4;
        memcpy(&td.latitude, &data[offset], 8); offset += 8;
        memcpy(&td.longitude, &data[offset], 8); offset += 8;
        memcpy(&td.altitude, &data[offset], 8); offset += 8;
        memcpy(&td.speed, &data[offset], 8); offset += 8;
        memcpy(&td.battery_percent, &data[offset], 2); offset += 2;
        memcpy(&td.status, &data[offset], 1); offset += 1;
        memcpy(&td.timestamp_ms, &data[offset], 8);
        return td;
    }

    std::vector<uint8_t> CommandData::serialize() const {
        // ”прощЄнно: фиксированный размер + строка
        size_t size = 4 + 4 + 1 + 8 + 8 + 8 + mission_data.size();
        std::vector<uint8_t> data(size);
        size_t offset = 0;
        memcpy(&data[offset], &command_id, 4); offset += 4;
        memcpy(&data[offset], &target_drone_id, 4); offset += 4;
        memcpy(&data[offset], &type, 1); offset += 1;
        memcpy(&data[offset], &param1, 8); offset += 8;
        memcpy(&data[offset], &param2, 8); offset += 8;
        memcpy(&data[offset], &param3, 8); offset += 8;
        memcpy(&data[offset], mission_data.data(), mission_data.size());
        return data;
    }

    std::vector<uint8_t> MissionItem::serialize() const {
        std::vector<uint8_t> data(2 + 1 + 8 + 8 + 4 + 4 + 4);
        size_t offset = 0;
        memcpy(&data[offset], &seq, 2); offset += 2;
        memcpy(&data[offset], &type, 1); offset += 1;
        memcpy(&data[offset], &latitude, 8); offset += 8;
        memcpy(&data[offset], &longitude, 8); offset += 8;
        memcpy(&data[offset], &altitude, 4); offset += 4;
        memcpy(&data[offset], &param1, 4); offset += 4;
        memcpy(&data[offset], &param2, 4);
        return data;
    }

} // namespace Protocol