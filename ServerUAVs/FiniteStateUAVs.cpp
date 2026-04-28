// FiniteStateUAVs - логика управления группой БВС
#include "Protocol.h"
#include "FiniteStateUAVs.h"

void FiniteStateUAVs::processEvent(d) {
	Protocol::CommandType actions = [];
		for action in input_actions :
	if Input.is_action_pressed(action) :
		actions.append(action)
	var fsm_input = FSM_Meta.FSM_Input.new(actions, is_on_floor())
	var new_state_id = state.input(fsm_input)
	if new_state_id != FSM_Meta.FSM_States.None :
		var new_state = get_new_state(new_state_id)
		state.exit()
		state = new_state
		state.enter()
	state.update(fsm_input, delta)
}