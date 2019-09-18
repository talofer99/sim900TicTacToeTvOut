#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {10, 11, 3, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 5, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


char getKeyPad(){
  char key = keypad.getKey();
  if (key){
    return key; 
  } else {
    return 0;
  }
}
