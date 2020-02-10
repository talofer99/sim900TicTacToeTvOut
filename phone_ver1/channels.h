#define totalChannels 4
byte channelPin[totalChannels] = {A0, A1, A2, A3};
boolean channelState[totalChannels] = {false, false, false, false};


void updateChannelState() {
  for (byte i = 0; i < totalChannels; i++) {
    digitalWrite(channelPin[i], channelState[i]);
  } //end for
} //end updateChannelState()


void setChannel(byte channelID, boolean channelNewState) {
  channelState[channelID] = channelNewState;
  updateChannelState();
} //end setChannel

void flipChannelState(byte channelID) {
  setChannel(channelID, !channelState[channelID]);
}

void setAllChanneloff() {
  for (byte i = 0; i < totalChannels; i++) {
    channelState[i] = false;
  }
  updateChannelState();
}

void setChannelOnlyOn(byte channelID) {
  for (byte i = 0; i < totalChannels; i++) {
    if (i == channelID)
      channelState[i] = true;
    else
      channelState[i] = false;
  }
  updateChannelState();
}



void setup_channels() {
  for (byte i = 0; i < totalChannels; i++) {
    pinMode(channelPin[i], OUTPUT);
  } //end for
} //end setup_channels
