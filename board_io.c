//#include <avr/io.h>
// #include "includes/board_io.h"
// 
// volatile Circuit *Circ;
// 
// void board_init()
// {
//     circuit_init();
//     Circ = circuit_get_circuit();
// 		gpio_init();
// }
// 
// void board_write(SignalOutputs out, bool state)
// {
//     // write to the analog port
// }
// 
// void board_update(SignalInputs sig, bool state)
// {
//     Circ->signal_input[sig] = state;
//     circuit_update();
// 
//     for ( SignalOutputs out = 0; out < NUM_OF_OUTPUTS; out++ )
//     {
//         board_write(out, Circ->signal_output[out]);
//     }
// }
// 
