#ifndef Buttons_h
#define Buttons_h

#include "Arduino.h"

class Button
{
  public:
    Button(int _buttonPin);
    Button(int _buttonPin, int _buttonType);

    void process();
    void pressHandler(void (*f)(int));
    void releaseHandler(void (*f)(int));
    void (*pressCallback)(int);
    void (*releaseCallback)(int);

    int buttonType = -1;
    byte buttonPin;
    bool firstCheck;
    bool buttonState;
    bool lastButtonState;
    unsigned long lastCheck;
    int checkInt;

    static const int LEFT   = 1;
    static const int RIGHT  = 2;
    static const int UP     = 3;
    static const int DOWN   = 4;

  private:
};


#endif
