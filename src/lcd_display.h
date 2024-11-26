#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

// BASIC 4X2 CHARACTER SCREEN STUFF
constexpr int CHAR_WIDTH = 5;   // number of x pixels in a character
constexpr int CHAR_HEIGHT = 8;  // number of y pixels in a character
constexpr int LCD_WIDTH = 4;    // number of characters wide
constexpr int LCD_HEIGHT = 2;   // number of characters tall
constexpr int WIDTH = CHAR_WIDTH * LCD_WIDTH;       // number of total x pixels in 4x2 character screen
constexpr int HEIGHT = CHAR_HEIGHT * LCD_HEIGHT;      // number of total y pixels in 4x2 character screen
bool cleared = 0; // flag to keep track of whether or not the lcd is cleared.

void setupLCD();
void updateLCD(byte gameGrid[][CHAR_HEIGHT * LCD_HEIGHT]);
void printText(const char *text, int row);
void createCustomCharacter(byte index, byte data[]);

#endif
