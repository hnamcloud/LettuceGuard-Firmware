#ifndef STRESS_H
#define STRESS_H

#include "sensors.h"

inline float calculateStress(const SensorData& d) {
  float stress = 0.0f;

  // Soil moisture
  if (!d.soilFail) {
    if (d.humidity < 60) stress += (60 - d.humidity) * 2.0f;
    if (d.humidity > 90) stress += (d.humidity - 90) * 0.8f;
  }

  // Temperature
  if (!d.tempFail) {
    if (d.airTemp > 30) stress += (d.airTemp - 30) * 3.0f;
    if (d.airTemp < 15) stress += (15 - d.airTemp) * 1.5f;
  }

  // Light
  if (!d.lightFail) {
    if (d.light > 45000) stress += (d.light - 45000) / 1000.0f * 25.0f;
    if (d.light < 5000 && !d.tempFail && d.airTemp > 28) stress += 15.0f;
  }

  return constrain(stress, 0, 100);
}

#endif
