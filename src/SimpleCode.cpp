#include "SimpleCode.h"

void regPins(uint8_t firstPin,uint8_t lastPin,byte mode){
    for(byte i = firstPin; i <= lastPin; i++){
        pinMode(i,mode);
    }
}

void regPinsArray(const uint8_t pins[], byte count, byte mode){
    for(byte i = 0; i < count; i++){
        pinMode(pins[i], mode);
    }
}

Button::Button(uint8_t pin, unsigned long debounceDelay) {
  _pin = pin;
  _debounceDelay = debounceDelay;
  _wasPressed = false;
  _isInitialized = false;
  _debounceTimer = 0;
}

bool Button::isPressed() {
  if (!_isInitialized) {
    pinMode(_pin, INPUT_PULLUP);
    _isInitialized = true;
  }

  int reading = digitalRead(_pin);
  bool triggered = false;

  if (reading == LOW && !_wasPressed) {
    if (millis() - _debounceTimer >= _debounceDelay) {
      _wasPressed = true;
      _debounceTimer = millis();
      triggered = true;
    }
  } 
  else if (reading == HIGH && _wasPressed) {
    if (millis() - _debounceTimer >= _debounceDelay) {
      _wasPressed = false;
      _debounceTimer = millis();
    }
  }

  return triggered;
}

LED::LED(uint8_t pin) {
  _pin = pin;
  _timer = 0; 
  _isInitialized = false;
  _state = LOW;
}

void LED::on() {
  if (!_isInitialized) { pinMode(_pin, OUTPUT); _isInitialized = true; }
  digitalWrite(_pin, HIGH);
}

void LED::off() {
  if (!_isInitialized) { pinMode(_pin, OUTPUT); _isInitialized = true; }
  digitalWrite(_pin, LOW);
}

void LED::blink(unsigned long interval) {
  if (!_isInitialized) { 
    pinMode(_pin, OUTPUT); 
    _isInitialized = true; 
    _timer = millis(); 
  }

  if (millis() - _timer >= interval) {
    _state = !_state;          
    digitalWrite(_pin, _state); 
    _timer = millis();         
  }
}

void LED::toggle() {
  if (!_isInitialized) {
    pinMode(_pin, OUTPUT);
    _isInitialized = true;
  }
  _state = !_state;
  digitalWrite(_pin, _state); 
}