### 🌐 Выберите язык / Select Language
[Русский](README_RU.MD) | [English](README.MD)

# SimpleCode

An Arduino library that makes code shorter and cleaner by replacing long blocks of code, debounce routines, and timers with simple, intuitive functions.

### Features
* **Explicit Initialization**: Pin configuration is cleanly separated into `.begin()` methods for better control and adherence to standard Arduino practices.
* **Manual Bulk Setup**: Easily configure an entire continuous range of raw pins using `regPins()` or an arbitrary array of pins using `regPinsArray()`.
* **No delay()**: Switch debouncing and LED blinking work asynchronously under the hood using `millis()`, keeping your execution loop perfectly non-blocking.
* **Advanced LED Control**: Simple methods to control LEDs (`on()`, `off()`, `toggle()`), along with an advanced `blink()` function supporting independent ON and OFF intervals.
* **Universal Switch Handling**: Divided into state-polling (`is...`) and event-driven (`was...`) methods, making it perfectly suitable for both tactical push-buttons and latching toggle switches.

---

### How to Use (Functions & Classes)

#### Global Functions (For setup)

* **`regPins(firstPin, lastPin, mode)`**  
  Configures a continuous range of pins from start to end.
  * `firstPin` (uint8_t): The first pin in the range.
  * `lastPin` (uint8_t): The last pin in the range.
  * `mode` (byte): Pin mode (`INPUT`, `INPUT_PULLUP`, or `OUTPUT`).

* **`regPinsArray(pinsArray, arraySize, mode)`**  
  Configures an array of pins that can be in any arbitrary or non-sequential order.
  * `pinsArray` (const uint8_t[]): Your custom array containing pin numbers.
  * `arraySize` (byte): Total number of pins in the array (use `sizeof(pinsArray) / sizeof(pinsArray[0])`).
  * `mode` (byte): Pin mode (`INPUT`, `INPUT_PULLUP`, or `OUTPUT`).

#### Class: Switch
Designed for both tactile push-buttons (momentary switches) and physical latching toggle switches (uses internal `INPUT_PULLUP`).

* **`Switch(pin, debounceDelay = 50, clickTimeout = 400)`**  
  Constructor to create a switch/button object.
  * `debounceDelay`: Anti-flicker debounce time in milliseconds (defaults to 50ms).
  * `clickTimeout`: Max duration in ms for a press to be registered as a quick click by `wasPressed()` (defaults to 400ms).
* **`begin()`** — Configures the pin as `INPUT_PULLUP`. Must be called inside `setup()`.
* **`isPressed()`** — Returns `true` continuously as long as the switch is physically active/held down.
* **`isReleased()`** — Returns `true` continuously as long as the switch is physically inactive/released.
* **`wasPressed()`** — Returns `true` strictly once upon releasing the button, provided it was held down for less than `clickTimeout` ms (detects a short click event).
* **`wasReleased()`** — Returns `true` strictly once at the exact moment the switch transitions from active (LOW) to inactive (HIGH).

#### Class: LED
Used for controlling LEDs without blocking the main loop execution.

* **`LED(pin)`** — Constructor to create an LED object on a specific pin.
* **`begin()`** — Configures the pin as `OUTPUT`. Must be called inside `setup()`.
* **`on()`** — Turns the LED ON. Resets any ongoing blink state.
* **`off()`** — Turns the LED OFF. Resets any ongoing blink state.
* **`toggle()`** — Inverts the LED state (turns it ON if it was off, or OFF if it was on).
* **`blink(onTime, offTime)`** — Blinks the LED asynchronously inside the `loop()`.
  * `onTime` (unsigned long): Duration the LED stays ON in milliseconds.
  * `offTime` (unsigned long): Duration the LED stays OFF in milliseconds.
* **`stopBlink()`** — Stops the asynchronous blinking process and forces the LED OFF.

---

### Example

```cpp
#include "SimpleCode.h"

// NOTE: This example is specifically designed for Toggle / Latching switches (ON/OFF).
// Create a Switch object on pin 2 (perfect for toggle switches or buttons)
Switch myButton(2);

// Create an LED object on pin 13
LED myLED(13);

void setup() {
  // Initialize both the switch and LED
  myButton.begin();
  myLED.begin();
}

void loop() {
  // Check if the switch is currently active
  if (myButton.isPressed()) {
    // Blinks asynchronously without blocking the loop
    myLED.blink(500, 500); 
  } else {
    // When the switch is turned off, stop blinking immediately
    myLED.stopBlink(); 
  }
}
```

---

## License
This project is licensed under the MIT License - see the `LICENSE.txt` file for details.
