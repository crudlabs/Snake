#ifndef Controls_h
#define Controls_h

#include "Button.h"

class Controls {
  public:
    Controls();

    Button *controlArray[10];
    int numControls = 0;

    void process();
    void addControl(Button *b);
    void onButtonPress(int buttonType);
    void pressHandler(void (*f)(int));
    void (*pressCallback)(int);

  private:

};

#endif
