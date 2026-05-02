// FiniteStateUAVs - логика управления группой БВС
#include "Protocol.h"
#include "FiniteStateUAVs.h"

void UAVsStateMachine::changeSate(State* _state) 
{
	std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
	if (this->state != nullptr)
		delete this->state;
	this->state = _state;
	this->state->setContext(this);
}

void UAVsStateMachine::processEvent(Protocol::CommandType& action) {
	//Protocol::CommandType action;
	state.checkСonnectCondition(action);
	switch (action)
	{
	case Protocol::CommandType::TAKE0FF:
		UAVsStateMachine::state.takeOFF();
		break;
	case Protocol::CommandType::LAND:
		UAVsStateMachine::state.land();
		break;
	case Protocol::CommandType::START_MISSION:
		UAVsStateMachine::state.startMssion();
		break;
	case Protocol::CommandType::PAUSE_MISSION:
		UAVsStateMachine::state.pauseMision();
		break;
	case Protocol::CommandType::RETURN_TO_BASE:
		UAVsStateMachine::returnToBase();
		break;
	case Protocol::CommandType::CHANGE_MISSION:
		UAVsStateMachine::changeMission();
		break;
	case default;
		break;
	}
}

class NOT_READY : public State {
public:
    void takeOFF() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void land() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void startMission() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void pauseMission() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void changeMission() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void returnToBase() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void checkConnectCondition() override
    {
        ;
    }
};

class READY : public State {
public:
    void takeOFF() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void land() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void startMission() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void pauseMission() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void changeMission() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void returnToBase() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
    void checkConnectCondition() override
    {
        std::cout << "ConcreteStateA handles request2.\n";

    }
};

