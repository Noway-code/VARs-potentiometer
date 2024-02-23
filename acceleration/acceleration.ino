// INPUT: Potentiometer should be connected to 5V and GND
int potPin = A3;    // Potentiometer output connected to analog pin 3
int potVal1 = 0;    // Variable to store the first potentiometer reading, 0-1023
int potVal2 = 0;    // Variable to store the second potentiometer reading
int calibLow = 1024; // Max Val
int calibHigh = 0;  // Min Val
int calib = 0;
unsigned long prevTime = 0;  // Variable to store the previous time
float acceleration = 0;  // Variable to store acceleration

int buttonPin = 2;   // Pin for the button
bool calibrationMode = false;  // Flag to indicate calibration mode

bool buttonState = false; // Current state of the button
bool lastButtonState = false; // Previous state of the button

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
}

void loop() {
  unsigned long currentTime = millis(); // Get the current time
  
  // Read the state of the button
  buttonState = digitalRead(buttonPin);
  
  // Check if the button state has changed
  if (buttonState != lastButtonState) {
    // If the button state has changed, toggle calibration mode
    if (buttonState == LOW) {
      calibrationMode = !calibrationMode;
      
      if (calibrationMode) {
        Serial.println("Entering calibration mode...");
        calibLow = 1024; // Reset calibration low
        calibHigh = 0;   // Reset calibration high
      }
    }
    delay(50); // Delay for debouncing
  }
  
  lastButtonState = buttonState; // Update the last button state
  
  if (calibrationMode) {
    // Calibration mode
    calib = analogRead(potPin);

    if (calib < calibLow) {
      calibLow = calib;
    }
    if (calib > calibHigh) {
      calibHigh = calib;
    }

    Serial.print("Calibrating... Current value: ");
    Serial.print(calib);
    Serial.print(" Min: ");
    Serial.print(calibLow);
    Serial.print(" Max: ");
    Serial.println(calibHigh);
    delay(250);
  } else {
    // Measurement mode
    if (currentTime - prevTime >= 50) {  // Read potentiometer value every 100 milliseconds, step time
      potVal1 = analogRead(potPin);  // Read the first potentiometer value
      delay(10); // Delay for stability (optional)
      potVal2 = analogRead(potPin);  // Read the second potentiometer value
      
      // Calculate acceleration (change in velocity / change in time)
      float velocity1 = potVal1;
      float velocity2 = potVal2;
      acceleration = (velocity2 - velocity1) / ((currentTime - prevTime) / 1000.0); // Convert time to seconds
      
      prevTime = currentTime;  // Update previous time
      Serial.print("Acceleration: ");
      Serial.println(acceleration);
    }

    
  }
}
