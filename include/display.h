#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include "sensors.h"

extern LiquidCrystal_I2C lcd;
extern bool autoMode;
extern bool pumpRunning;

void updateLCD(const SensorData& d, float stress) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.print(d.humidity, 0);
  lcd.print("% L:");

  if (d.light >= 10000) {
    lcd.print(d.light / 1000.0f, 1);
    lcd.print("k");
  } else {
    lcd.print((int)d.light);
  }
  lcd.print("lx");

  lcd.setCursor(0, 1);
  lcd.print("T:");
  if (!d.tempFail) lcd.print(d.airTemp, 1);
  else lcd.print("--.-");

  lcd.print("C S:");
  lcd.print((int)stress);
  lcd.print("% ");

  lcd.print(autoMode ? "AUTO" : "MANUAL");
  if (pumpRunning) lcd.print(" BOM");
}

#endif
