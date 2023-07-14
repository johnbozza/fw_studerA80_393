#include "includes/gpio_driver.h"

void gpio_init()
{

	cli();
	
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
	DDRB = 0x2C;
	
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
	DDRC |= 0x03;
	
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
	DDRD = 0x02;
	
	uart_setup();
	
	// enable SPI, set as master, and clock to fosc/128
	PORTC = 0xff;
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
	spi_write(SPI_CS1, MCP23S08_IODIR, 0x00); // configure MCP23S08 01 as output
	spi_write(SPI_CS2, MCP23S08_IODIR, 0x00); // configure MCP23S08 02 as output
	

	
	// Configure ENC A and ENC B interrupts
	/*
	EICRA |= ( 1 << ISC00 ) | ( 1 << ISC01 ); // INT0 FALLING EDGE
	EICRA |= ( 1 << ISC10 ) | ( 1 << ISC11 ); // INT1 LOW LEVEL
	
	EIMSK |= ( 1 << INT0 ) | ( 1 << INT1 );
	*/
	

}

void uart_setup()
{
	 /* Set Baudrate  */
	 unsigned int ubrr = BAUD_RATE_115200_BPS;
	 UBRR0H = (ubrr>>8);
	 UBRR0L = (ubrr);
	 
	 UCSR0C = 0x06;       /* Set frame format: 8data, 1stop bit  */
	 UCSR0B = ( 1 << TXEN0 ); /* Enable  transmitter                 */
}

void spi_gpio(uint8_t io_pin, uint8_t val)
{
	uint8_t cs_pin = ( io_pin >> 4 ) & 0x01;
	
	uint8_t data = spi_read(cs_pin, MCP23S08_GPIO);
	
	if ( val )
	{
		spi_write(cs_pin, MCP23S08_GPIO, ( data | ( 1 << io_pin ) ) );
	}
	else
	{
		spi_write(cs_pin, MCP23S08_GPIO, ( data & ~( 1 << io_pin ) ) );
	}
}

void spi_write(uint8_t cs_pin, uint8_t reg, uint8_t data)
{
	SPI_PORT &= ~( 1 << cs_pin );
		spi_xfer(SPI_CS1, MCP23S08_OPCOD);
		spi_xfer(SPI_CS1, reg);
		spi_xfer(SPI_CS1, data);
	SPI_PORT |= ( 1 << cs_pin );
}

uint8_t spi_read(uint8_t cs_pin, uint8_t reg)
{
	uint8_t data;
	SPI_PORT &= ~( 1 << cs_pin );
		spi_xfer(cs_pin, MCP23S08_OPCOD + 0x01);
		spi_xfer(cs_pin, reg);
		data = spi_xfer(cs_pin, 0xff); // dummy byte
	SPI_PORT |= ( 1 << cs_pin );
	
	return (uint8_t)data;
}


uint8_t spi_xfer(uint8_t cs_pin, uint8_t data)
{
	SPDR = data;	
	while(!(SPSR & (1 << SPIF)));
	
	return SPDR;
}

