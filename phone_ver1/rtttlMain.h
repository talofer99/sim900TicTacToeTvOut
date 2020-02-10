#include <Rtttl.h>

#define SPEAKER_PIN 7 //Pin you have speaker/piezo connected to (be sure to include a 100 ohm resistor) and other side of speaker to GND.

Rtttl Rtttl(SPEAKER_PIN);
//FLASH_STRING(song,"TwinkleT:d=4,o=5,b=60:16c#6,16c#6,16g#6,16g#6,16a#6,16a#6,16g#6,16p,16f#6,16f#6,16e#6,16e#6,16d#6,8c#6,16p,16g#6,16g#6,16f#6,16f#6,16e#6,16e#6,8d#6,16p,16g#6,16f#6,16f#6,16e#6,16e#6,8d#6,16p,16c#6,16c#6,16g#6,16g#6,16a#6,16g#6,16p,16f#6,16f#6,16e#6,16e#6,16d#6,16d#6,8c#6");
FLASH_STRING(song,"AxelF:d=4,o=5,b=125:32p,8g,8p,16a#.,8p,16g,16p,16g,8c6, 8g,8f,8g,8p,16d.6,8p,16g,16p,16g,8d#6,8d6,8a#,8g,8d6,8g6, 16g,16f,16p,16f,8d,8a#,2g,p,SS,16f6,8d6,8c6,8a#,g,8a#.,16g, 16p,16g,8c6,8g,8f,g,8d.6,16g,16p,16g,8d#6,8d6,8a#,8g,8d6, 8g6,16g,16f,16p,16f,8d,8a#,2g");
//FLASH_STRING(song,"Tetris:d=4,o=5,b=200:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a");
//FLASH_STRING(song,"PacMan:b=160:32b,32p,32b6,32p,32f#6,32p,32d#6,32p,32b6,32f#6,16p,16d#6,16p,32c6,32p,32c7,32p,32g6,32p,32e6,32p,32c7,32g6,16p,16e6,16p,32b,32p,32b6,32p,32f#6,32p,32d#6,32p,32b6,32f#6,16p,16d#6,16p,32d#6,32e6,32f6,32p,32f6,32f#6,32g6,32p,32g6,32g#6,32a6,32p,32b.6");



void rtttl_setup() {
  Rtttl.play(song);
}


void process_rtttl() {
  if (!Rtttl.updateMelody()) {
    Rtttl.play(song);
  } //end if 
}

void stop_rtttl() {
  Rtttl.stopTone();
}
