#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#include <avr/io.h>
#include "board_def.h"

void spi_init();

void spi_gpio(uint8_t io_pin, uint8_t val);

void spi_write(uint8_t cs_pin, uint8_t reg, uint8_t data);

uint8_t spi_read(uint8_t cs_pin, uint8_t reg);

uint8_t spi_xfer(uint8_t cs_pin, uint8_t data);


#endif /* SPI_DRIVER_H_ */