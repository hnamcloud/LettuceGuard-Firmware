#ifndef STATUS_H
#define STATUS_H

#include "sensors.h"

void publishStatus(const SensorData& data, float stress);

#endif
