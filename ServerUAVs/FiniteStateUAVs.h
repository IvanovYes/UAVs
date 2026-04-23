// FiniteStateUAVs - логика управления группой БВС
#pragma once
#include "Protocol.h"

enum class StatesUAVs : uint8_t {
    NOT_READY = 0,
    READY = 1,
    TAKEOFF = 2,           // Взлёт
    LAND = 3,              // Посадка
    RETURN_TO_BASE = 4,
    START_MISSION = 5,
    PAUSE_MISSION = 6,
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

class ComandProcess {

};