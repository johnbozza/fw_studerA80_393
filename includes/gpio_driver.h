#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include <stdbool.h>

#define F_CPU			16000000
#define TIMER_1S		65535 - ( F_CPU / 1024 ) / 32

#include <avr/interrupt.h>
#include <avr/io.h>
#include "board_def.h"
#include "spi_driver.h"
#include "logic_circuit.h"

void gpio_init();
void gpio_uart_setup();
void gpio_do_uart_rx();
void gpio_timer_init();
void gpio_do_timer();
void gpio_do_update();

	// PORT B
	// PB0 pin 14 IN	YBI_FAD
	// PB1 pin 15 IN	YBI_LOW
	// PB2 pin 16 OUT	AUX LED
	// PB3 pin 17 OUT	SPI_MOSI
	// PB4 pin 18 IN	SPI_MISO
	// PB5 pin 19 OUT	SPI_SCK
	// PB6 pin 09 XTAL
	// PB7 pin 10 XTAL
	// X X 1 0 1 1 0 0
			
	// PORT C
	// PC0 pin 23 OUT	SPI_CS1
	// PC1 pin 24 OUT	SPI_CS2
	// PC2 pin 25 IN	YPS_REC
	// PC3 pin 26 IN	YPS_STOP
	// PC4 pin 27 IN	YPS_REPR
	// PC5 pin 28 IN	YSP_FORW
	// PC6 pin 01		RESET
	// PC7 N/C
	// X X 0 0 0 0 1 1

	// PORT D
	// PD0 pin 02 IN	UART_RX
	// PD1 pin 03 OUT	UART_TX
	// PD2 pin 04 IN	INT0 (ENC A)
	// PD3 pin 05 IN	INT1 (ENC B)
	// PD4 pin 06 IN	YPS_REW
	// PD5 pin 11 IN	YPS_CUT
	// PD6 pin 12 IN	YBI_END
	// PD7 pin 13 IN	YBI_MOVE
	// 0 0 0 0 0 0 1 0
	
#endif /* GPIO_DRIVER_H_ */