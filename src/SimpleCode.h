#ifndef SimpleCode_h
#define SimpleCode_h

#include <Arduino.h>

extern bool globalPinRegistry[256];
void regPins(uint8_t firstPin, uint8_t lastPin, byte mode);
void regPinsArray(const uint8_t pins[], byte count, byte mode);
bool wait(unsigned long ms);

class Switch {
  private:
  uint8_t _pin;
  unsigned long _debounceDelay;
  unsigned long _clickTimeout;
  bool _wasReleasedState;
  int _tgLastDebouncedState;
  int _tgLastRawState;
  unsigned long _tgLastDebounceTime;
  bool _tgChangedToOn;
  bool _tgChangedToOff;
  struct DebounceState {
    unsigned long timer = 0;
    int lastDebounced = HIGH;
    int lastRaw = HIGH;
    
    unsigned long timer400 = 0;
    bool pending = false;
    bool held = false;
  };

  DebounceState _dbIsPressed;
  DebounceState _dbWasPressed;
  DebounceState _dbIsReleased;
  DebounceState _dbWasReleased;
  
  public:
    Switch(uint8_t pin, unsigned long debounceDelay = 50, unsigned long clickTimeout = 400); 
    void begin();
    bool isPressed();
    bool wasPressed();
    bool isReleased();
    bool wasReleased();
    void tickToggle();
    bool wasFlippedOn();
    bool wasFlippedOff();
    bool isOn(); 
    bool isOff();
};

class LED {
  private:
    uint8_t _pin;
    unsigned long _timer;
    int _state;  
    bool _isBlinking;
    unsigned long _onTime;
    unsigned long _offTime;

  public:
    LED(uint8_t pin);
    void begin();
    void on();
    void off();
    void blink(unsigned long onTime, unsigned long offTime);
    void stopBlink();
    void toggle();
};

#endif