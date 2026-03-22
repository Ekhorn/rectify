#include <SoftwareSerial.h>

SoftwareSerial t23Serial(10, 11); // RX, TX

void setup() {
  Serial.begin(115200);    // Talk to PC
  t23Serial.begin(115200); // Talk to Ingenic T23
  Serial.println("--- Bridge Ready ---");
}

void loop() {
  if (t23Serial.available()) {
    Serial.write(t23Serial.read()); // From Ingenic T23 to PC
  }
  if (Serial.available()) {
    t23Serial.write(Serial.read()); // From PC to T23
  }
}
