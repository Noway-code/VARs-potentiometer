#define BUTTON_PIN 2

//GND and 5V with UNO
//10k resistor on negative line with a digital out to 2 on other side. positive line just has a wire to positive rail.

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(BUTTON_PIN));
  delay(100);
}
