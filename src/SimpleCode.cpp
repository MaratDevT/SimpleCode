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
  _wasPressedState = false;      // Используем как флаг: "удержание уже происходило" (для блокировки клика)
  _wasReleasedState = true; 
  _debounceTimer = 0;
  _lastDebouncedState = HIGH;
  _lastRawState = HIGH;
  _pressDurationTimer = 0;
  _clickPending = false;
}

void Switch::begin() {
  pinMode(_pin, INPUT_PULLUP);
  globalPinRegistry[_pin] = true;
}

// ЛОГИКА УДЕРЖАНИЯ: шлет true постоянно, пока кнопка удерживается дольше таймаута
bool Switch::isPressed() {
  if (!globalPinRegistry[_pin]) return false;

  int reading = digitalRead(_pin);
  if (reading != _lastRawState) {
    _debounceTimer = millis();
    _lastRawState = reading;
  }

  if ((millis() - _debounceTimer) >= _debounceDelay) {
    _lastDebouncedState = reading;
  }

  // Если кнопка физически зажата с учетом дебаунса
  if (_lastDebouncedState == LOW) {
    // Если удерживаем дольше таймаута — возвращаем true постоянно в каждом цикле
    if (_clickPending && (millis() - _pressDurationTimer >= _clickTimeout)) {
      _wasPressedState = true; // Запоминаем, что удержание было (чтобы отменить клик)
      return true;
    }
  }

  return false; 
}

bool Switch::isReleased() {
  if (!globalPinRegistry[_pin]) return true; 

  int reading = digitalRead(_pin);
  if (reading != _lastRawState) {
    _debounceTimer = millis();
    _lastRawState = reading;
  }

  if ((millis() - _debounceTimer) >= _debounceDelay) {
    _lastDebouncedState = reading;
  }

  return (_lastDebouncedState == HIGH);
}

// ЛОГИКА КЛИКА: возвращает true при отпускании, только если НЕ было удержания
bool Switch::wasPressed() {
  if (!globalPinRegistry[_pin]) return false;

  int reading = digitalRead(_pin);
  if (reading != _lastRawState) {
    _debounceTimer = millis();
    _lastRawState = reading;
  }

  if ((millis() - _debounceTimer) >= _debounceDelay) {
    // Нажатие кнопки
    if (reading == LOW && _lastDebouncedState == HIGH) {
      _pressDurationTimer = millis();
      _clickPending = true;
      _wasPressedState = false; // Сброс флага для нового нажатия
    }
    
    // Отпускание кнопки
    if (reading == HIGH && _lastDebouncedState == LOW) {
      _lastDebouncedState = reading;
      // Если кнопку отпустили раньше таймаута и удержания не зафиксировано
      if (_clickPending && !_wasPressedState && (millis() - _pressDurationTimer < _clickTimeout)) {
        _clickPending = false;
        return true;
      }
      _clickPending = false;
    }
    _lastDebouncedState = reading;
  }

  return false;
}

bool Switch::wasReleased() {
  if (!globalPinRegistry[_pin]) return false;

  int reading = digitalRead(_pin);
  if (reading != _lastRawState) {
    _debounceTimer = millis();
    _lastRawState = reading;
  }

  if ((millis() - _debounceTimer) >= _debounceDelay) {
    if (reading == HIGH && _lastDebouncedState == LOW) {
      _lastDebouncedState = reading;
      return true;
    }
    _lastDebouncedState = reading;
  }
  return false;
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
