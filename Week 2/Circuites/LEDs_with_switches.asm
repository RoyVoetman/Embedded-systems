;
; Create a program that lights an LED when a button is pressed.
; When button 0 is pressed, LED 0 must light up.
; When button 1 is pressed, LED 1 must light up.
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
	out PORTD, r16

	rjmp loop
	