;
; Assignment write the C (assignment1.c) in AVR Assembly
; Using only register
;
; Author: Roy Voetman
;
.cseg
.def a = r16
.def b = r17
.def c = r18

ldi a, 3
ldi b, 7

do:
	dec a
	dec b
	mov c, a

	cpi b, 6
	breq equal_six

	cpi b, 3
	breq equal_three

	rjmp while

equal_six:
	sub b, a
	rjmp while

equal_three:
    dec a

while:
	cpi a, 0
	breq end
	brlt end

	rjmp do

end:
	rjmp end