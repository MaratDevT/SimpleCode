### 🌐 Выберите язык / Select Language
[Русский](README_RU.MD) | [English](README.MD)

# SimpleCode
An Arduino library that makes code shorter and cleaner by replacing long blocks of code, debounce routines, and timers with simple, intuitive functions.

## Features
* **Explicit Initialization:** Pin configuration is cleanly separated into `.begin()` methods for better control and adherence to standard Arduino practices.
* **Manual Bulk Setup:** Easily configure an entire continuous range of raw pins using `regPins()` or an arbitrary array of pins using `regPinsArray()`.
* **No `delay()`:** Switch debouncing, LED blinking, and code waiting work asynchronously under the hood using `millis()`, keeping your execution loop perfectly non-blocking.
* **Advanced LED Control:** Simple methods to control LEDs (`on()`, `off()`, `toggle()`), along with an advanced `blink()` function supporting independent ON and OFF intervals.
* **Universal Switch Handling:** Features dedicated methods for different hardware. It uses `isPressed`/`wasPressed` for momentary push-buttons and `isOn`/`wasFlippedOn` for latching toggle switches.

---

## How to Use (Functions & Classes)

### Global Functions (For setup and timing)
* **`regPins(firstPin, lastPin, mode)`**  
  Configures a continuous range of pins from start to end.
  * `firstPin` *(uint8_t)*: The first pin in the range.
  * `lastPin` *(uint8_t)*: The last pin in the range.
  * `mode` *(byte)*: Pin mode (`INPUT`, `INPUT_PULLUP`, or `OUTPUT`).
* **`regPinsArray(pinsArray, arraySize, mode)`**  
  Configures an array of pins that can be in any arbitrary or non-sequential order.
  * `pinsArray` *(const uint8_t[])*: Your custom array containing pin numbers.
  * `arraySize` *(byte)*: Total number of pins in the array (use `sizeof(pinsArray) / sizeof(pinsArray[0])`).
  * `mode` *(byte)*: Pin mode (`INPUT`, `INPUT_PULLUP`, or `OUTPUT`).
* **`wait(ms)`**  
 A non-blocking alternative to `delay()`. It does not freeze your code and returns `true` when the waiting time is over
  * `ms` *(unsigned long)*: Time to wait in milliseconds.

---

### Class: Switch
Designed for both tactile push-buttons (momentary switches) and physical latching toggle switches (uses internal `INPUT_PULLUP`).

* **`Switch(pin, debounceDelay = 50, clickTimeout = 400)`**  
  Constructor to create a switch/button object.
  * `debounceDelay`: Anti-flicker debounce time in milliseconds (defaults to 50ms).
  * `clickTimeout`: Max duration for a press to be registered as a quick click by `wasPressed()` (defaults to 400ms).
* **`begin()`** — Configures the pin as `INPUT_PULLUP`. Must be called inside `setup()`.

#### Methods for Momentary Push-Buttons:
* **`isPressed()`** — Returns `true` continuously as long as the button is physically held down and the click timeout is exceeded (detects long press/hold).
* **`isReleased()`** — Returns `true` continuously as long as the button is physically inactive/released.
* **`wasPressed()`** — Returns `true` strictly once upon releasing the button, provided it was held down for less than `clickTimeout` ms (detects a short click event).
* **`wasReleased()`** — Returns `true` strictly once at the exact moment the button transitions from active (LOW) to inactive (HIGH).

#### Methods for Physical Toggle Switches:
* **`wasFlippedOn()`** — Returns `true` strictly once at the exact moment the toggle switch is flipped to the **ON** position.
* **`wasFlippedOff()`** — Returns `true` strictly once at the exact moment the toggle switch is flipped to the **OFF** position.
* **`isOn()`** — Returns `true` continuously as long as the toggle switch remains in the **ON** position.
* **`isOff()`** — Returns `true` continuously as long as the toggle switch remains in the **OFF** position.

---

### Class: LED
Used for controlling LEDs without blocking the main loop execution.

* **`LED(pin)`** — Constructor to create an LED object on a specific pin.
* **`begin()`** — Configures the pin as `OUTPUT`. Must be called inside `setup()`.
* **`on()`** — Turns the LED ON. Resets any ongoing blink state.
* **`off()`** — Turns the LED OFF. Resets any ongoing blink state.
* **`toggle()`** — Inverts the LED state (turns it ON if it was off, or OFF if it was on).
* **`blink(onTime, offTime)`** — Blinks the LED asynchronously inside the `loop()`.
  * `onTime` *(unsigned long)*: Duration the LED stays ON in milliseconds.
  * `offTime` *(unsigned long)*: Duration the LED stays OFF in milliseconds.
* **`stopBlink()`** — Stops the asynchronous blinking process and forces the LED OFF.

---

## Example: Toggle Switch Control
This example demonstrates how to use the library with a physical latching toggle switch to control an LED and handle state checking using non-blocking timers.

```cpp
#include "SimpleCode.h"

// Note: This example is designed for a physical toggle switch (latching switch)

// Set up the toggle switch on digital pin 2
Switch toggleSwitch(2);

// Set up the LED on digital pin 13
LED testLed(13);

void setup() {
  // Open serial monitor at 9600 speed
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
```

---

## License
This project is licensed under the MIT License - see the LICENSE.txt file for details.
