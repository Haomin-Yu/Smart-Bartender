
/**
 * Unit test for the breathalyzer system
 */

/* System Constants */
static const byte BREATHALYZER_SYSTEM_ENABLE = 0xAA;
static const unsigned int EXPECTED_BYTES;
static const unsigned int WAIT_TIME = 2050;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.write(BREATHALYZER_SYSTEM_ENABLE);
  unsigned long startTime = millis();
  while(Serial.available() < EXPECTED_BYTES && millis() - startTime < WAIT_TIME) {}
  if(Serial.available() < EXPECTED_BYTES) { /* Timed out */
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    Serial.print("Time took: "); Serial.println(millis() - startTime);
    int returnedMeasurement = Serial.read() << 8 | Serial.read();
    Serial.print("Returned Measurement: "); Serial.println(returnedMeasurement);
  }
  while(true) {}
}

void loop() {}
