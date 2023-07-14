#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include "board_def.h"
#include <avr/interrupt.h>
#include <avr/io.h>

void uart_setup();
void gpio_init();
void spi_write(uint8_t cs_pin, uint8_t reg, uint8_t data);
void spi_gpio(uint8_t io_pin, uint8_t val);
uint8_t spi_xfer(uint8_t cs_pin, uint8_t data);
uint8_t spi_read(uint8_t cs_pin, uint8_t reg);

#endif /* GPIO_DRIVER_H_ */