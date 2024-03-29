int potPin = A3;    // Potentiometer output connected to analog pin 3
int potVal1 = 0;    // Variable to store the first potentiometer reading, 0-1023
int potVal2 = 0;    // Variable to store the second potentiometer reading
int calibLow = 1024; // Max Val
int calibHigh = -1;  // Min Val
int calib = 0;
int repCount = -1;
int jointAngle = 0;
unsigned long prevTime = 0;  // Variable to store the previous time
float acceleration = 0;  // Variable to store acceleration
int buttonPin = 2;   // Pin for the button
bool calibrationMode = false;  // Flag to indicate calibration mode
bool buttonState = false; // Current state of the button
bool lastButtonState = false; // Previous state of the button
int topFlag = 0; // Flag to indicate if the top of the joint range is reached
bool exerciseMode = false; // Flag to indicate exercise mode

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
        exerciseMode = false; // Exit exercise mode if in it
      }
    }
    delay(50); // Delay for debouncing
  }
  
  lastButtonState = buttonState; // Update the last button state
  
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    if (message == "Exercise") {
      // Enter exercise mode
      Serial.println("Entering exercise mode...");
      exerciseMode = true;
    }
  }

  if (!exerciseMode) { // Only run the following if not in exercise mode
    if (calibrationMode) {
      // Calibration mode
      repCount = 0;
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
        potVal1 = analogRead(potPin);  // Read the first potentiometer value
        
      // Acceleration
      if (currentTime - prevTime >= 100) {  // Read potentiometer value every 100 milliseconds, step time
        if(calibLow != 1024 && calibHigh != -1){
          jointAngle = map(potVal1, calibLow, calibHigh, 0, 300);
          if(jointAngle > 0 && jointAngle <= 300){
            Serial.print("Joint Angle: ");
            Serial.print(jointAngle);
            Serial.println(" Degrees");
          }
          
        }
        else
          Serial.print("calibration needed");

        
        delay(50); // Delay for stability (optional)
        potVal2 = analogRead(potPin);  // Read the second potentiometer value
        // Calculate acceleration (change in velocity / change in time)
        float velocity1 = potVal1;
        float velocity2 = potVal2;
        acceleration = (velocity2 - velocity1) / ((currentTime - prevTime) / 1000.0); // Convert time to seconds
        
        prevTime = currentTime;  // Update previous time
        // Serial.print("Acceleration: ");
        // Serial.print(acceleration);
        if(repCount >= 1 && (calibHigh - calibLow > 300)){
          Serial.print(" Rep: ");
          Serial.println(repCount);
        }
        else
          Serial.println();
      }

        
        int totalRange = (calibHigh - calibLow);
        int threshHoldLow = (totalRange * .2) + calibLow;
        int threshHoldHigh = (0.8 * totalRange ) + calibLow;
        
        if(potVal1 >= threshHoldHigh && topFlag == 0){
          topFlag = 1;
        }
        if(topFlag == 1 && potVal1 <= threshHoldLow){
          topFlag = 0;
          repCount++;
        }

        if(topFlag == 1)
          Serial.println("flag on!");

    }
  }
  else {
    // In exercise mode, pause other activities
    // You can add your specific code for exercise mode here
    // For example, you could display a message indicating exercise mode is active
    Serial.println("Exercise mode active. Press 'Exercise' again to exit.");
    delay(1000); // Add a delay to avoid rapid toggling
    if (Serial.available() > 0) {
      String message = Serial.readStringUntil('\n');
      if (message == "Exercise") {
        // Enter exercise mode
        Serial.println("Entering exercise mode...");
        exerciseMode = false;
      }
    }
  }
  
}
