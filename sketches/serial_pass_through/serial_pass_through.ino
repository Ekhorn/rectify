void setup() {
  // INPUT - High Impedance
  // see https://docs.arduino.cc/language-reference/en/variables/constants/inputOutputPullup/#input
  pinMode(0, INPUT); // T23 TX → Arduino Pin 0
  pinMode(1, INPUT); // T23 RX → Arduino Pin 1
}
void loop() {}
