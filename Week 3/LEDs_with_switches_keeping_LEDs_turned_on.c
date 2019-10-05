//
// Take your code from the last weeks assignment to switch two LEDs with two switches
// Translate that AVR Asm code into C code
//
// Author: Roy Voetman
//
#include <avr/io.h>

int main(void) {
	DDRB = 0x00;
	DDRD = 0xff;
	
	while(1) {
		int temp = PINB & 0b00000011;
		
		if(temp > 0) {
			PORTD = temp;
		}
	}
}