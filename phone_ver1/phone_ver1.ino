// PHONE VERSION I
// CODE BY: TALOFER99@HOTMAIL.COM
// LINKS !!!
// https://randomnerdtutorials.com/sim900-gsm-gprs-shield-arduino/
// https://www.espruino.com/datasheets/SIM900_AT.pdf
// https://www.raviyp.com/embedded/182-sim900-sms-call-commands
// https://cdn-shop.adafruit.com/datasheets/sim800_series_at_command_manual_v1.01.pdf
// http://m2msupport.net/m2msupport/atcsq-signal-quality/

// http://arduiniana.org/libraries/flash/ (ADJUSTED !!!)
// https://bitbucket.org/teckel12/arduino-timer-free-tone/downloads/
// https://github.com/spicajames/Rtttl

// AT+COLP=1 --> set tp get COLP on answer
// AT+CLIP=1 --> set call id on
// AT+CREG=1 --> show GEG state out put
// AT+COPS=0,2 --> NO SURE :)
// AT+CPBF="" search

#include "rtttlMain.h";
#include "screen.h"
#include "keyPad.h"
#include "channels.h"
#include "commands.h"
#define INTCALLCODE 972
String callerID;
String reply;
char dialedNumber[16];
byte dialedNumberCursor = 0;
String intFrmtNumber;
byte systemState;
unsigned long systemStateStartMillis;


String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // open serial to computer on 19200 as well mainly for the loop of sata to the sim900
  Serial.begin(115200);
  //Serial.println("System started");

  // start channels
  setup_channels();

  // start the screen
  screen_setup();

  //setup rtttl
  rtttl_setup();


  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200);
  Serial2.begin(115200); // used for the tic-tac-toe communictaion
  // delay to let the hardare wakeup
  delay(2000);

  // set system state to ZERO
  setSystemState(0);
} //end setup

void loop() {

  // print the string when a newline arrives:
  if (stringComplete) {
    SIM900.print(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  } //end if

  if (Serial.available()) {
    char readS = Serial.read();
    if (readS == '1') {
      sendCommand("AT+CPBS=\"SM\""); // MC
    } else if (readS == '2') {
      sendCommand("AT+CPBR=?");
    } else if (readS == '3') {
      sendCommand("AT+CPBR=1");
    } else if (readS == '4') {
      sendCommand("AT+CPBS=\"DC\"");
    } else if (readS == '5') {
      sendCommand("AT+COLP?");
    } else if (readS == '6') {
      sendCommand("AT+COLP=1"); // SHOW CALL ID ON ANSWER CALL ... LATE RESPONSE TO ATD !!!
    } else if (readS == '7') {
      sendCommand("AT+CREG=1");
    } else if (readS == '8') {
      sendCommand("AT+COPS=0,2");
    } else if (readS == '9') {
      sendCommand("AT+CLVL?"); // SOUND LEVEL
    } else if (readS == 'm') {
      sendCommand("AT+CMUR?"); // MUTE STAE

    } else if (readS == 's') {
      sendCommand("AT+CSQ"); // SIGNAL STRENGTH !!!!!!
    } else if (readS == 'S') {
      sendCommand("AT+CSQ=?"); // MUTE STAE

    } else if (readS == 'a') {
      sendCommand("AT+DDET?"); // DTFM DETECTION CONTROL

    } else if (readS == 'A') {
      sendCommand("AT+DDET=1"); // DTFM DETECTION CONTROL

    } else if (readS == 'b') {
      sendCommand("AT+ATS4?"); // DETEMINE THE END AND START OF MESSAGES

    } else if (readS == 'B') {
      sendCommand("AT+ATS4=?"); // DETEMINE THE END AND START OF MESSAGES

    } //end if
  } //end if


  // process logic
  processBySystemState();

  // listen to the port for incoming data
  listenToComPort();

  // handle returned values
  if (sim900IncomingMessageReady) {
    // run the process
    processsim900IncomingMessage();

    // clear flag and string
    sim900IncomingMessageReady = false;
    sim900IncomingMessage = "";
  } //end if
} //end loop


//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//
//    // if the incoming character is a newline, set a flag so the main loop can
//    // do something about it:
//    if (inChar == '\n') {
//      inputString += '\r';
//      stringComplete = true;
//    } else {
//      // add it to the inputString:
//      inputString += inChar;
//    } //end if
//  } //end while
//}



// Display caller id
void displayCallerID() {
  if (callerID.length()) {
    printSecondLine(callerID);
  } //end if
} //end displayCallerID

// dial number
void dialNumer() {
  // clear
  intFrmtNumber = "";
  //define starting point
  byte startingPoint = 0;
  // if number start with ZERO we remove it
  if (dialedNumber[0] == '0') {
    intFrmtNumber = INTCALLCODE;
    startingPoint = 1;
  } //end if

  for (byte i = startingPoint; i < dialedNumberCursor; i++) {
    intFrmtNumber += dialedNumber[i];
  } //end for
  // MAKE CALL
  sendCommand("ATD + +" + intFrmtNumber + ";");
} //end dial number

// add digit to dialed number
void addDigitToDialNumber(char getKeyPadInput) {
  printSecondLineCol(getKeyPadInput, dialedNumberCursor);
  dialedNumber[dialedNumberCursor] = getKeyPadInput;
  dialedNumberCursor++;
} //end addDigitToDialNumber




// PROCESS RETURN DATA
void processsim900IncomingMessage() {
  Serial.print(F("sim900IncomingMessage - "));
  Serial.println(sim900IncomingMessage);

  // ATH -- > Hang Up Call
  if (sim900IncomingMessage.indexOf("ATH") >= 0) {
    setSystemState(2);
  } //end if (before else if)

  // ATD -- > MAKE CALL
  else if (sim900IncomingMessage.indexOf("ATD") >= 0) {
    //set in DAILING
    setSystemState(5);
  } //end if (before else if)

  // NO CARRIER -- > hang up (other side on RING)
  else if (sim900IncomingMessage.indexOf("NO CARRIER") >= 0) {
    setSystemState(2);
  } //end if (before else if)

  // ATA -- > Anser Call
  else if (sim900IncomingMessage.indexOf("ATA") >= 0) {
    setSystemState(4);
  } //end if (before else if)


  // BUSY -- > HANG ON OTHER SIDE
  else if (sim900IncomingMessage.indexOf("BUSY") >= 0) {
    //set to phone ready
    printSecondLine("BUSY........");
    delay(1000);
    setSystemState(2);
  } //end if (before else if)

  // COLP -- > CALL ANSWERED BY DIAL
  else if (sim900IncomingMessage.indexOf("COLP") >= 0) {
    setSystemState(4); //set in call
  } //end if (before else if)

  // CLIP incoming caller ID
  else if (sim900IncomingMessage.indexOf("+CLIP") > 0) {
    // clear the caller ID
    callerID = "";
    // look for clip location
    int clipPoistion = sim900IncomingMessage.indexOf("+CLIP:");
    // if we found clip as well
    if (clipPoistion > 0) {
      //adject the postion to trim the number
      clipPoistion += 8;
      // look for nd dbl-qoute
      int secDblQuotePosition = sim900IncomingMessage.indexOf("\"", clipPoistion);
      if (secDblQuotePosition > 0) {
        callerID = sim900IncomingMessage.substring(clipPoistion, secDblQuotePosition);
        displayCallerID();
      } //end if
    }//end if
  } //end if (before else if)

  // incoming call RING
  else if (sim900IncomingMessage.indexOf("RING") > 0) {
    //set system state to call
    if (systemState != 3 && (systemState != 4 && systemStateStartMillis + 500 < millis())) {
      // set system to 3 - incoming call
      setSystemState(3);
    } //end if
  } //end if (before else if)

  // AT COMMAND
  else if (sim900IncomingMessage.indexOf("AT\r\r\n") >= 0) {
    // set to system state 1 - check line
    setSystemState(1);
  } //end if (before else if)

  // CCALR: command
  else if (sim900IncomingMessage.indexOf("CCALR:") >= 0) {
    char reply = sim900IncomingMessage[sim900IncomingMessage.indexOf("CCALR:") + 7];
    if (reply == '1') {
      setSystemState(2);
    } else {
      printSecondLine("NOT READY ...");
      delay(3000);
      // retry
      setSystemState(1);
    } //end if
  } //end if (before else if)

  // NORMAL POWER DOWN -- > SIM900 IS SHUTTING DOWN
  else if (sim900IncomingMessage.indexOf("NORMAL POWER DOWN") >= 0) {
    setSystemState(0); //set to check module
  } //end if (before else if)

  // NORMAL POWER DOWN -- > SIM900 IS SHUTTING DOWN
  else if (sim900IncomingMessage.indexOf("NO DIALTONE") >= 0) {
    printSecondLine("NO DIALTONE...");
    delay(1000);
    setSystemState(1); //check line is ready
  } //end if (before else if)

  // DTMF
  else if (sim900IncomingMessage.indexOf("DTMF") >= 0) {
    char reply = sim900IncomingMessage[sim900IncomingMessage.indexOf("DTMF") + 5];
    // echo out
    Serial.print(reply);
    Serial2.print(reply);
    // 1 = char(49)
    //if (reply >= 49 && reply < 49 + totalChannels) {
    //flipChannelState(reply-49);
    //
    //} //end if
  } //end if (before else if)

  // CPBR: 1 -- > GET FIRST RECORD OF SOMTHING
  else if (sim900IncomingMessage.indexOf("CPBR: 1,\"+") >= 0) {
    // calc postion of number
    byte startPos = sim900IncomingMessage.indexOf("CPBR: 1,\"+") + 10;
    byte secDblQuotePosition = sim900IncomingMessage.indexOf("\"", startPos);
    //get number
    String phoneNumer = sim900IncomingMessage.substring(startPos, secDblQuotePosition);

    // add to dial
    for (byte i = 0; i < phoneNumer.length(); i++) {
      addDigitToDialNumber(phoneNumer[i]);
    } //end for


  } else {
    //Serial.print(F("UNMANGED RETURN DATA:"));
    //Serial.println(sim900IncomingMessage);
  } //end if

} //end processsim900IncomingMessage


// PROCESS BY STATE
void processBySystemState() {
  switch (systemState) {
    case 0:
      // if its been more then 1000 millis
      if (systemStateStartMillis + 1000 < millis()) {
        // show fail message
        printSecondLine("FAIL.....");
        // delay
        delay(1000);
        // try again
        setSystemState(0);
      } //end if
      break;

    case 1:
      // if its been more then 5000 millis
      if (systemStateStartMillis + 5000 < millis()) {
        // show fail message
        printSecondLine("FAIL.....");
        // delay
        delay(1000);
        // try again
        setSystemState(1);
      } //end if
      break;

    case 2: {
        // IDLE STATE
        char getKeyPadInput = getKeyPad();

        if (getKeyPadInput == '*') {
          if (dialedNumberCursor == 0) {
            // lets get last dialed number
            sendCommand("AT+CPBS=\"DC\"");
            delay(100);
            sendCommand("AT+CPBR=1");
          } else if (dialedNumberCursor > 0) {
            dialNumer();
          } //end if

        } else if (getKeyPadInput == '#' ) {
          // DELETE LAST DIGIT
          if (dialedNumberCursor > 0) {
            dialedNumberCursor--;
            printSecondLineCol(' ', dialedNumberCursor);
          }
        } else if (getKeyPadInput) {
          //PRINT DIGIT AND KEEP FOR MAKING PHONE CALL
          addDigitToDialNumber(getKeyPadInput);
        } //end if
      }  //end case
      break;
    case 3:
      {
        // INCOMING CALL (RING)
        // process song
        process_rtttl();
        //get keypad input
        char getKeyPadInput = getKeyPad();
        if (getKeyPadInput == '*') {
          Serial.println("Answering .... ");
          // send answer command
          sendCommand("ATA");
        } else if (getKeyPadInput == '#') {
          // send hangup  command
          sendCommand("ATH");
        }//end if
      } //end case
      break;
    case 4:
      // IN CALL
      if (getKeyPad() == '#') {
        // send hangup command
        sendCommand("ATH");
      } //end if
      break;
    case 5:
      // IN DAILING
      if (getKeyPad() == '#') {
        // send hangup command
        sendCommand("ATH");
      } //end if
      break;
    default:
      Serial.println(F("UN-HANDELDED SYSTEM STATE ON PROCESS"));
      break;

  } //end switch
} //end processBySystemState




// Set new state for system
// available states
// 0 - CHECK FOR MODULE
// 1 - CHECK FOR LINE
// 2 - READY STATE (call and sms)
// 3 - INCUMING CALL (RING)
// 4 - IN CALL
// 5 - DAILING NUMBER
void setSystemState(byte newState) {
  switch (newState) {
    case 0:
      printNew("HARDWARE CHECK..");
      sendCommand("AT");
      break;
    case 1:
      printNew("LINE CHECK..");
      sendCommand("AT+CCALR?");
      break;
    case 2:
      memset(dialedNumber, 0, 16);
      dialedNumberCursor = 0;
      // in case the rtttl is playing
      stop_rtttl();
      printNew("PHONE READY");
      break;
    case 3:
      printNew("INCOMING CALL");
      break;
    case 4:
      // in case the rtttl is playing
      stop_rtttl();
      //adjuest display
      printNew("IN CALL");
      displayCallerID();
      break;
    case 5:
      printNew("DIALING");
      // for diplay later
      callerID = intFrmtNumber;
      // print number
      printSecondLine(intFrmtNumber);
      break;
    default:
      Serial.println(F("UN-HANDELED NEW SYSTEM STATE"));
      break;
  } //end switch

  systemState = newState;
  systemStateStartMillis = millis();

} //end setSystemState



