#include "includes/logic_gates.h"

void logic_gates_make_flipflop(FlipFlop *new_flipflop)
{
	new_flipflop->reset_port	= true;
	new_flipflop->set_port		= true;
	new_flipflop->set_state		= true;
}

void logic_gates_do_flipflop(FlipFlop *flipflop, bool new_reset, bool new_set)
{
	if ( new_reset	&&	( ! flipflop->reset_port) ) flipflop->set_state = false;
	if ( new_set	&&	( ! flipflop->set_port) )	flipflop->set_state = true;

	flipflop->reset_port = new_reset;
	flipflop->set_port   = new_set;
}
