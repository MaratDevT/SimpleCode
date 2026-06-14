#include "SimpleCode.h"

bool globalPinRegistry[256] = { false };
static unsigned long _globalWaitTimer = 0;

void regPins(uint8_t firstPin, uint8_t lastPin, byte mode) {
  for (byte i = firstPin; i <= lastPin; i++) {
    pinMode(i, mode);
    globalPinRegistry[i] = true; 
  }
}

void regPinsArray(const uint8_t pins[], byte count, byte mode) {
  for (byte i = 0; i < count; i++) {
    pinMode(pins[i], mode);
    globalPinRegistry[pins[i]] = true; 
  }
}

bool wait(unsigned long ms) {
  if (millis() - _globalWaitTimer >= ms) {
    _globalWaitTimer = millis();
    return true;
  }
  return false;
}

Switch::Switch(uint8_t pin, unsigned long debounceDelay, unsigned long clickTimeout) {
  _pin = pin;
  _debounceDelay = debounceDelay;
  _clickTimeout = clickTimeout; 
  _wasReleasedState = true; 
  
  _tgLastDebouncedState = HIGH; 
  _tgLastRawState = HIGH;
  _tgLastDebounceTime = 0;
  _tgChangedToOn = false;
  _tgChangedToOff = false;
}

void Switch::begin() {
  pinMode(_pin, INPUT_PULLUP);
  globalPinRegistry[_pin] = true;

  int startReading = digitalRead(_pin);
  _tgLastDebouncedState = startReading;
  _tgLastRawState = startReading;
}

bool Switch::isPressed() {
  if (!globalPinRegistry[_pin]) return false;

  int reading = digitalRead(_pin);
  if (reading != _dbIsPressed.lastRaw) {
    _dbIsPressed.timer = millis();
    _dbIsPressed.lastRaw = reading;
  }

  if ((millis() - _dbIsPressed.timer) >= _debounceDelay) {
    if (reading == LOW && _dbIsPressed.lastDebounced == HIGH) {
      _dbIsPressed.timer400 = millis();
      _dbIsPressed.pending = true;
      _dbIsPressed.held = false;
    }
    _dbIsPressed.lastDebounced = reading;
  }

  if (_dbIsPressed.lastDebounced == LOW) {
    if (_dbIsPressed.pending && (millis() - _dbIsPressed.timer400 >= _clickTimeout)) {
      _dbIsPressed.held = true;
      return true;
    }
  }

  return false; 
}

bool Switch::wasPressed() {
  if (!globalPinRegistry[_pin]) return false;

  int reading = digitalRead(_pin);
  if (reading != _dbWasPressed.lastRaw) {
    _dbWasPressed.timer = millis();
    _dbWasPressed.lastRaw = reading;
  }

  if ((millis() - _dbWasPressed.timer) >= _debounceDelay) {
    if (reading == LOW && _dbWasPressed.lastDebounced == HIGH) {
      _dbWasPressed.timer400 = millis();
      _dbWasPressed.pending = true;
      _dbWasPressed.held = false; 
    }
    
    if (reading == HIGH && _dbWasPressed.lastDebounced == LOW) {
      _dbWasPressed.lastDebounced = reading;
      if (millis() - _dbWasPressed.timer400 >= _clickTimeout) {
        _dbWasPressed.held = true;
      }
      if (_dbWasPressed.pending && !_dbWasPressed.held && (millis() - _dbWasPressed.timer400 < _clickTimeout)) {
        _dbWasPressed.pending = false;
        return true;
      }
      _dbWasPressed.pending = false;
    }
    _dbWasPressed.lastDebounced = reading;
  }

  return false;
}

bool Switch::isReleased() {
  if (!globalPinRegistry[_pin]) return true; 

  int reading = digitalRead(_pin);
  if (reading != _dbIsReleased.lastRaw) {
    _dbIsReleased.timer = millis();
    _dbIsReleased.lastRaw = reading;
  }

  if ((millis() - _dbIsReleased.timer) >= _debounceDelay) {
    _dbIsReleased.lastDebounced = reading;
  }

  return (_dbIsReleased.lastDebounced == HIGH);
}

bool Switch::wasReleased() {
  if (!globalPinRegistry[_pin]) return false;

  int reading = digitalRead(_pin);
  if (reading != _dbWasReleased.lastRaw) {
    _dbWasReleased.timer = millis();
    _dbWasReleased.lastRaw = reading;
  }

  if ((millis() - _dbWasReleased.timer) >= _debounceDelay) {
    if (reading == HIGH && _dbWasReleased.lastDebounced == LOW) {
      _dbWasReleased.lastDebounced = reading;
      return true;
    }
    _dbWasReleased.lastDebounced = reading;
  }
  return false;
}

void Switch::tickToggle() {
  if (!globalPinRegistry[_pin]) return;

  int reading = digitalRead(_pin);

  if (reading != _tgLastRawState) {
    _tgLastDebounceTime = millis();
    _tgLastRawState = reading;
  }

  if ((millis() - _tgLastDebounceTime) >= _debounceDelay) {
    if (reading != _tgLastDebouncedState) {
      if (reading == LOW) {   
        _tgChangedToOn = true;
      } else {                
        _tgChangedToOff = true;
      }
      _tgLastDebouncedState = reading;
    }
  }
}

bool Switch::wasFlippedOn() {
  tickToggle();
  if (_tgChangedToOn) {
    _tgChangedToOn = false;
    return true;
  }
  return false;
}

bool Switch::wasFlippedOff() {
  tickToggle();
  if (_tgChangedToOff) {
    _tgChangedToOff = false;
    return true;
  }
  return false;
}

bool Switch::isOn() {
  tickToggle();
  return (_tgLastDebouncedState == LOW);
}

bool Switch::isOff() {
  tickToggle();
  return (_tgLastDebouncedState == HIGH);
}


LED::LED(uint8_t pin) {
  _pin = pin;
  _timer = 0; 
  _state = LOW;
  _isBlinking = false;
  _onTime = 500;
  _offTime = 500;
}

void LED::begin() {
  pinMode(_pin, OUTPUT);
  globalPinRegistry[_pin] = true;
}

void LED::on() {
  if (!globalPinRegistry[_pin]) return; 
  _isBlinking = false; 
  digitalWrite(_pin, HIGH);
  _state = HIGH;
}

void LED::off() {
  if (!globalPinRegistry[_pin]) return;
  _isBlinking = false; 
  digitalWrite(_pin, LOW);
  _state = LOW;
}

void LED::blink(unsigned long onTime, unsigned long offTime) {
  if (!globalPinRegistry[_pin]) return;

  if (!_isBlinking) {
    _isBlinking = true;
    _timer = millis();
    _state = HIGH;
    digitalWrite(_pin, _state);
  }

  _onTime = onTime;
  _offTime = offTime;

  unsigned long currentInterval = (_state == HIGH) ? _onTime : _offTime;

  if (millis() - _timer >= currentInterval) {
    _state = !_state;          
    digitalWrite(_pin, _state); 
    _timer = millis();         
  }
}

void LED::stopBlink() {
  _isBlinking = false; 
  _state = LOW;
  digitalWrite(_pin, LOW);
}

void LED::toggle() {
  if (!globalPinRegistry[_pin]) return;
  _isBlinking = false;
  _state = !_state;
  digitalWrite(_pin, _state); 
}
