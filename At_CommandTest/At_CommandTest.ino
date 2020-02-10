//#include <SoftwareSerial.h>
// Configure software serial port
#define SIM900 Serial1
//SoftwareSerial SIM900(7, 8);


String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

String sim900IncomingMessage = "";
int sim900IncomingMessageHandler = -1;
boolean sim900IncomingMessageReady = false;


void setup() {
  // open serial to computer on 19200 as well mainly for the loop of sata to the sim900
  Serial.begin(115200);
  Serial.println("System started");

  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200);

}

void loop() {
  processSIM900();
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.print(inputString);
    SIM900.print(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  } //end if

  if (sim900IncomingMessageReady) {
    Serial.println(sim900IncomingMessage);
    sim900IncomingMessageReady = false;
    sim900IncomingMessage = "";
  } //end if 
} //end loop



void processSIM900() {
  if (SIM900.available()) {
    char readSIM900Input = SIM900.read();
    sim900IncomingMessage += readSIM900Input;
    // get last indx
    int lastIndexOfRN = sim900IncomingMessage.lastIndexOf("\r\n");
    if (sim900IncomingMessageHandler == -1 && lastIndexOfRN != -1) {
      sim900IncomingMessageHandler = lastIndexOfRN;
    } else if (sim900IncomingMessageHandler != -1 && sim900IncomingMessageHandler != lastIndexOfRN) {
      sim900IncomingMessageHandler = -1;
      sim900IncomingMessageReady = true;
    }//end if
  } //end if
} //end void



/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();

    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      inputString += '\r';
      stringComplete = true;
      
    } else {
      // add it to the inputString:
      inputString += inChar;
    } //end if
  } //end while
}
