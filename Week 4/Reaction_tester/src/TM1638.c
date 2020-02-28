#include <avr/io.h>
#include <stdint.h>
#include "TM1638.h"

// Read value from pin on PORTB
int read(uint8_t pin)
{
	return (PINB & _BV(pin)) ? HIGH : LOW;
}

// Write value to pin on PORTB
void write(uint8_t pin, uint8_t val)
{
    if (val == LOW) {
        PORTB &= ~(_BV(pin)); // clear bit
    } else {
        PORTB |= _BV(pin);    // set bit
    }
}

// Send value (1 byte) over the data pin
void shiftOut (uint8_t val)
{
	// Send all 8 bits (LSB first)
    for (uint8_t i = 0; i < 8; i++)  {
        write(CLOCK, LOW);                 // Bit valid on rising edge
        write(DATA, val & 1 ? HIGH : LOW); // LSB first
        val = val >> 1;
        write(CLOCK, HIGH);
    }
}

// shift in value from data
uint8_t shiftIn(void)
{
    uint8_t value = 0;
    uint8_t i;

    DDRB &= ~(_BV(DATA)); // clear bit, direction = input
    
    for (i = 0; i < 8; ++i) {
        write(CLOCK, LOW);   // bit valid on rising edge
        value = value | (read(DATA) << i); // lsb first
        write(CLOCK, HIGH);
    }
    
    DDRB |= _BV(DATA); // set bit, direction = output
    
    return value;
}

uint8_t readButtons(void)
{
    uint8_t buttons = 0;

    write(STROBE, LOW);

    shiftOut(0x42);       // Send command: read buttons

    DDRB &= ~(_BV(DATA)); // DATA pin = input

    // Read in 4 bytes containing button status
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t v = shiftIn() << i;
        buttons |= v;
    }

    DDRB |= _BV(DATA);   // DATA pin = output

    write(STROBE, HIGH);

    return buttons;
}

// Send a command
void sendCommand(uint8_t value)
{
    write(STROBE, LOW);
    shiftOut(value);
    write(STROBE, HIGH);
}

// Clear memory (LEDs and Display)
void clearDisplay()
{
	// Command: set auto increment mode
	sendCommand(0x40);
	
	// Command arguments
	write(STROBE, LOW);
	
	shiftOut(0xc0);                    // Starting at address 0
	for(uint8_t i = 0; i < 16; i++) {
		shiftOut(0x00);                // Clear register
	}
	
	write(STROBE, HIGH);
}