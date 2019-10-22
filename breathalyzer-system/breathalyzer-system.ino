
/**
 * Communicates with the breathalyzer and holds the
 * data until requested
 * (Sensor MAXIMUM 0.05 - 500 ppm)
 */
#include <ArduinoSort.h>
#include "SystemConstants.h"

void setup() {
  Serial.begin(BREATHALYZER_BAUD_RATE);
  pinMode(BREATHALYZER_ENABLE_PIN, OUTPUT);
  digitalWrite(BREATHALYZER_ENABLE_PIN, LOW);
}

void loop() {
  if(Serial.read() == SYSTEM_ENABLE_BYTE) {
    for(int i = 0; i < MEASUREMENT_COUNT; i++) {
      int val = analogRead(BREATHALYZER_DATA_PIN) * MAX_BREATHALYZER_VALUE / MAX_ANALOG_VALUE;
      measurements[i] = val;
      delay(100);
    }
    sortArrayReverse(measurements, MEASUREMENT_COUNT);
    unsigned int result = (measurements[0] + measurements[1]) / 2;
    sentToExternal(result);
  }
}
