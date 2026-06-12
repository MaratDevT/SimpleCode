#ifndef SIMPLECODE_H
#define SIMPLECODE_H

#include <Arduino.h>

void regPins(uint8_t firstPin, uint8_t lastPin, byte mode);

void regPinsArray(const uint8_t pins[], uint8_t count, byte mode);

class Button {
  private:
    uint8_t _pin;
    bool _wasPressed;
    bool _isInitialized;
    unsigned long _debounceTimer;
    unsigned long _debounceDelay;

  public:
    Button(uint8_t pin, unsigned long debounceDelay = 50);
    
    bool isPressed();
};

class LED {
  private:
    uint8_t _pin;
    unsigned long _timer; 
    bool _isInitialized;
    bool _state;          

  public:
    LED(uint8_t pin);
    
    void on();
    void off();
    void blink(unsigned long interval); 
    void toggle(); 
};



#endif