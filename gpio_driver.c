#include "includes/gpio_driver.h"
#include "includes/board_def.h"
#include <util/delay.h>
#include <stdio.h>

//#define EXT_INT

volatile Circuit *Circ;

void gpio_init(uint8_t eeprom_register)
{
	cli();
	
	circuit_init();
	Circ = circuit_get_circuit();
	
	// 7 6 5 4 3 2 1 0
	// X X 1 0 1 1 0 0
	DDRB = 0x2C;	PORTB = 0xff;
	// 7 6 5 4 3 2 1 0
	// X X 0 0 0 0 1 1
	DDRC |= 0x03;	PORTC = 0xff;
	// 7 6 5 4 3 2 1 0
	// 0 0 0 0 0 0 1 0
	DDRD = 0x02;	PORTD = 0xff;

	spi_init();
	
	// UART SETUP
	unsigned int ubrr = BAUD_RATE_115200_BPS;
	UBRR0H = ( ubrr >> 8 );
	UBRR0L = ( ubrr );
	UCSR0C = 0x06;       /* Set frame format: 8data, 1stop bit  */
	UCSR0B = ( 1 << TXEN0 ) | ( 1 << RXEN0 ) | ( 1 << RXCIE0 ); // TX, RX and RX INT
	

	PCICR  |= (1 << PCIE0) | (1 << PCIE1)  | (1 << PCIE2);		
	PCMSK0 |= ( 1 << YBI_FAD_PIN ) | ( 1 << YBI_LOW_PIN );		
	PCMSK1 |= ( 1 << YPS_REC_PIN ) | ( 1 << YPS_STOP_PIN ) | ( 1 << YPS_REPR_PIN ) | ( 1 << YPS_FORW_PIN );
	PCMSK2 |= ( 1 << YPS_REW_PIN ) | ( 1 << YPS_CUT_PIN )  | ( 1 << YBI_END_PIN )  | ( 1 << YBI_MOVE_PIN );	

	// TIMER SETUP
	TCNT1 = TIMER_1S;
	TCCR1B = ( 1 << CS10 ) | ( 1 << CS12 ); // 16M / 1024 = 15625Hz
	TCCR1A = 0x00; // overflow
	TIMSK1 = ( 1 << TOIE1 ); // overflow int
	
	// set the CLK pin to LOW
	spi_gpio(YBI_CLK, 0);
	
	Circ->signal_output[M4_1_OUT] = 0;
	Circ->signal_output[M4_2_OUT] = 0;
	Circ->signal_output[M4_3_OUT] = 0;
	Circ->signal_output[M4_4_OUT] = 0;
	
		// ENC B interrupt
	EICRA |= ( 1 << ISC10 );	// INT1 LEVEL CHANGE
		
	EIMSK |= ( 1 << INT1 );

	gpio_do_update();
	
	sei();
}

void gpio_do_uart_rx(uint8_t ch)
{
	char str[40] = {0};
	
	if ( ( ch == 0x30 ) || ( ch == 0x31 ) )
	{
		Circ->signal_input[MOVE_IN] = ch - 0x30;
		sprintf(str, "Direction = %d\n", Circ->signal_input[MOVE_IN]);
		gpio_send_str(str);
	}
	
	if ( ch == 0x32 )
	{
		sprintf(str, "Output = ");
		gpio_send_str(str);
		
		for ( SignalOutputs i = 1; i < 6; i++)
		{		
			sprintf(str, "%d ", Circ->signal_output[i]);
			gpio_send_str(str);
		}
		
		sprintf(str, "\n");
		gpio_send_str(str);
		
	}
	
	if ( ch == 0x33 )
	{
		for ( SignalInputs i = 0; i < NUM_OF_INPUTS; i++)
		{
			//char *inp[] = { "FAD_IN", "REC_IN", "END_IN", "CUT_IN", "REW_IN", "STOP_IN", "MOVE_IN", "FORW_IN", "REPR_IN", "LOW_IN" };
			sprintf(str, "%d ", Circ->signal_input[i]);
			gpio_send_str(str);
		}
		
		sprintf(str, "\n");
		gpio_send_str(str);
	}
	
	if ( ch == 0x34 )
	{
		sprintf(str, "FlipFlops = ");
		gpio_send_str(str);
		
		for ( FlipFlopsOutput i = 0; i < NUM_OF_FLIPFLOPS; i++)
		{
			sprintf(str, "%d", Circ->ffs[i].set_port);
			gpio_send_str(str);
			sprintf(str, "%d", Circ->ffs[i].reset_port);
			gpio_send_str(str);			
			sprintf(str, "%d   ", Circ->ffs[i].set_state);
			gpio_send_str(str);
		}
		
		sprintf(str, "\n");
		gpio_send_str(str);
	}	
}

void gpio_send_str(char * str)
{
	for (int i = 0; i < 40; i++)
	{
		if ( str[i] == 0 ) break;
		UDR0 = (char)str[i];
		while (!( UCSR0A & (1<<UDRE0)));
	}
}

	

// 	for (int i = 0; i < NUM_OF_OUTPUTS; i ++)
// 	{
// 		UDR0 = Circ->signal_output[i];
// 		while (!( UCSR0A & (1<<UDRE0)));
// 	}	

void gpio_do_timer()
{
	static uint8_t mcu_led_period = 0;

cli();

	//gpio_do_update();
	
	if ( ++mcu_led_period > 16)
	{
		PORTB ^= ( 1 << AUX_LED_PIN );
		mcu_led_period = 0;
		
		gpio_do_uart_rx(2);
	}
	//gpio_do_update();
	TCNT1 = TIMER_1S;
	
	gpio_do_update();
sei();

}

void gpio_do_update()
{	
	circuit_update();
	
	Circ->signal_input[FAD_IN]	= _READ_PIN(YBI_FAD_PORT,	YBI_FAD_PIN);
	Circ->signal_input[REC_IN]	= _READ_PIN(YPS_REC_PORT,	YPS_REC_PIN);
	Circ->signal_input[END_IN]	= _READ_PIN(YBI_END_PORT,	YBI_END_PIN);
	Circ->signal_input[CUT_IN]	= _READ_PIN(YPS_CUT_PORT,	YPS_CUT_PIN);
	Circ->signal_input[REW_IN]	= _READ_PIN(YPS_REW_PORT,	YPS_REW_PIN);
	Circ->signal_input[STOP_IN]	= _READ_PIN(YPS_STOP_PORT,	YPS_STOP_PIN);
	//Circ->signal_input[MOVE_IN]	= _READ_PIN(YBI_MOVE_PORT,	YBI_MOVE_PIN);
	Circ->signal_input[FORW_IN]	= _READ_PIN(YPS_FORW_PORT,	YPS_FORW_PIN);
	Circ->signal_input[REPR_IN]	= _READ_PIN(YPS_REPR_PORT,	YPS_REPR_PIN);
	Circ->signal_input[LOW_IN]	= _READ_PIN(YBI_LOW_PORT,	YBI_LOW_PIN);
	
	circuit_update();

	// values setup by logic circuit
	spi_gpio(YBI_MOV2,	Circ->signal_output[MOVE2_OUT]);
	spi_gpio(YBI_FF0,	Circ->signal_output[FF0_OUT]);
	spi_gpio(YBI_FF1,	Circ->signal_output[FF1_OUT]);
	spi_gpio(YBI_FF2,	Circ->signal_output[FF2_OUT]);
	spi_gpio(YBI_FF3,	Circ->signal_output[FF3_OUT]);
	spi_gpio(YBI_FF4,	Circ->signal_output[FF4_OUT]);
	spi_gpio(YBI_CUT,	Circ->signal_output[CUT_OUT]);
	spi_gpio(B_REC,		Circ->signal_output[REC_OUT]);
	spi_gpio(YBI_STOP,	0);
	
	spi_gpio(M4_1,		Circ->signal_output[M4_1_OUT]);
	spi_gpio(M4_2,		Circ->signal_output[M4_2_OUT]);
	spi_gpio(M4_3,		Circ->signal_output[M4_3_OUT]);
	spi_gpio(M4_4,		Circ->signal_output[M4_4_OUT]);
	spi_gpio(YBI_DIR,	Circ->signal_output[DIR_OUT]);
// 	spi_gpio(YBI_CLK,	Circ->signal_output[CLK_OUT]);
 	
}

void gpio_do_encoder()
{

}

void gpio_trigger_clk()
{

	static volatile bool ping_pong = true;
	cli();
	//spi_gpio(YBI_CLK, ping_pong);
	// trigger clk to turn off after 40uS
	
	if ( Circ->signal_output[DIR_OUT] == REWARD )
	{
		if ( ping_pong )
		{
			Circ->signal_output[M4_1_OUT] = !Circ->signal_output[M4_3_OUT];
			Circ->signal_output[M4_2_OUT] = !Circ->signal_output[M4_1_OUT];
		}
		else
		{
			Circ->signal_output[M4_3_OUT] = Circ->signal_output[M4_1_OUT];
			Circ->signal_output[M4_4_OUT] = !Circ->signal_output[M4_3_OUT];			
		}
	}
	else
	{
		if ( ping_pong )
		{
			Circ->signal_output[M4_1_OUT] = Circ->signal_output[M4_3_OUT];
			Circ->signal_output[M4_2_OUT] = !Circ->signal_output[M4_1_OUT];

		}
		else
		{
			Circ->signal_output[M4_3_OUT] = !Circ->signal_output[M4_1_OUT];
			Circ->signal_output[M4_4_OUT] = !Circ->signal_output[M4_3_OUT];
		}
	}
	
	gpio_do_update();	
	ping_pong = !ping_pong;	

	sei();
}

/*INTERRUPTS*/
ISR (INT0_vect)
{
}

/*


DIR
	HIGH, if QPDIR1 is the same level as QPDIR2 after QPDIR2 level change
	LOW, if QPDIR1 is inverted in respect to QPDIR2

CLK
	for DIR is HIGH
	if YBI-LOW is HIGH
	Pulse at QPDIR2 Raising Edge
	if YBI-LOW is LOW
	Pulse at QPDIR2 Level Change
	
	for DIR is LOW
	if YBI-LOW is HIGH
	Pulse at QPDIR2 Falling Edge
	if YBI-LOW is LOW
	Pulse at QPDIR2 Level Change
	
M4
	DIR is LOW,  CLK affects M4-1 first follows QPDIR2 (first pulse)
	DIR is HIGH, CLK affects M4-3 first, inverted in respect to QPDIR2 (first pulse)
*/

ISR (TIMER0_OVF_vect)
{
	//spi_gpio(YBI_CLK, 0);
}
ISR (INT1_vect)
{
	volatile bool qp_dir_1, qp_dir_2;
	
	if ( Circ->signal_input[END_IN] )
	{
		qp_dir_1 = _READ_PIN(QP_DIR1_PORT, QP_DIR1_PIN);
		qp_dir_2 = _READ_PIN(QP_DIR2_PORT, QP_DIR2_PIN);

		if ( ( qp_dir_1 == qp_dir_2 ) )
			Circ->signal_output[DIR_OUT] = FORWARD;
		else
			Circ->signal_output[DIR_OUT] = REWARD;
		
		if ( ! Circ->signal_input[LOW_IN] )
		{
			gpio_trigger_clk();
		}
		else
		{
			if ( ( Circ->signal_output[DIR_OUT] == FORWARD ) && ( qp_dir_2 ) )
				gpio_trigger_clk();
			if ( ( Circ->signal_output[DIR_OUT] == REWARD ) && (!qp_dir_2 ) )
				gpio_trigger_clk();
		}
	}
	gpio_do_update();
	
}
ISR (TIMER1_OVF_vect)
{
	gpio_do_timer();
}
ISR (USART_RX_vect)
{
	cli();
		uint8_t ch = UDR0;		
		gpio_do_uart_rx(ch);
	sei();
}
#define UART_CMD 0x34
ISR (PCINT0_vect)
{
	gpio_do_update();
	gpio_do_uart_rx(UART_CMD);
}
ISR (PCINT1_vect)
{
	gpio_do_update();
	gpio_do_uart_rx(UART_CMD);
}
ISR (PCINT2_vect)
{
	gpio_do_update();
	gpio_do_uart_rx(UART_CMD);
}