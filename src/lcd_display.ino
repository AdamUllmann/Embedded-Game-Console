#include "lcd_display.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // init the lcd

void setupLCD() {
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    lcd.clear();
}

void updateLCD() {    // draw the 4x2 character screen from pixel grid
  const int maxCustomChars = 8;
 
  for (int charY = 0; charY < LCD_HEIGHT; charY++) {
    for (int charX = 0; charX < LCD_WIDTH; charX++) {
      byte charData[8] = {0};
 
      for (int y = 0; y < CHAR_HEIGHT; y++) {
        for (int x = 0; x < CHAR_WIDTH; x++) {
          int pixelX = charX * CHAR_WIDTH + x;
          int pixelY = charY * CHAR_HEIGHT + y;
 
          if (pixelX < WIDTH && pixelY < HEIGHT) {
            if (gameGrid[pixelX][pixelY]) {         // directly draw from game grid to save memory.
                                                    // (switching display modes to game of life or other grid-dependant modes can create interesting results)
              charData[y] |= (1 << (4 - x));
            }
          }
        }
      }
 
      byte charIndex = (charY * LCD_WIDTH + charX) % maxCustomChars;
      createCustomCharacter(charIndex, charData);
      lcd.setCursor(charX, charY);
      lcd.write(charIndex);
    }
  }
}

void printText(const char* text, int row) {
  lcd.setCursor(4, row);
  lcd.print(text);
}

void createCustomCharacter(byte index, byte data[]) {
  lcd.createChar(index, data);
}
