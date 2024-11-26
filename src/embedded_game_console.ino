#include <LiquidCrystal.h>
#include <EEPROM.h>

#include "lcd_display.h"  //    ]
#include "game_manager.h" //    ] CORE
#include "pins.h"         //    ]

// ========= Games ==========
#include "game_of_life.h"
#include "wireframe_renderer.h"
#include "raycaster.h"
#include "rain_game.h"
#include "powder_sim.h"
#include "flappy_bird.h"
#include "tone_gen.h"

void setup() {
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  pinMode(modeButtonPin, INPUT_PULLUP);
  initializeGrid();
  updateDisplay();
  initializeRain();
  highScoreFlappyBird = EEPROM.read(highScoreAddressFlappyBird);
  highScoreRain = EEPROM.read(highScoreAddressRain);
  initializeToneGen();
}

void loop() {
  static bool lastModeButtonState = HIGH;
  bool modeButtonState = digitalRead(modeButtonPin);
 
  if (lastModeButtonState == HIGH && modeButtonState == LOW) {
    displayMode = (displayMode + 1) % 6;
    updateDisplay();
  }
  lastModeButtonState = modeButtonState;
 
  if (displayMode == GAME_OF_LIFE) {
      updateGrid();
      updateLCD();
      delay(30);
    }
    else if (displayMode == FLAPPY_BIRD) {
      if (gameOver == 0) {
        updateFlappyBird();
        delay(100);
        updateLCD();
        lcd.setCursor(11, 0);
        lcd.print("Score");
        lcd.setCursor(11, 1);
        lcd.print(score);
        if (cleared != 0) {
          cleared = 0;
        }
      }
      else {
        if (cleared != 1) {
          lcd.clear();
          cleared = 1;
        }
        lcd.setCursor(0, 0);
        lcd.print("Game Over");
        lcd.setCursor(0, 1);
        lcd.print("Score:");
        lcd.print(score);
 
        if (score > highScoreFlappyBird) {
          highScoreFlappyBird = score;
          EEPROM.update(highScoreAddressFlappyBird, highScoreFlappyBird);
        }
     
        lcd.setCursor(8, 1);
        lcd.print("High:");
        lcd.print(highScoreFlappyBird);
        if (!analogRead(joystickSWPin) > 0) {
          gameOver = 0;
          lcd.clear();
          cleared = 1;
          updateDisplay();
          score = 0;
        }
        delay(50);
      }
    }
    else if (displayMode == WOLFENSTEIN) {
        renderWolfenstein();
        delay(20);
    }
    else if (displayMode == RAIN) {
      if (gameOver == 0) {
        updateRain();
        updateLCD();
        delay(20);
        lcd.setCursor(9, 0);
        lcd.print("Hi:");
        lcd.print(highScoreRain);
        lcd.setCursor(6, 1);
        lcd.print("Score:");
        lcd.print(score);
        if (cleared != 0) {
          cleared = 0;
        }
      }
      else {
        if (cleared != 1) {
          lcd.clear();
          cleared = 1;
        }
        lcd.setCursor(0, 0);
        lcd.print("Game Over");
        lcd.setCursor(0, 1);
        lcd.print("Score");
        lcd.print(score);
 
        if (score > highScoreRain) {
          highScoreRain = score;
          EEPROM.update(highScoreAddressRain, highScoreRain);
        }
     
        lcd.setCursor(9, 1);
        lcd.print("Hi");
        lcd.print(highScoreRain);
        if (!analogRead(joystickSWPin) > 0) {
          gameOver = 0;
          lcd.clear();
          cleared = 1;
          updateDisplay();
          score = 0;
        }
        delay(50);
      }
    }
    else if (displayMode == POWDER) {
        updatePowder();
        updateLCD();
    }
    else {
      rotateCube();
      renderCube();
      delay(16); // for 60 fps
    }
  updateToneGen();
}