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
  initializeWireframe();
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

  runGame(displayMode);

  updateToneGen();
}