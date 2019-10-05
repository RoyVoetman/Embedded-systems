;
; Adjust the previous assignment so that the LED stays on even when you release the button.
; The LED must remain lit until the other button is pressed.
;
; Author: Roy Voetman
;
.cseg
.org 0
ldi r16, 0xff
out DDRD, r16

ldi r16, 0x00
out DDRB, r16

loop:
	in r16, PINB

	andi r16, 0b00000011

	cpi r16, 1
	brge set_leds
	
	rjmp loop

set_leds:
	out PORTD, r16

	rjmp loop