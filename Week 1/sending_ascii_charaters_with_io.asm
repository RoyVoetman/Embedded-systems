;
; This program defines PORTC as an input port and PORTD as an output port
; It will use the value of the lowest three pins on PORTC to let the Z pointer point to an ASCII represented digit from 0 to 7
; Subsequently this ASCII digit is send over via PORTD
; Notice: Keep in mind that you can let the Z pointer point to memory locations passed the ASCII represented digit from 0 to 7
;         Because the Z pointer is incremented inside of a loop
;
; In the assignment we needed to analyse the function of this code
; And document the action of every instruction
;
.include "m32def.inc"            ; Include configuration for the ATMega328P board

.org 0                           ; Set origin equal to 0 (i.e. save next first instruction at address 0x00)
ldi zh, high(ASCII_TABLE << 1)   ; Map the address saved in ASCII_TABLE to the equivalent Z pointer address (first 8 bits stored at ASCII_TABLE)
ldi zl, low(ASCII_TABLE << 1)    ; The multiplication by two is needed because the code segment consists of 16 bit registers
                                 ; The Atmel Data Sheets states that the Z pointer will point to either the high or low byte of a register in cseg
                                 ; To correctly map the code segment address to the correct Z pointer address you have to multiply the address by two
                                 ; All digits at ASCII_TABLE are stored as there ASCII representation shown by the surrounding quotes.
                                 ; An ASCII character is always one byte so the Z pointer will point to exactly one ASCII represented digit

ldi r16, 0x0                     ; Clear register 16
out DDRC, r16                    ; Set the Data Direction Register of Port C to configure all pins on that port as an input
ldi r16, 0xff                    ; Set register 16
out DDRD, r16                    ; Set the Data Direction Register of Port D to configure all pins on that port as an output

BEGIN:                           ; Define label "BEGIN"
    in r16, PINC                 ; Load value of IO register PINC into register 16
    andi r16, 0b00000111         ; Mask upper 5 bits (note you can't count higher than 7 with 3 bits)
    add zl, r16                  ; Add value of register 16 (now a value between 0 and 7) to the lower byte of the Z pointer
    lpm r17, z                   ; Load character from the "ASCII_TABLE" where the Z pointer is currently pointing to into register 17
    out PORTD, r17               ; Send the ASCII character that we just loaded into register 17 over PORTD
    rjmp BEGIN                   ; Set Program Counter equal to address saved in "BEGIN" (i.e. Go to BEGIN label)

.org 20                          ; Set origin equal to 20 (note this is a base 10 number not base 16)
                                 ; 20 in base 10 = 14 in base 16 (i.e. next line will begin at address 0x14 in code segment)
ASCII_TABLE:                     ; Define label "ASCII_TABLE"
    .DB '0','1','2','3','4','5','6','7' ; Save the ASCII representation of the digits 0 to 7 back to back in memory starting at address 0x14
