#include <stdint.h>

#define HIGH 0x1
#define LOW  0x0

// Pins on port B
//-------------------------------------------------------------------

#define DATA   0
#define CLOCK  1
#define STROBE 2

// Function prototypes
//-------------------------------------------------------------------

int read(uint8_t pin);
void write(uint8_t pin, uint8_t val);
void shiftOut (uint8_t val);
uint8_t shiftIn(void);
uint8_t readButtons(void);
void sendCommand(uint8_t value);
void clearDisplay();