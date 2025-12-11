#ifndef PUMP_H
#define PUMP_H

#include "config.h"

extern bool autoMode;
extern bool pumpRunning;
extern unsigned long pumpStartTime;

inline void pumpOn() {
  digitalWrite(RELAY_PIN, HIGH);
  pumpRunning = true;
  Serial.println("BƠM BẬT");
}

inline void pumpOff() {
  digitalWrite(RELAY_PIN, LOW);
  pumpRunning = false;
  Serial.println("BƠM TẮT");
}

void controlPump(float stress) {
  if (!autoMode) return;

  if (stress >= 70 && !pumpRunning) {
    pumpOn();
    pumpStartTime = millis();
  }

  if (pumpRunning && millis() - pumpStartTime >= 15000) {
    pumpOff();
  }
}

#endif
