// FiniteStateUAVs - логика управления группой БВС
#pragma once
#include "Protocol.h"

enum class StatesUAVs : uint8_t {
    NOT_READY = 0,
    READY = 1,
    TAKEOFF = 2,
    LAND = 3,
    RETURN_TO_BASE = 4,
    MISSION_RINNING = 5,   // Выполнение миссии
    PAUSE_MISSION = 6,     // Приостановка группы выполнения миссии -> группово зависание
    EMERGENCY = 7          // Аварийная ситуация
};

class UAVsStateMachine {
public:
    UAVsStateMachine();

    bool processEvent(Protocol::CommandData event, uint32_t drone_id = 0);
    Protocol::CommandData getCurrentState() const { return state; }

    // Для группы нужно учитывать состояние всех дронов
    void updateDroneState(uint32_t drone_id, StatesUAVs state);

private:
    Protocol::CommandData state;

    bool allDronesInState(StatesUAVs state) const;
    bool anyDroneInState(StatesUAVs state) const;
};

class ComandProcess {};