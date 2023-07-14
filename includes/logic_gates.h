#include "stdbool.h"
#include "stdint.h"

#ifndef LOGIC_GATES_H_
#define LOGIC_GATES_H_

typedef struct FlipFlop
{
	bool reset_port, set_port, set_state;
} FlipFlop;

/*
	make_flipflop(FlipFlop *new_flipflop, uint8_t name, uint8_t size)

	Populates the newly created flip flop with the standard start-up values
	name : number assigned to the flipflop to keep track of it
	size : number of inputs. The xtra input will be always set to TRUE
*/
void make_flipflop(FlipFlop *new_flipflop);


/*
	do_flipflop(FlipFlop *flipflop, FlipFlop_ports *ports)

	receives the 2 or 3 values from each input of both set and reset NANDs from *ports
	if previous RESET state was 0 and now it is 1, sets the OUTPUT to 0
	if previous SET state was 0 and now it is 1, sets the OUTPUT to 1
	assign the current values to values received
*/
void do_flipflop(FlipFlop *flipflop, bool new_reset, bool new_set);

#endif /* LOGIC_GATES_H_ */
