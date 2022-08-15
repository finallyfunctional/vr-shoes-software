#include "encoder.h"

int Encoder::ticks;
int Encoder::pinA;
int Encoder:: pinB;

void Encoder::initialize(int pinA, int pinB) {
    Serial.println("Enabling encoder interrupts");
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(pinA), pinAInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinB), pinBInterrupt, CHANGE);
    Encoder::pinA = pinA;
    Encoder::pinB = pinB;
}

void Encoder::pinAInterrupt() {
  if (digitalRead(pinA) == HIGH) {
    if (digitalRead(pinB) == LOW) {
      ticks++;
    }
    else {
      ticks--;
    }
  } 
  else {
    if (digitalRead(pinB) == HIGH) {
      ticks++;
    }
    else {
      ticks--;
    }
  }
}

void Encoder::pinBInterrupt() {
  if (digitalRead(pinB) == HIGH) {
    if (digitalRead(pinA) == HIGH) {
      ticks++;
    }
    else {
      ticks--;
    }
  }
  else {
    if (digitalRead(pinA) == LOW) {
      ticks++;
    }
    else {
      ticks--;
    }
  }
}