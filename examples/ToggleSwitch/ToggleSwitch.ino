#include "SimpleCode.h"

// Note: This example is designed for a physical toggle switch (latching switch)

// Set up the toggle switch on digital pin 2
Switch toggleSwitch(2);

// Set up the LED on digital pin 13
LED testLed(13);

void setup() {
  // Open serial monitor at 9600 baud
  Serial.begin(9600);
  
  // Start the switch and LED hardware
  toggleSwitch.begin();
  testLed.begin();
  
  Serial.println("Physical Toggle Switch Example Started!");

  // Check the switch position right at startup
  if (toggleSwitch.isOn()) {
    Serial.println("Startup: Switch is already ON");
    testLed.on(); // Turn on the LED if the switch is closed
  } else {
    Serial.println("Startup: Switch is already OFF");
    testLed.off(); // Keep the LED off if the switch is open
  }
}

void loop() {
  // Triggered exactly ONCE when the switch is flipped to ON
  if (toggleSwitch.wasFlippedOn()) {
    testLed.on();
    Serial.println("Event: Switch flipped -> ON");
  }

  // Triggered exactly ONCE when the switch is flipped to OFF
  if (toggleSwitch.wasFlippedOff()) {
    testLed.off();
    Serial.println("Event: Switch flipped -> OFF");
  }

  // Check the current position of the switch continuously
  if (toggleSwitch.isOn()) {
    // Print every 2 seconds without freezing the code
    if (wait(2000)) {
      Serial.println("Toggle is ON");
    }
  } else if (toggleSwitch.isOff()) {
    // Print every 2 seconds without freezing the code
    if (wait(2000)) {
      Serial.println("Toggle is OFF");
    }
  }
}