#include "includes/logic_gates.h"

void logic_gates_make_flipflop(FlipFlop *new_flipflop)
{
	new_flipflop->reset_port	= true;
	new_flipflop->set_port		= true;
	new_flipflop->set_state		= true;
}

void logic_gates_do_flipflop(FlipFlop *flipflop, bool new_reset, bool new_set)
{
	// Cannot have SET and RESET both at LOW
	if ( ( new_reset == 0 ) && ( new_set == 0 ) ) return;
	
	if ( new_set == 0 ) flipflop->set_state = true;
	if ( new_reset== 0 ) flipflop->set_state = false;
	
	// if set = 0 and reset = 0, hold state and update
	
// 	if ( new_reset == false	&&	( flipflop->reset_port == true) ) flipflop->set_state = false;
// 	if ( new_set == false	&&	( flipflop->set_port == true )	) flipflop->set_state = true;

	flipflop->reset_port = new_reset;
	flipflop->set_port   = new_set;
}
