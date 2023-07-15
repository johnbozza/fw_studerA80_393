#include "includes/gpio_driver.h"
volatile Circuit *Circ;

void gpio_init()
{

	cli();
	
	circuit_init();
	Circ = circuit_get_circuit();
	
	DDRB = 0x2C;
	PORTB = 0xff;
	
	DDRC |= 0x03;
	PORTC = 0xff;
	
	DDRD = 0x02;
	PORTD = 0xff;
	
	gpio_uart_setup();
	spi_init();
	gpio_timer_init();
	
	sei();

	// Configure ENC A and ENC B interrupts

	EICRA |= ( 1 << ISC00 ) | ( 1 << ISC01 ); // INT0 FALLING EDGE
	EICRA |= ( 1 << ISC10 ) | ( 1 << ISC11 ); // INT1 LOW LEVEL
	EIMSK |= ( 1 << INT0 )  | ( 1 << INT1  );
	
	PCICR  |= (1 << PCIE0) | (1 << PCIE1)  | (1 << PCIE2);		
	PCMSK0 |= ( 1 << YBI_FAD_PIN ) | ( 1 << YBI_LOW_PIN );		
	PCMSK1 |= ( 1 << YPS_REC_PIN ) | ( 1 << YPS_STOP_PIN ) | ( 1 << YPS_REPR_PIN ) | ( 1 << YPS_FORW_PIN );
	PCMSK2 |= ( 1 << YPS_REW_PIN ) | ( 1 << YPS_CUT_PIN )  | ( 1 << YBI_END_PIN )  | ( 1 << YBI_MOVE_PIN );		

}

void gpio_uart_setup()
{
	 unsigned int ubrr = BAUD_RATE_115200_BPS;
	 UBRR0H = (ubrr>>8);
	 UBRR0L = (ubrr);
	 
	 UCSR0C = 0x06;       /* Set frame format: 8data, 1stop bit  */
	 UCSR0B = ( 1 << TXEN0 ) | ( 1 << RXEN0 ) | ( 1 << RXCIE0 ); // TX, RX and RX INT
}

void gpio_do_uart_rx()
{

	for (int i = 0; i < NUM_OF_OUTPUTS; i ++)
	{
		UDR0 = Circ->signal_output[i];
		while (!( UCSR0A & (1<<UDRE0)));
	}
	
	
}

void gpio_do_timer()
{
	static uint8_t mcu_led_period = 0;
	static bool ping_pong = false;
	cli();
	
	gpio_do_update();
	
	if ( ++mcu_led_period > 16)
	{
		PORTB ^= ( 1 << AUX_LED_PIN );
		mcu_led_period = 0;
		gpio_do_uart_rx();
	}
// 	spi_gpio(M4_1, !ping_pong);
// 	spi_gpio(M4_2, ping_pong);
// 	spi_gpio(M4_3, !ping_pong);
// 	spi_gpio(M4_4, ping_pong);
// 	spi_gpio(YBI_FF1, ping_pong);
// 	spi_gpio(YBI_FF2, !ping_pong);
// 	spi_gpio(YBI_FF3, ping_pong);
// 	
// 	ping_pong = !ping_pong;
// 	//UDR0 = spi_read(0,9);
// 	//while (!( UCSR0A & (1<<UDRE0)));
	TCNT1 = TIMER_1S;
	sei();
}

void gpio_timer_init()
{
	TCNT1 = TIMER_1S;
	TCCR1B = ( 1 << CS10 ) | ( 1 << CS12 ); // 16M / 1024 = 15625Hz
	TCCR1A = 0x00; // overflow
	TIMSK1 = ( 1 << TOIE1 ); // overflow int
}

void gpio_do_update()
{	
	
	//FAD_IN, REC_IN, END_IN, CUT_IN, REW_IN, STOP_IN, MOVE_IN, FORW_IN, REPR_IN, LOW_IN 
	Circ->signal_input[FAD_IN]	= _READ_PIN(YBI_FAD_PORT,	YBI_FAD_PIN);
	Circ->signal_input[REC_IN]	= _READ_PIN(YPS_REC_PORT,	YPS_REC_PIN);
	Circ->signal_input[END_IN]	= _READ_PIN(YBI_END_PORT,	YBI_END_PIN);
	Circ->signal_input[CUT_IN]	= _READ_PIN(YPS_CUT_PORT,	YPS_CUT_PIN);
	Circ->signal_input[REW_IN]	= _READ_PIN(YPS_REW_PORT,	YPS_REW_PIN);
	Circ->signal_input[STOP_IN]	= _READ_PIN(YPS_STOP_PORT,	YPS_STOP_PIN);
	Circ->signal_input[MOVE_IN]	= _READ_PIN(YBI_MOVE_PORT,	YBI_MOVE_PIN);
	Circ->signal_input[FORW_IN]	= _READ_PIN(YPS_FORW_PORT,	YPS_FORW_PIN);
	Circ->signal_input[REPR_IN]	= _READ_PIN(YPS_REW_PORT,	YPS_REPR_PIN);
	Circ->signal_input[LOW_IN]	= _READ_PIN(YBI_LOW_PORT,	YBI_LOW_PIN);
	
	circuit_update();

	spi_gpio(YBI_MOV2,	Circ->signal_output[MOVE2_OUT]);
	spi_gpio(YBI_FF0,	Circ->signal_output[FF0_OUT]);
	spi_gpio(YBI_FF1,	Circ->signal_output[FF1_OUT]);
	spi_gpio(YBI_FF2,	Circ->signal_output[FF2_OUT]);
	spi_gpio(YBI_FF3,	Circ->signal_output[FF3_OUT]);
	spi_gpio(YBI_FF4,	Circ->signal_output[FF4_OUT]);
	spi_gpio(YBI_CUT,	Circ->signal_output[CUT_OUT]);
	spi_gpio(B_REC,		Circ->signal_output[REC_OUT]);
	spi_gpio(M4_1,		Circ->signal_output[M4_1_OUT]);
	spi_gpio(M4_2,		Circ->signal_output[M4_2_OUT]);
	spi_gpio(M4_3,		Circ->signal_output[M4_3_OUT]);
	spi_gpio(M4_4,		Circ->signal_output[M4_4_OUT]);
	spi_gpio(YBI_DIR,	Circ->signal_output[DIR_OUT]);
	spi_gpio(YBI_CLK,	Circ->signal_output[CLK_OUT]);
	spi_gpio(YBI_STOP,	0);

}

/*INTERRUPTS*/

ISR (INT0_vect)
{
	cli();
		UDR0 = 0x00;
		while (!( UCSR0A & (1<<UDRE0)));
	sei();
}

ISR (INT1_vect)
{
	cli();
		UDR0 = 0x01;
		while (!( UCSR0A & (1<<UDRE0)));	
	sei();
}

ISR (TIMER1_OVF_vect)
{
	gpio_do_timer();
}

ISR (USART_RX_vect)
{
	cli();
	UDR0;
	gpio_do_uart_rx();
	sei();
}

ISR (PCINT0_vect)
{
	gpio_do_update();	
}

ISR (PCINT1_vect)
{
	gpio_do_update();	
}

ISR (PCINT2_vect)
{
	gpio_do_update();	
}