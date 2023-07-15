#include "includes/spi_driver.h"

void spi_init()
{
	// enable SPI, set as master, and clock to fosc/128
	PORTC = 0xff;
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
	spi_write(SPI_CS1, MCP23S08_IODIR, 0x00); // configure MCP23S08 01 as output
	spi_write(SPI_CS2, MCP23S08_IODIR, 0x00); // configure MCP23S08 02 as output
	spi_write(SPI_CS1, MCP23S08_GPIO, 0x00);
	spi_write(SPI_CS2, MCP23S08_GPIO, 0x00);
	
}
void spi_gpio(uint8_t io_pin, uint8_t val)
{
	uint8_t cs_pin = ( io_pin >> 4 ) & 0x01;
	
	if ( cs_pin == 0 ) cs_pin = SPI_CS2; // U2
	if ( cs_pin == 1 ) cs_pin = SPI_CS1; // U1
	
	io_pin &= 0x0f;
	
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


