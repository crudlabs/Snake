#include "Arduino.h"
#include "Controls.h"

Controls::Controls() {
  
  Button *leftButton = new Button(11, Button::LEFT);
  addControl(leftButton);
  Button *rightButton = new Button(9, Button::RIGHT);
  addControl(rightButton);
  Button *upButton = new Button(8, Button::UP);
  addControl(upButton);
  Button *downButton = new Button(10, Button::DOWN);
  addControl(downButton);

  for (int i = 0; i < numControls; i++) {
    controlArray[i]->pressHandler(onButtonPress);
  }
}

void Controls::process() {
  for (int i = 0; i < numControls; i++) {
    controlArray[i]->process();
  }
}

void Controls::pressHandler(void (*f)(int)) {
  pressCallback = *f;
}

void Controls::addControl(Button *b) {
  controlArray[numControls] = b;
  numControls++;
}

void Controls::onButtonPress(int buttonType) {
  Serial.print("Controls buttonType: ");
  Serial.println(buttonType);
  pressCallback(buttonType);
}
