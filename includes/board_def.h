#ifndef BOARD_PINS_H_
#define BOARD_PINS_H_

// COMMS
#define UART_TX		PIND1
#define UART_RX		PIND0
#define SPI_MISO	PINB3
#define SPI_MOSI	PINB4
#define SPI_CLK		PINB5
#define SPI_CS1		PINC1 // U1
#define SPI_CS2		PINC0 // U2

#define SPI_PORT	PORTC

// INPUTS
#define YPS_REC_PIN			PINC2
#define YPS_STOP_PIN		PINC3
#define YPS_REPR_PIN		PINC4
#define YPS_FORW_PIN		PINC5
#define YPS_REW_PIN			PIND4
#define YPS_CUT_PIN			PIND5
#define YBI_END_PIN			PIND6
#define YBI_MOVE_PIN		PIND7
#define YBI_FAD_PIN			PINB0
#define YBI_LOW_PIN			PINB1
#define QP_DIR1_PIN			PIND2
#define QP_DIR2_PIN			PIND3
#define AUX_LED_PIN			PINB2

#define YPS_REC_PORT		PINC
#define YPS_STOP_PORT		PINC 
#define YPS_REPR_PORT		PINC
#define YPS_FORW_PORT		PINC 
#define YPS_REW_PORT		PIND
#define YPS_CUT_PORT		PIND
#define YBI_END_PORT		PIND 
#define YBI_MOVE_PORT		PIND 
#define YBI_FAD_PORT		PINB
#define YBI_LOW_PORT		PINB
#define QP_DIR1_PORT		PIND
#define QP_DIR2_PORT		PIND 
#define AUX_LED_PORT		PINB

#define _READ_PIN(Port, Pin)	( Port & ( 1 << Pin ) )

// OUTPUTS MCP23S08
// U1
#define YBI_CUT		0x10
#define YBI_FF1		0x11
#define YBI_MOV2	0x13
#define YBI_FF0		0x12
#define YBI_FF2		0x14
#define YBI_FF3		0x15
#define M4_3		0x16
#define M4_4		0x17
// U2
#define YBI_CLK		0x00
#define YBI_DIR		0x01
#define B_REC		0x03
#define M4_2		0x02
#define M4_1		0x04
#define YBI_FF4		0x05
#define YBI_STOP	0x06

#define MCP23S08_IODIR	0x00
#define MCP23S08_OPCOD	0x40
#define MCP23S08_GPIO	0x09

// Baud = [ Fosc / ( baud * 16 ) ] - 1
#define BAUD_RATE_9600_BPS  103  // 9600bps
#define BAUD_RATE_19200_BPS  51 // 19.2k bps
#define BAUD_RATE_57600_BPS  16 // 57.6k bps
#define BAUD_RATE_115200_BPS  8 // 115.2k bps

#endif /* BOARD_PINS_H_ */