#include "SimpleCode.h"

// Note: This example is designed for a momentary pushbutton (tactile switch)

// Set up the button on pin 2
Switch testButton(2);

// Set up the LED on pin 13
LED testLed(13);

void setup() {
  // Open serial monitor at 9600 speed
  Serial.begin(9600);
  
  // Start the button and LED
  testButton.begin();
  testLed.begin();
  
  Serial.println("Library Test Started!");
}

void loop() {
  // If the button is being held down right now
  if (testButton.isPressed()) {
    // Blink the LED: 100ms on, 200ms off
    testLed.blink(100, 200);
  }

  // If the button was just clicked
  if (testButton.wasPressed()) {
    Serial.println("Event: Short Click Detected!");
    testLed.off(); // Turn off the LED
  }

  // If the button was just released
  if (testButton.wasReleased()) {
    Serial.println("Event: Button Released!");
  }

  // If the button is not pressed right now
  if (testButton.isReleased()) {
    // Wait for 500ms without freezing the code
    if (wait(500)){
      Serial.println("Button Idle...");
    }
  }
}