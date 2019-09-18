#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void screen_setup() {
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("WAKING UP..");
}


void printNew(String message) {
  lcd.clear();
  lcd.print(message);
}



void printSecondLine(String message) {
  lcd.setCursor(0, 1);
  lcd.print(message);
}

void printSecondLineCol(char message, byte col) {
  lcd.setCursor(col, 1);
  lcd.print(message);
}
