#ifndef CONFIG_H
#define CONFIG_H

// WiFi
const char* WIFI_SSID     = "TP-LINK_4558";
const char* WIFI_PASSWORD = "123456789@";

// MQTT - HiveMQ Cloud
#define MQTT_SERVER "237def3f76ff4662a4b4c4567452c4b8.s1.eu.hivemq.cloud"
#define MQTT_PORT   8883
#define MQTT_USER   "hnam04"
#define MQTT_PASS   "Manh2704@"
#define MQTT_TOPIC_STATUS  "lettuceguard/status"
#define MQTT_TOPIC_CONTROL "lettuceguard/control"

// Chân kết nối
#define SOIL_PIN      34
#define DS18B20_PIN   4
#define RELAY_PIN     5
#define LCD_ADDR      0x27
#define LCD_COLS      16
#define LCD_ROWS      2

#endif
