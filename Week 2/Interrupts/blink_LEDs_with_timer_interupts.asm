;
; Write a program that makes all LEDs blink with a 0.5 second period. Use the 16-bit timer 1 to determine the interval.
; Generate an interrupt every 0.5 seconds by counting the counter until an "output compare match" is reached.
; Indicate in your answer how you calculated the value of OCR1A.
;
; Author: Roy Voetman
;
.def tempSREG = r17
.def temp = r18

.org 0
	rjmp init
	
.org OC1Aaddr
	rjmp TIMER1_COMP_ISR                   ; Address ISR (Timer1 Output CompareMatch)
	
init:
	ldi R16, high(RAMEND)                  ; Init stack pointer
	out SPH, R16
	ldi R16, low(RAMEND)
	out SPL, R16

	ldi temp, high(31250)                  ; Init Output Compare Register
	sts OCR1AH, temp                       ; 0.5 sec = (256/16.000.000) * 31250
	ldi temp, low(31250)                   ; Write high byte before the low byte!
	sts OCR1AL, temp

	ldi temp, (1 << CS12) | (1 << WGM12)   ; Set prescaler to 256 and set timer in CTC-mode
	sts TCCR1B, temp

	ldi temp, (1 << OCIE1A)                ; Enable interrupt
	sts TIMSK1, temp 

	ser temp
	out DDRB, temp                         ; Port B is output port
	out PORTB, temp                        ; Turn off all LEDs on PORTB
	
	sei                                    ; Enable all interrupts

loop:
	rjmp loop                              ; Wait for an interrupt
		
TIMER1_COMP_ISR:          ; Interrupt service routine (ISR)
	in tempSREG, SREG     ; Save SREG
	in r16, PORTB        
	com r16               
	out PORTB, r16        
	out SREG, tempSREG    ; Restore SREG
	reti