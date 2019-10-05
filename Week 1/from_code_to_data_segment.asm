;
; This code will copy the string "hello world !"
; From Code Segment (Flash memory) to Data Segment (SRAM) with the use of the X en Z pointers
;
; In the assignment we needed to analyse the function of this code
; And document the action of every instruction
;
.dseg                           ; Start data segment (volatile)
.org SRAM_START                 ; Start allocating at the beginning of Static RAM
dest: .byte 20                  ; Allocate 20 bytes and save the address of the first byte in "dest"

.cseg                           ; Start code segment (non-volatile)
    rjmp start                  ; Set Program Counter equal to address saved in "start" (i.e. Go to start label)

src: .db "hello world !"        ; Define character array "hello world !" in code segment and save the array pointer in "src"
.equ length = 13                ; Define constant 13, length of character array
.def temp = r0                  ; Alias register 0 with "temp"
.def counter = r17              ; Alias register 17 with "counter"

start:                          ; Define label "start"
    ldi zh, high(2*src)         ; Let the Z pointer point to the first character in the "src" array
    ldi zl, low(2*src)          ; The multiplication by two is needed because the code segment consists of 16 bit registers
                                ; The Atmel Data Sheets states that the Z pointer will point to either the high or low byte of a register  in cseg
                                ; To correctly map the code segment address to the correct Z pointer address you have to multiply the address by two
                                ; All characters of the string "src" are stored as there ASCII representation shown by the surrounding double quotes.
                                ; An ASCII character is always one byte so the Z pointer will point to exactly one character in the string at the time
    
    ldi xh, high(dest)          ; Let the X pointer point to the first byte allocated referenced by "dest"
    ldi xl, low(dest)           ; The addresses are 16 bit so we need a high and a low byte to store the full address
                                ; This is an address in data segment, which has 8 bit register so we don't need the multiply by two method
    
    clr counter                 ; Clear counter (register 17) in case it had some garbage data.

loop:                           ; Define label loop
    lpm                         ; The Instruction Set states the if LPM is used without operands
                                ; it will store the value where the Z pointer is pointing to into register 0 (aliased as "temp")
    inc z                       ; Increment the Z pointer to point at the next character in the "src" array
    lst x+, temp                ; Store the value of temp (register 0) at the location where the X pointer is pointing
                                ; Post increment x (x+) so the X pointer is incremented after the store instruction
    inc counter                 ; Increment the value in counter (register 17)
    cpi counter, length         ; Compare the value in counter (register 17) with the constant length.
    brlt loop                   ; Set Program Counter equal to address saved in "loop", if the previous compare resulted in "Less Then".
                                ; (i.e. Go back to "loop" if counter is less then length)
                                ; This is checked by looking at the SREG (Status register) which is manipulated by, but not limited to, Compare instructions

end:                            ; Define label end
    rjmp end                    ; Infinite loop to in order to prevent the executing of previously flashed code.