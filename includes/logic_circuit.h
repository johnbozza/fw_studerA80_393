#ifndef LOGIC_CIRCUIT_H_
#define LOGIC_CIRCUIT_H_

#include <stdbool.h>
#include "logic_gates.h"

#define NUM_OF_INPUTS        10
#define NUM_OF_OUTPUTS       14
#define NUM_OF_FLIPFLOPS     5

typedef enum { FAD_IN, REC_IN, END_IN, CUT_IN, REW_IN, STOP_IN, MOVE_IN, FORW_IN, REPR_IN, LOW_IN } SignalInputs;
typedef enum { FLFP_01_OUT, FLFP_02_OUT, FLFP_03_OUT, FLFP_04_OUT, FLFP_05_OUT } FlipFlopsOutput;
typedef enum { MOVE2_OUT, FF0_OUT, FF1_OUT, FF2_OUT, FF3_OUT, FF4_OUT, CUT_OUT, REC_OUT, M4_1_OUT, M4_2_OUT, M4_3_OUT, M4_4_OUT, DIR_OUT, CLK_OUT } SignalOutputs;

typedef struct circuit
{
    FlipFlop ffs[NUM_OF_FLIPFLOPS];
    bool signal_input[NUM_OF_INPUTS];
    bool signal_output[NUM_OF_OUTPUTS];

} Circuit;

void circuit_init();
void circuit_update();
// static void circuit_do_outputs();
// static void circuit_do_flfp();
Circuit * circuit_get_circuit();

#define AND(a,b)				    (bool)(a && b)
#define OR(a,b)					    (bool)(a || b)
#define NOT(a)					    (bool)(!a)

/*
//	SIGNALS
#define FLFP_01_LOGIC	(MOVE_IN, AND( FF3_OUT, FF2_OUT ))
#define FLFP_02_LOGIC	AND(STATE_01, STATE_02), AND(STATE_03, STATE_04)
#define FLFP_03_LOGIC	AND(STATE_03, AND(STATE_02, REW_IN) ), AND(STATE_04, FORW_IN)
#define FLFP_04_LOGIC	AND(STATE_03, STATE_01), AND(STATE_02, STATE_04)
#define FLFP_05_LOGIC	AND(STATE_02, AND(STATE_01, STATE_03)), REC_IN

#define STATE_01_LOGIC  AND( FAD_IN, AND ( NOT(STOP_IN), NOT(END_IN)))
#define STATE_02_LOGIC  (OR( CUT_IN, MOVE_IN))
#define STATE_03_LOGIC  (AND( REW_IN, FORW_IN))
#define STATE_04_LOGIC  (AND( NOT( AND( END_IN, FAD_IN ) ), REPR_IN))
// Logic INPUTs
#define FAD_IN			GET(YBI_FAD)
#define REC_IN			GET(YBI_FAD)
#define END_IN			GET(YBI_END)
#define CUT_IN			GET(YPS_CUT)
#define REW_IN			GET(YPS_REW)
#define STOP_IN			GET(YPS_STOP)
#define MOVE_IN			GET(YBI_MOVE)
#define FORW_IN			GET(YPS_FORW)
#define REPR_IN			GET(YPS_REPR)

// States coming from Buttons and inputs
#define STATE_01		AND( FAD_IN, NOT(STOP_IN), NOT(END_IN) )
#define STATE_02		OR(  CUT_IN, MOVE_IN )
#define STATE_03		AND( REW_IN, FORW_IN )
#define STATE_04		AND( NOT( AND( END_IN, FAD_IN ) ), REPR_IN )

// Signals FlipFlops
#define FLFP_01_OUT		DO_FLFP(FLFP_01, MOVE_IN, AND( FF3_OUT, FF2_OUT ))
#define FLFP_02_OUT		DO_FLFP(FLFP_02, AND(STATE_01, STATE_02), AND(STATE_03, STATE_04) )
#define FLFP_03_OUT		DO_FLFP(FLFP_03, AND(STATE_03, AND(STATE_02, REW_IN) ), AND(STATE_04, FORW_IN) )
#define FLFP_04_OUT		DO_FLFP(FLFP_04, AND(STATE_03, STATE_01), AND(STATE_02, STATE_04) )
#define FLFP_05_OUT		DO_FLFP(FLFP_05, AND(STATE_02, AND(STATE_01, STATE_03)), REC_IN )

// Logic OUTPUTs
#define MOVE2_OUT		NOT( MOVE_IN )
#define FF0_OUT			AND( FLFP_01_OUT, AND ( FF2_OUT, FF3_OUT ) )
#define FF1_OUT			FLFP_02_OUT
#define FF2_OUT			FLFP_03_OUT
#define FF3_OUT			FLFP_04_OUT
#define FF4_OUT			FLFP_05_OUT
#define CUT_OUT			CUT_IN
#define REC_OUT			FLFP_05_OUT


*/

#endif /* LOGIC_CIRCUIT_H_ */
