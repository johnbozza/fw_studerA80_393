#include "includes/logic_circuit.h"

Circuit circuit;

Circuit * circuit_get_circuit()
{
    return &circuit;
}

void circuit_init()
{
    for (int i = 0; i < NUM_OF_FLIPFLOPS; i++)
    {
        FlipFlop tmp_ff;
        logic_gates_make_flipflop(&tmp_ff);
        circuit.ffs[i] = tmp_ff;
    }
}

static void circuit_do_flfp()
{
    for ( FlipFlopsOutput flfp = 0; flfp < NUM_OF_FLIPFLOPS; flfp++)
    {
        if ( flfp == FLFP_01_OUT )
        {
            logic_gates_do_flipflop((FlipFlop*)&circuit.ffs[flfp],
                            circuit.signal_input[MOVE_IN],
                            ( circuit.signal_output[FF3_OUT] && circuit.signal_output[FF4_OUT] ) );//REVIEW THIS ON FF IS NOT FLFP
        }
// 		R (FAD+STOP+END)(CUT+MOVE)
// 		S: (REW.FORW)( (END.FAD) . REPR)
        if ( flfp == FLFP_02_OUT )
        {
            logic_gates_do_flipflop((FlipFlop*)&circuit.ffs[flfp],
                            ( circuit.signal_input[FAD_IN] || NOT(circuit.signal_input[STOP_IN]) || NOT(circuit.signal_input[END_IN]) ) 
							&& ( circuit.signal_input[CUT_IN] || circuit.signal_input[MOVE_IN] ),
							( circuit.signal_input[REW_IN] && circuit.signal_input[FORW_IN] ) 
							&& ( ( NOT( circuit.signal_input[END_IN] && circuit.signal_input[FAD_IN] ) ) && circuit.signal_input[REPR_IN] )

                        );
        }
// 		R (REW.FORW)(CUT + MOVE)(REW)
// 		S: ( (END.FAD) . REPR )(FORW)
        if ( flfp == FLFP_03_OUT )
        {
            logic_gates_do_flipflop((FlipFlop*)&circuit.ffs[flfp],
                            ( (circuit.signal_input[REW_IN] && circuit.signal_input[FORW_IN]) && (circuit.signal_input[CUT_IN] || circuit.signal_input[MOVE_IN] ) && circuit.signal_input[REW_IN] ),
							( ( NOT( circuit.signal_input[END_IN] && circuit.signal_input[FAD_IN] ) && circuit.signal_input[REPR_IN] ) && circuit.signal_input[FORW_IN] )
                        );
        }
        if ( flfp == FLFP_04_OUT )
        {
            logic_gates_do_flipflop((FlipFlop*)&circuit.ffs[flfp],
                            AND((AND( circuit.signal_input[REW_IN], circuit.signal_input[FORW_IN])),
                                    AND( circuit.signal_input[FAD_IN], AND ( NOT(circuit.signal_input[STOP_IN]), NOT(circuit.signal_input[END_IN])))),
                            AND((OR( circuit.signal_input[CUT_IN], circuit.signal_input[MOVE_IN])),
                                    (AND( NOT( AND( circuit.signal_input[END_IN], circuit.signal_input[FAD_IN] ) ), circuit.signal_input[REPR_IN])))
                        );
        }
        if ( flfp == FLFP_05_OUT )
        {
            logic_gates_do_flipflop((FlipFlop*)&circuit.ffs[flfp],
                            AND((OR( circuit.signal_input[CUT_IN], circuit.signal_input[MOVE_IN])),
                                    AND(AND( circuit.signal_input[FAD_IN], AND ( NOT(circuit.signal_input[STOP_IN]), NOT(circuit.signal_input[END_IN]))),
                                        (AND( circuit.signal_input[REW_IN], circuit.signal_input[FORW_IN])))),
                            circuit.signal_input[REC_IN]
                        );
        }
    }
}

static void circuit_do_outputs()
{
        circuit.signal_output[MOVE2_OUT] = NOT( circuit.signal_input[MOVE_IN] );

        circuit.signal_output[FF0_OUT] = NOT( circuit.ffs[FLFP_01_OUT].set_state && circuit.signal_output[FF2_OUT] && circuit.signal_output[FF3_OUT] );

        circuit.signal_output[FF1_OUT] = circuit.ffs[FLFP_02_OUT].set_state;
		
        circuit.signal_output[FF2_OUT] = circuit.ffs[FLFP_03_OUT].set_state;

        circuit.signal_output[FF3_OUT] = circuit.ffs[FLFP_04_OUT].set_state;

        circuit.signal_output[FF4_OUT] = circuit.ffs[FLFP_05_OUT].set_state;
		
		circuit.signal_output[CUT_OUT] = circuit.signal_input[CUT_IN];

        circuit.signal_output[REC_OUT] = circuit.ffs[FLFP_05_OUT].set_state;
		
// 		if ( out == M4_1_OUT )	circuit.signal_output[out] = 0; 
// 		
// 		if ( out == M4_2_OUT )	circuit.signal_output[out] = 0; 
// 		
// 		if ( out == M4_3_OUT )	circuit.signal_output[out] = 0; 
// 		
// 		if ( out == M4_4_OUT )	circuit.signal_output[out] = 0; 
// 		
// 		if ( out == DIR_OUT )	circuit.signal_output[out] = 0; 
// 		
// 		if ( out == CLK_OUT )	circuit.signal_output[out] = 0; 
}
void circuit_update()
{
    circuit_do_flfp();

    circuit_do_outputs();
}
