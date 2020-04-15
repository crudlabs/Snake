#include "Arduino.h"
#include "Button.h"

Button::Button(int _buttonPin)
{
  buttonPin = _buttonPin;

  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(buttonPin, HIGH);

  lastCheck = 0;
  checkInt = 20;
}

Button::Button(int _buttonPin, int _buttonType):Button(_buttonPin)
{
  buttonType = _buttonType;
}

void Button::process()
{
  if (millis() < lastCheck + checkInt) return;

  lastCheck = millis();

  lastButtonState = buttonState;
  buttonState = digitalRead(buttonPin);

  if (lastButtonState == LOW && buttonState == HIGH) {
    pressCallback(buttonType);
  }
  if (lastButtonState == HIGH && buttonState == LOW) {
//    releaseCallback();
  }
}

void Button::pressHandler(void (*f)(int))    //button down
{
  pressCallback = *f;
}

void Button::releaseHandler(void (*f)(int))  //button release
{
  releaseCallback = *f;
}
