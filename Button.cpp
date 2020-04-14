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

void Button::process()
{
  if (millis() < lastCheck + checkInt) return;

  lastCheck = millis();

  lastButtonState = buttonState;
  buttonState = digitalRead(buttonPin);

  if (lastButtonState == LOW && buttonState == HIGH) {
    pressCallback();
  }
  if (lastButtonState == HIGH && buttonState == LOW) {
//    releaseCallback();
  }
}

void Button::pressHandler(void (*f)())    //button down
{
  pressCallback = *f;
}

void Button::releaseHandler(void (*f)())  //button release
{
  releaseCallback = *f;
}
