/*
 * Assignment: Write a program that displays a counter on the display
 * that is incremented by one every 100 milliseconds.
 *
 * Resources:
 *   - http://blog.3d-logic.com/2015/01/10/using-a-tm1638-based-board-with-arduino/
 *
 * _Setup_
 * Vcc :        => +5v
 * GND :        -> Ground
 * DIO : data   -> (PB0)
 * CLK : clock  -> (PB1)
 * STB : strobe -> (PB2)
 *
 * Author: Roy Voetman
 *
 * _Supported commands_
	 0x8? 	(none) 	             activate board (bit a), set brightness (bits b)
	 0x44 	0xc? 0x?? 	         write value 0x?? at location 0xc? (single address mode)
	 0x40 	0xc? 0x?? 0x?? 0x??  write values 0x?? starting from location 0xc? (address auto increment mode)
	 0x42 	N/A 	             read buttons
 */

#include <avr/io.h>
#include <stdint.h>
#define F_CPU 16E6
#include <util/delay.h>

#define HIGH 0x1
#define LOW  0x0

// Pins
const uint8_t data = 0;
const uint8_t clock = 1;
const uint8_t strobe = 2;

// 7 Segment encoded digits from 0 to 9 
const uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };

// Current digit
uint16_t digit = 0;

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
        write(clock, LOW);                 // Bit valid on rising edge
        write(data, val & 1 ? HIGH : LOW); // LSB first
        val = val >> 1;
        write(clock, HIGH);
    }
}

// Send a command
void sendCommand(uint8_t value)
{
    write(strobe, LOW);
    shiftOut(value);
    write(strobe, HIGH);
}

// Display a digit
void displayDigit(uint16_t digit)
{
	// Split whole digit into single digits [0 to 9]
	uint8_t digitArray[8];
	for(uint8_t i = 0; i < 8; i++) {
		digitArray[i] = digit % 10;    // Save current single digit
		
		digit /= 10;				   // Calculate next digit
	}
	
	// Command: set auto increment mode
	sendCommand(0x40);
	
	// Command arguments
	write(strobe, LOW);
	
	// Starting at address 0
	shiftOut(0xc0);
	
    for(uint8_t i = 0; i < 8; i++)
    {
		// Display digit 
	    shiftOut(digits[digitArray[7-i]]);
		
		// Set LED
		shiftOut(0x00);
    }
	
	write(strobe, HIGH);
}

// Clear memory (LEDs and Display)
void clearMemory()
{
	// Command: set auto increment mode
	sendCommand(0x40);
	
	// Command arguments
	write(strobe, LOW);
	
	shiftOut(0xc0);                    // Starting at address 0
	for(uint8_t i = 0; i < 16; i++) {
		shiftOut(0x00);                // Clear register
	}
	
	write(strobe, HIGH);
}

int main()
{	
	// Setup
    DDRB = 0xff;        // Set PORTB as output
    sendCommand(0x89);  // Activate and set brightness to medium
	clearMemory();      // Clear memory (LEDs and Display)

	// Loop
    while (1) {
	   displayDigit(digit);
	   digit++;

       _delay_ms(100);
    }
    
    return 0;
}
