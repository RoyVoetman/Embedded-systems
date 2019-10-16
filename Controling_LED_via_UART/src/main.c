// B) 1 / 19200 * 8 = 0,0004 sec
#include <avr/io.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>

#define F_CPU 16E6
#include <util/delay.h>

// output on USB = PD1 = board pin 1
// datasheet p.190; F_OSC = 16 MHz & baud rate = 19.200
#define UBBRVAL 51

void uart_init() 
{
    // set the baud rate
    UBRR0H = 0;
    UBRR0L = UBBRVAL;
    
    // disable U2X mode
    UCSR0A = 0;
    
    // enable transmitter and receiver
    UCSR0B = _BV(TXEN0) | (1 << RXEN0);
    
    // set frame format : asynchronous, 8 data bits, 1 stop bit, no parity
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

uint8_t usart_read(){
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */

    return UDR0;
}

void usart_trasmit(uint8_t data)
{
    // wait for an empty transmit buffer
    // UDRE is set when the transmit buffer is empty
    loop_until_bit_is_set(UCSR0A,UDRE0);
    // send the data
    UDR0 = data;
}

int main(void) {
    DDRB = 0xff;

    uart_init();
    _delay_ms(1000);

    while(1) { 
        uint8_t temp = usart_read();
        usart_trasmit(temp); 

        if(temp == 0x31) {
            PORTB = 0xff;
        } else {
            PORTB = 0x00;
        }
    }
}