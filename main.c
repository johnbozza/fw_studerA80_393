#include "stdio.h"
#include "includes/board_io.h"

#define F_CPU 16000000

#include <util/delay.h>

int main(void)
{
    board_init();
	PORTC &= ~(1<<YPS_REC_PIN);
 
while(1)
	{
		if ( _READ_PIN(YPS_REC_PORT, YPS_REC_PIN) )
		{ 
			spi_gpio(YBI_FF3, 1); 	
			UDR0 = spi_read(0,9);
			while (!( UCSR0A & (1<<UDRE0)));
			_delay_ms(100);
		}

		//spi_gpio(YBI_DIR, 1);


		spi_gpio(YBI_FF3, 0);
		//spi_gpio(YBI_DIR, 0);
		UDR0 = spi_read(0,9);
		while (!( UCSR0A & (1<<UDRE0)));
		_delay_ms(100);
				


		
		

//		i = 0;
//		while (data[i]!=0)
//		{
//			UDR0 = data[i++];            /* Put data into buffer, sends the data */
//			while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/ 
//		}
		
	}
    return 0;
}