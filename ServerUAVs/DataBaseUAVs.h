#pragma once
#pragma once
#include "../common/protocol.h"
#include <unordered_map>
#include <mutex>
#include <vector>
#include <chrono>

struct DataUAV {
    bool isConnected;
    std::string ip_address;
    uint16_t port;

    DataUAV() : isConnected(false), port(0) {}
};


class DataUAVs {
public:
    
    void updateUAV(const std::string& ip, uint16_t port);

    DataUAV getUAV(uint32_t id) const;
    std::vector<uint32_t> getAllActiveUAVs() const;
    void removeStateUAVS(int timeout_seconds = 10);

    size_t getActiveCount() const;

private:
    //mutable std::mutex m_mutex;
    std::unordered_map<uint32_t, DataUAV> UAVs;
};