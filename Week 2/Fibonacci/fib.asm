;
; Write in Python (or another higher programming language) a function that prints the row of Fibonacci
; "Translate" this program to assembly. You do not need to generate numbers greater than 255.
;
; Author: Roy Voetman
;
.cseg
.def a = r16
.def b = r17
.def temp = r18

; print(0)

while:
    ldi temp, a ; temp = a
    ldi a, b    ; a = b
    add temp, b ; temp = temp + b

    ; print(a)

	brcs loop

	rjmp while

loop:
	rjmp loop

