//#include <SoftwareSerial.h>
// Configure software serial port
#define SIM900 Serial1
//SoftwareSerial SIM900(7, 8);

String sim900IncomingMessage = "";
int sim900IncomingMessageHandler = -1;
boolean sim900IncomingMessageReady = false;

void sendCommand(String command) {
  Serial.println("Sending:" + command);
  SIM900.print(command + "\r");
} //end sendCommand


void listenToComPort() {
  if (SIM900.available()) {
    char readSIM900Input = SIM900.read();
    sim900IncomingMessage += readSIM900Input;
    //Serial.print(readSIM900Input);
    //Serial.print("-");
    //Serial.println((byte)readSIM900Input);
    // get last indx
    int lastIndexOfRN = sim900IncomingMessage.lastIndexOf("\r\n");
    if (sim900IncomingMessageHandler == -1 && lastIndexOfRN != -1) {
      sim900IncomingMessageHandler = lastIndexOfRN;
    } else if (sim900IncomingMessageHandler != -1 && sim900IncomingMessageHandler != lastIndexOfRN) {
      sim900IncomingMessageHandler = -1;
      sim900IncomingMessageReady = true;
    }//end if
  } //end if
} //end listenToComPort


