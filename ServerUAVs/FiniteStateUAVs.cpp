// FiniteStateUAVs - логика управления группой БВС
#include "Protocol.h"
#include "FiniteStateUAVs.h"

void FiniteStateUAVs::processEvent(Protocol::CommandType& action) {
	Protocol::CommandType action;
	switch (action)
	{
		case Protocol::CommandType::
	}
	var fsm_input = FSM_Meta.FSM_Input.new(actions, is_on_floor())
	var new_state_id = state. 
	if new_state_id != FSM_Meta.FSM_States.None:
		var new_state = get_new_state(new_state_id)
		state.exit()
		state = new_state
		state.enter()
	state.update(fsm_input, delta)
}