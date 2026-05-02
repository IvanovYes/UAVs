// FiniteStateUAVs - логика управления группой БВС
#pragma once
#include "Protocol.h"

enum class StatesUAVs : uint8_t 
{
    NOT_READY = 0,
    READY = 1,
    TAKEOFF = 2,
    LAND = 3,
    MISSION_RANNING = 4,   // Выполнение миссии
    PAUSE_MISSION = 5,     // Приостановка группы выполнения миссии -> групповое зависание
    RETURN_TO_BASE = 6,
    EMERGENCY_STOP = 7
};

class State {
protected:
    Context* context;

public:
    virtual ~State() {}

    void setContext( UAVsStateMachine* _context) 
    {
        this->context = _context;
    }

    virtual void takeOFF() = 0;
    virtual void land() = 0;
    virtual void startMission() = 0;
    virtual void pauseMission() = 0;
    virtual void returnToBase() = 0;
    virtual void cnangeMission() = 0;
    virtual void checkСonnectCondition() = 0;
};

class UAVsStateMachine {
public:
    UAVsStateMachine(State* state) : _state(nullptr)
    {
        this->changeSate(state);
    }
    ~UAVsStateMachine() {
        delete state;
    }

    void processEvent(Protocol::CommandData Event);
    // StatesUAVs getCurrentState() const { return state; }

    void changeSate(State* _state);

    // Для группы нужно учитывать состояние всех дронов
    // void updateDroneState(uint32_t drone_id, StatesUAVs state);

private:
    State* state;

    // bool allDronesInState(StatesUAVs state) const;
    // bool anyDroneInState(StatesUAVs state) const;
};

class NotReady : public State {
public:
    void takeOFF() override;
    void land() override;
    void startMission() override;
    void pauseMission() override;
    void changeMission() override;
    void returnToBase() override;
    void checkConnectCondition() override;
};

class Ready : public State {
public:
    void takeOFF() override;
    void land() override;
    void startMission() override;
    void pauseMission() override;
    void changeMission() override;
    void returnToBase() override;
    void checkConnectCondition() override;
};

class TakeOFF : public State {
public:
    void takeOFF() override;
    void land() override;
    void startMission() override;
    void pauseMission() override;
    void changeMission() override;
    void returnToBase() override;
    void checkConnectCondition() override;
};

class Land : public State {
public:
    void takeOFF() override;
    void land() override;
    void startMission() override;
    void pauseMission() override;
    void changeMission() override;
    void returnToBase() override;
    void checkConnectCondition() override;
};

class MissionRunning : public State {
public:
    void takeOFF() override;
    void land() override;
    void startMission() override;
    void pauseMission() override;
    void changeMission() override;
    void returnToBase() override;
    void checkConnectCondition() override;
};

class MissionStop : public State{
public:
    void takeOFF() override;
    void land() override;
    void startMission() override;
    void pauseMission() override;
    void changeMission() override;
    void returnToBase() override;
    void checkConnectCondition() override;
};

class ReturnToBase : public State {
public:
    void takeOFF() override;
    void land() override;
    void startMission() override;
    void pauseMission() override;
    void changeMission() override;
    void returnToBase() override;
    void checkConnectCondition() override;
};

class ChangeMission : public State {
public:
    void takeOFF() override;
    void land() override;
    void startMission() override;
    void pauseMission() override;
    void changeMission() override;
    void returnToBase() override;
    void checkConnectCondition() override;
};

class EmergancyStop : public State {
public:
    void takeOFF() override;
    void land() override;
    void startMission() override;
    void pauseMission() override;
    void changeMission() override;
    void returnToBase() override;
    void checkConnectCondition() override;
};

class ComandProcess {};