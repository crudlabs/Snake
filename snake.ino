#include <Adafruit_NeoPixel.h>
#include "Button.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;
AudioOutputAnalog        dac1;
AudioConnection          patchCord1(waveform1, dac1);
// GUItool: end automatically generated code


Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, 12, NEO_GRB + NEO_KHZ800);

const int LEFT  = 0;
const int RIGHT = 1;
const int UP    = 2;
const int DOWN  = 3;

const int MOVE  = 0;
const int EAT   = 1;
const int LOSE  = 2;

Button leftButton(11, Button::LEFT);
Button rightButton(9, Button::RIGHT);
Button upButton(8, Button::UP);
Button downButton(10, Button::DOWN);

int snakeArray[64];
int snakeLength;
int snakeSpeed = 300;
int snakeHeadPosition;
int snakeDirection;
int snakeState;

int eggPosition = 0;
unsigned long lastStepTime = 0;

void setup() {
  //init serial
  Serial.begin(9600);

  AudioMemory(12);
  waveform1.begin(0, 0, WAVEFORM_SQUARE);

  //init neopixel
  strip.begin();
  strip.show();
  //  strip.setBrightness(200);

  //set button listeners
  leftButton.pressHandler(onButtonPress);
  rightButton.pressHandler(onButtonPress);
  upButton.pressHandler(onButtonPress);
  downButton.pressHandler(onButtonPress);

  //set initial snake conditions
  clearAndReset();

  //draw
  updateNeopixel();
}

void loop() {
  processButtons();

  if (millis() > lastStepTime + snakeSpeed) {
    moveSnake();
    checkEgg();
    updateNeopixel();

    lastStepTime = millis();
  }

  sound();
}

void processButtons() {
  leftButton.process();
  rightButton.process();
  upButton.process();
  downButton.process();
}

void sound() {
  if (millis() < lastStepTime + 100) {
    waveform1.amplitude(1);
    if (snakeState == MOVE) waveform1.frequency(261);
    else if (snakeState == EAT) waveform1.frequency(522);
    else if (snakeState == LOSE) waveform1.frequency(130);
  } else {
    waveform1.amplitude(0);
  }
}

void moveSnake() {

  snakeState = MOVE;

  int lastSnakeHeadPosition = snakeHeadPosition;

  if (snakeDirection == LEFT)   snakeHeadPosition -= 1;
  if (snakeDirection == RIGHT)  snakeHeadPosition += 1;
  if (snakeDirection == UP)     snakeHeadPosition -= 8;
  if (snakeDirection == DOWN)   snakeHeadPosition += 8;

  //if it went over the left edge
  if (lastSnakeHeadPosition % 8 == 0 && snakeHeadPosition % 8 == 7) loseGame();
  //if it went over the right edge
  else if (lastSnakeHeadPosition % 8 == 7 && snakeHeadPosition % 8 == 0) loseGame();
  //if it went over the top or bottom edge
  else if (snakeHeadPosition < 0 || snakeHeadPosition >= 64) loseGame();
  //if the head hits the body
  else if (partOfSnakeBody(snakeHeadPosition)) loseGame();
  //if we didn't hit anything move the snakes head
  else updateSnake();
}

void updateSnake() {
  //move every piece of the snake down one position in the array
  for (int i = 63; i > 0; i--) {
    snakeArray[i] = snakeArray[i - 1];
  }

  //put the new head position at the front of the array
  snakeArray[0] = snakeHeadPosition;
}

void checkEgg() {
  //if the snake ate the egg
  if (headOnEgg()) {
    snakeState = EAT;
    snakeLength++;
    moveEgg();
  }
}

bool headOnEgg() {
  if (eggPosition == snakeHeadPosition) return true;
  else return false;
}

void moveEgg() {
  //set the egg to a new random place
  eggPosition = random(0, 64);

  //but make sure its not on the snakes body
  while (partOfSnakeBody(eggPosition)) {
    eggPosition = random(0, 64);
  }
}

bool partOfSnakeBody(int n) {
  for (int i = 0; i < snakeLength; i++) {
    if (n == snakeArray[i]) return true;
  }
  return false;
}

void loseGame() {
  snakeState = LOSE;
  clearAndReset();
}

void clearAndReset() {
  snakeLength = 1;
  snakeHeadPosition = 35;
  updateSnake();
  snakeDirection = random(0, 4);
  moveEgg();
}

void updateNeopixel() {
  strip.clear();

  //fill every pixel with bground color
  for (int i = 0; i < 64; i++) {
    //    strip.setPixelColor(i, 0, 0, 2);
    strip.setPixelColor(i, 10, 0, 0);
  }

  //color snake's head
  //strip.setPixelColor(snakeArray[0], 160, 0, 0);
  strip.setPixelColor(snakeArray[0], 0, 0, 160);

  //color rest of snake's body
  for (int i = 1; i < snakeLength; i++) {
    //strip.setPixelColor(snakeArray[i], 15, 1, 1);
    strip.setPixelColor(snakeArray[i], 0, 0, 50);
  }

  //color egg
  strip.setPixelColor(eggPosition, 0, 160, 0);

  strip.show();
}

void onButtonPress(int buttonType) {  
  if (buttonType == Button::LEFT) {
    Serial.println("left");
    snakeDirection = LEFT;
  } else if (buttonType == Button::RIGHT) {
    Serial.println("right");
    snakeDirection = RIGHT;
  } else if (buttonType == Button::UP) {
    Serial.println("up");
    snakeDirection = UP;
  } else if (buttonType == Button::DOWN) {
    Serial.println("down");
    snakeDirection = DOWN;
  } 
}
