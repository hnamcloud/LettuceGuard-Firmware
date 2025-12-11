#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <PubSubClient.h>
#include "pump.h"
#include "sensors.h"

extern PubSubClient client;
extern bool autoMode;

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String cmd;
  for (int i = 0; i < length; i++) cmd += (char)payload[i];

  cmd.replace("\"", ""); 
  
  cmd.trim();
  cmd.toUpperCase();

  Serial.println("Nhận lệnh ĐÃ XỬ LÝ: " + cmd);

  if (cmd == "AUTO") {
    autoMode = true;
  }
  else if (cmd == "MANUAL") {
    autoMode = false;
    pumpOff();
  }
  else if (cmd == "PUMP_ON" && !autoMode) {
    pumpOn();
  }
  else if (cmd == "PUMP_OFF" && !autoMode) {
    pumpOff();
  }
}

// ----------- Helper chống NaN gửi lên MQTT -----------
String safeFloat(float v) {
  if (isnan(v)) return "null";
  return String(v, 2);
}

// ----------- Gửi trạng thái lên MQTT -----------
void publishStatus(const SensorData& d, float stress) {

  // Tạo JSON theo đúng struct SensorData
  String json = "{";
  json += "\"humidity\":" + safeFloat(d.humidity) + ",";
  json += "\"light\":" + safeFloat(d.light) + ",";
  json += "\"airTemp\":" + safeFloat(d.airTemp) + ",";
  json += "\"stress\":" + safeFloat(stress) + ",";
  json += "\"mode\":\"" + String(autoMode ? "AUTO" : "MANUAL") + "\",";

  // Errors
  json += "\"errors\":[";
  bool first = true;
  if (d.soilFail)  { json += "\"SOIL_FAIL\""; first = false; }
  if (d.lightFail) { if (!first) json += ","; json += "\"LIGHT_FAIL\""; first = false; }
  if (d.tempFail)  { if (!first) json += ","; json += "\"TEMP_FAIL\""; }
  json += "]";

  json += "}";

  client.publish(MQTT_TOPIC_STATUS, json.c_str());
  Serial.println("Sent: " + json);
}

#endif
