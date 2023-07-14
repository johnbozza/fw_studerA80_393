#ifndef BOARD_DEF_H_
#define BOARD_DEF_H_

#include <stdbool.h>
#include <stdint.h>
#include "board_def.h"
#include "gpio_driver.h"
#include "logic_circuit.h"


void board_init();

bool board_read(uint8_t c);

#endif /* BOARD_DEF_H_ */


