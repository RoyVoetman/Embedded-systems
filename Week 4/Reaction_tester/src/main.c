/**
 * Setup TM1638 with Atmega328P
 * -----------------------------------
 * Vcc : +5v
 * GND : ground
 * DIO : data   (board pin 8)   (PB0)
 * CLK : clock  (board pin 9)   (PB1)
 * STB : strobe (board pin 10)  (PB2)
 */

#include <avr/io.h>
#include <stdint.h>

#include "AVR_TTC_scheduler.h"
#include "TM1638.h"

// 7 Segment encoded digits from 0 to 9 
const uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };

// Current digit
volatile uint32_t currentDigit = 0;

volatile unsigned char updateDisplayTaskId;

// Display a digit
void updateDisplay()
{
   uint32_t digit = currentDigit;

	// Split whole digit into single digits [0 to 9]
	uint8_t digitArray[8];
	for(uint8_t i = 0; i < 8; i++) {
		digitArray[i] = digit % 10;    // Save current single digit
		
		digit /= 10;				       // Calculate next digit
	}
	
	// Command: set auto increment mode
	sendCommand(0x40);
	
	// Command arguments
	write(STROBE, LOW);
	
	// Starting at address 0
	shiftOut(0xc0);
	
   for(uint8_t i = 0; i < 8; i++) {
		// Display digit 
	   shiftOut(digits[digitArray[7-i]]);
		
		// Set LED
		shiftOut(0x00);
   }
	
	write(STROBE, HIGH);

   currentDigit++;
}

void checkButtons() 
{
   uint8_t buttons = readButtons();

   uint8_t swA = 1;
   uint8_t swB = 5;

   uint8_t mskSwA = 1 << swA;
   uint8_t mskSwB = 1 << swB;

   if(buttons & mskSwA) {
      currentDigit = 0;
      updateDisplayTaskId = SCH_Add_Task(updateDisplay,0,1);
   }

   if(buttons & mskSwB) {
      currentDigit = 0;
      SCH_Delete_Task(updateDisplayTaskId);
   }
}

int main()
{	
	// Setup display
   DDRB = 0xff;         // Set PORTB as output
   sendCommand(0x89);   // Activate and set brightness to medium
	clearDisplay();      // Clear memory (LEDs and Display)

   // Setup schedular
   SCH_Init_T1();       // init de timer verwijder alle taken
   
   SCH_Add_Task(checkButtons,0,1);
   
   SCH_Start();         // start de scheduler

   // Loop
   while (1) {
      SCH_Dispatch_Tasks();
   }
    
   return 0;
}