#include "stdio.h"
#include "includes/gpio_driver.h"
#include <avr/eeprom.h>

// EEPROM
// UART COMMANDS
// 1 - ENCODER TRIGGER LOW
// 2 - ENCODER TRIGGER HIGH
// 3 - CLK FOLLOWS ENC_B
// 4 - CLK INVERTS ENC_B
// 5 - M4 IN PHASE WITH ENC_B
// 6 - M4 INVERTS ENC_B

#include <util/delay.h>

int main(void)
{
    gpio_init(eeprom_read_byte(0x00));
	
	while(1)
		{
			;;
		}
	return 0;
}