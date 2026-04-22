#pragma once
#pragma once
#include "../common/protocol.h"
#include <unordered_map>
#include <mutex>
#include <vector>
#include <chrono>

class DataUAVs {
public:
    struct DataUAV {
        bool is_connected;
        std::string ip_address;
        uint16_t port;

        DataUAV() : is_connected(false), port(0) {}
    };

    void updateUAV(const std::string& ip, uint16_t port);

    DataUAV getUAV(uint32_t id) const;
    std::vector<uint32_t> getAllActiveUAVs() const;
    void removeStaleUAVS(int timeout_seconds = 10);

    size_t getActiveCount() const;

private:
    //mutable std::mutex m_mutex;
    std::unordered_map<uint32_t, DataUAV> UAVs;
};