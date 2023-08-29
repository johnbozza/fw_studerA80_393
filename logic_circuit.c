#include "includes/logic_circuit.h"

Circuit circuit;
uint8_t cstates[] = { 0x01, 0x07, 0x03, 0x0E, 0x10, 0x09 };
	/*
				FF0		FF1		FF2		FF3		FF4
	st_INIT :	1		0		0		0		0
	st_FORW :	1		1		1		0		0
	st_REW	:	1		1		0		0		0
	st_REPR	:	0		1		1		1		0
	st_REC	:	0		1		1		1		1
	st_CUT	:	1		0		0		1		0
	*/	
	
#define IS_MOVING		0
#define NOT_MOVING		1

#define TAPE_IN		0
#define TAPE_OUT	1

#define PRESSED		0

Circuit * circuit_get_circuit()
{
    return &circuit;
}

void circuit_init()
{
	circuit.move = NOT_MOVING;
	circuit.state = cstates[st_INIT];
}

void circuit_do_outputs()
{
        circuit.signal_output[MOVE2_OUT] = circuit.signal_input[MOVE_IN];
        circuit.signal_output[FF0_OUT] = circuit.state & 0x01;
        circuit.signal_output[FF1_OUT] = circuit.state & 0x02;
        circuit.signal_output[FF2_OUT] = circuit.state & 0x04;
        circuit.signal_output[FF3_OUT] = circuit.state & 0x08;
        circuit.signal_output[FF4_OUT] = circuit.state & 0x10;
		circuit.signal_output[CUT_OUT] = circuit.signal_input[CUT_IN];
        circuit.signal_output[REC_OUT] = circuit.signal_output[FF4_OUT];
}
void circuit_update()
{
	bool static isRepr = false;
	bool static isRec  = false;
	
	bool ReprBtPressed = circuit.signal_input[REPR_IN] == PRESSED;
	bool recBtPressed  = circuit.signal_input[REC_IN]  == PRESSED;
	
	bool fadin_active  = circuit.signal_input[FAD_IN]  == true;
	
	if ( circuit.signal_input[END_IN] == TAPE_IN )
	{
		if (!fadin_active)
		{
			if ( ( isRepr && isRec ) && ( ReprBtPressed || recBtPressed ) ) return;
					
			if ( ( ( isRepr || ( isRepr && isRec ) ) && ( circuit.signal_input[MOVE_IN] == IS_MOVING ) && ( !( circuit.state & 0x01 ) ) ) )
			{
				circuit.state |= 0x01;
				circuit_do_outputs();
				return;
			}
			
			if ( ( isRec && ReprBtPressed ) || ( isRepr &&  recBtPressed ) )
			{
				isRepr = true;
				isRec  = true;
				circuit.state |= cstates[st_REPR] | cstates[st_REC];
				circuit_do_outputs();
				return;
			}
		}
		
		if		( circuit.signal_input[STOP_IN] == PRESSED )					{ circuit.state = cstates[st_INIT]; isRec = false; isRepr = false; }
		else if ( circuit.signal_input[FORW_IN] == PRESSED && !fadin_active )	  circuit.state = cstates[st_FORW];
		else if ( circuit.signal_input[REW_IN]	== PRESSED && !fadin_active )	  circuit.state = cstates[st_REW];	
		else if ( circuit.signal_input[REPR_IN] == PRESSED )					{ circuit.state = cstates[st_REPR]; isRepr = true; }
		else if ( circuit.signal_input[REC_IN]	== PRESSED && !fadin_active )	{ circuit.state = cstates[st_REC];	isRec  = true;  }
		else if ( circuit.signal_input[CUT_IN]	== PRESSED && !fadin_active )	  circuit.state = cstates[st_CUT];
	}
	else
	{
		circuit.state = cstates[st_INIT];
		isRepr = false;
		isRec  = false;
	}
	
	circuit_do_outputs();
}
