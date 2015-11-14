#include <avr/io.h>
#include <util/delay.h>

void main()
{
	// D13 as output (D13 is at PB5 on Pro Mini and NANO)
	DDRB = (1 << PB5);

	while(1) {
		// LED on: D13 high
		PORTB |= (1 << PB5);
		// wait 500 ms
		_delay_ms(500);

		// LED off: D13 low
		PORTB &= ~(1 << PB5);
		// wait 500 ms
		_delay_ms(500);
	}
}
