;
; Assignment write the C (assignment1.c) in AVR Assembly
; By storing and retrieving values from RAM assuming all integer are 16 bits
;
; Author: Roy Voetman
;
.dseg
.org SRAM_START
a: .DW 1
b: .DW 1
c: .DW 1

.cseg
ldi r16, low(3)  ; int a = 3
ldi r17, high(3)
sts a, r16
sts a+1, r17

ldi r16, low(7)  ; int b = 7
ldi r17, high(7)
sts b, r16
sts b+1, r17

do:
	lds r24,a       ; a = a - 1
	lds r25,a+1
	sbiw r25:r24, 1
	sts a, r24
	sts a+1, r25

	lds r24,b		; b = b - 1
	lds r25,b+1
	sbiw r25:r24, 1
	sts b, r24
	sts b+1, r25

	lds r24,a		; c = a
	lds r25,a+1
	sts c, r24
	sts c+1, r25

	lds r24,b		; if (b == 6)
	lds r25,b+1
    sbiw r25:r24, 6
    brne not_equal_six

	lds r24,b		; b = b - a
	lds r25,b+1
	lds r26,a
	lds r27,a+1
	sub r24,r26
    sbc r25,r27
	sts b, r24
	sts b+1, r25

	rjmp while 		; end of loop

not_equal_six:		; if (b == 3)
	lds r24,b		
	lds r25,b+1
    sbiw r25:r24, 3
    brne while

	lds r24,a       ; a = a - 1
	lds r25,a+1
	sbiw r25:r24, 1
	sts a, r24
	sts a+1, r25

while:
    lds r24,a    	; while (a > 0)
    lds r25,a+1
	ldi r16, 1
	ldi r17, 0
    cp r24, r16
    cpc r25, r17
    brge true

end:
	rjmp end	

true:
	rjmp do