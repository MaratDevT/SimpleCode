# SimpleCode

An Arduino library that makes code shorter and cleaner by replacing long blocks of code with simple functions.

## Features

- **Auto Pin Setup for Objects (Lazy Initialization):** You don't need to write `pinMode()` in `setup()` when using `Button` or `LED` objects. The library configures these pins automatically upon initialization.
- **Manual Bulk Setup:** For raw pins, you can easily configure them all at once using `regPins()` or `regPinsArray()`.
- **No delay():** Button debouncing and LED blinking work without blocking the code, using `millis()` under the hood.
- **Set Multiple Pins at Once:** Configure a range of pins with `regPins()` or a custom array of pins with `regPinsArray()` in a single line.
- **Basic LED Control:** Simple methods to control LEDs: `on()`, `off()`, `toggle()`, and non-blocking `blink()`.
- **Compatibility:** Made with standard Arduino functions. Tested on classic AVR boards.

## How to Use (Functions & Classes)

### Global Functions
These functions help you configure many pins at once in `setup()`.

#### `regPins(firstPin, lastPin, mode)`
Configures a continuous range of pins from start to end.
- `firstPin` (uint8_t): The first pin in the range.
- `lastPin` (uint8_t): The last pin in the range.
- `mode` (byte): Pin mode (`INPUT`, `INPUT_PULLUP`, or `OUTPUT`).

#### `regPinsArray(pinsArray, arraySize, mode)`
Configures an array of pins that can be in any arbitrary, custom, or non-sequential order.
- `pinsArray` (const uint8_t[]): Your custom array with pin numbers.
- `arraySize` (byte): Total number of pins in the array (use `sizeof(array) / sizeof(array[0])`).
- `mode` (byte): Pin mode (`INPUT`, `INPUT_PULLUP`, or `OUTPUT`).

---

### Class: `Button`
Designed specifically for tactile push-buttons (momentary switches without physical latching/toggle). It detects the exact moment the button is pressed.

#### `Button(pin, debounceDelay)`
Constructor to create a button object.
- `pin` (uint8_t): Pin number where the button is connected.
- `debounceDelay` (unsigned long): Anti-flicker time in milliseconds.

#### `isPressed()`
Checks if the button was clicked. 
- **Returns:** `true` strictly **once** per click (on edge detection). Returns `false` at all other times, even if you keep holding the button down.

---

### Class: `LED`
Used for controlling LEDs without blocking the code execution.

#### `LED(pin)`
Constructor to create an LED object on a specific pin.

#### `on()`
Turns the LED on.

#### `off()`
Turns the LED off.

#### `toggle()`
Inverts the LED state (turns it ON if it was off, or OFF if it was on).

#### `blink(interval)`
Blinks the LED asynchronously inside the `loop()`.
- `interval` (unsigned long): Time between ON and OFF states in milliseconds.

## Installation

### Method 1: Via Arduino Library Manager (Recommended)
1. Open the **Arduino IDE**.
2. Go to **Sketch** -> **Include Library** -> **Manage Libraries...**
3. Search for **SimpleCode** and click **Install**.

### Method 2: Manual Installation
1. Download this repository.
2. Copy the `SimpleCode` folder into your Arduino `libraries` directory (usually located in `Documents/Arduino/libraries`).
3. Restart the Arduino IDE.

## Example

```cpp
// Example: Blinking multiple LEDs at different rates without blocking the code.
#include "SimpleCode.h"

// Create two LED objects on different pins
LED statusLed(13); // Built-in LED on most Arduino boards
LED alertLed(12);  // External LED on pin 12

void setup() {
  // Pin configuration is handled automatically by the LED class.
}

void loop() {
  // Blink the status LED every 500ms
  statusLed.blink(500);
  
  // Blink the alert LED every 150ms (much faster)
  // Both LEDs blink independently
  alertLed.blink(150);
}
```

## License

This project is licensed under the MIT License - see the LICENSE.txt file for details.
