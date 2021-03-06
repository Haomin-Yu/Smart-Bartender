
void transmitToSystem(byte targetSystem) {
  switch (targetSystem) {
    case SYSTEM_ID_READER:
      IDReader.begin(9600);
      if(Serial.read() == 0xAA) {
        while(IDReader.available() == 0) {}
        sendToController(IDReader.read());
        // Breathalyzer.begin(9600);
      } else {consumeAllBytes();}
      IDReader.end();
      break; 
    case SYSTEM_BREATHALYZER:
      Breathalyzer.begin(9600);
      if(Serial.read() == 0xAA) {
        Breathalyzer.write(0xAA);
        delay(10500);
        byte intHigh = Breathalyzer.read();
        byte intLow  = Breathalyzer.read();
        if (intHigh == 0x00) intHigh = 0xFF;
        sendToController(intHigh, intLow);
      } else {consumeAllBytes();}
      Breathalyzer.end();
      break;
    case SYSTEM_DISPENSER:
      Dispenser.begin(9600);
      byte valve   = Serial.read();
      byte intHigh = Serial.read();
      byte intLow  = Serial.read();
      if (intHigh == 0xFF) intHigh = 0x00;
      if (valve == 0xFF) valve = 0x00;
      sendToDispenser(valve, intHigh, intLow);
      Dispenser.end();
      break;
    default:
      consumeAllBytes();
      break;
  }
}

void sendToController(byte byte1) {
  Serial.write(byte1);
}

void sendToController(byte byte1, byte byte2) {
  byte toSend[] = {byte1, byte2};
  Serial.write(toSend, sizeof(toSend));
}

void sendToDispenser(byte byte1, byte byte2, byte byte3) {
  byte toSend[] = {byte1, byte2, byte3};
  Dispenser.write(toSend, sizeof(toSend));
}
