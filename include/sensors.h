#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <BH1750.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

extern BH1750 lightMeter;
extern DallasTemperature ds18b20;

struct SensorData {
  float humidity;
  float light;
  float airTemp;
  bool soilFail;
  bool lightFail;
  bool tempFail;
};

SensorData readAllSensors() {
  SensorData d;

  // Soil
  int raw = analogRead(SOIL_PIN);
  d.humidity = map(raw, 4095, 0, 0, 100);
  d.humidity = constrain(d.humidity, 0.0f, 100.0f);
  d.soilFail = (raw == 0 || raw == 4095);

  // Light
  d.light = lightMeter.readLightLevel();
  d.lightFail = (d.light <= 0 || d.light >= 60000);

  // DS18B20
  ds18b20.requestTemperatures();
  float t = ds18b20.getTempCByIndex(0);

  if (t == DEVICE_DISCONNECTED_C || t == 85.0f) {
    d.airTemp = NAN;
    d.tempFail = true;
  } else {
    d.airTemp = t;
    d.tempFail = false;
  }

  return d;
}

#endif
