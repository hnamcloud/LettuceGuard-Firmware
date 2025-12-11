#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <BH1750.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#include "config.h"
#include "sensors.h"
#include "stress.h"
#include "pump.h"
#include "display.h"
#include "mqtt_handler.h"

BH1750 lightMeter;
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

WiFiClientSecure espClient;
PubSubClient client(espClient);

bool autoMode = true;
bool pumpRunning = false;
unsigned long pumpStartTime = 0;

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.print("LettuceGuard v2");
  lcd.setCursor(0,1);
  lcd.print("Dang khoi dong...");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Wire.begin();
  lightMeter.begin();
  ds18b20.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected!");

  espClient.setInsecure();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqttCallback);

  while (!client.connect("LettuceGuard", MQTT_USER, MQTT_PASS)) delay(1000);
  client.subscribe(MQTT_TOPIC_CONTROL);
  Serial.println("MQTT connected!");

  lcd.clear();
  lcd.print("He thong ok!");
  delay(1500);
}

void loop() {
  if (!client.connected()) {
    while (!client.connect("LettuceGuard", MQTT_USER, MQTT_PASS)) delay(1000);
    client.subscribe(MQTT_TOPIC_CONTROL);
  }
  client.loop();

  static unsigned long last = 0;
  if (millis() - last < 5000) return;
  last = millis();

  SensorData data = readAllSensors();
  float stress = calculateStress(data);

  if (autoMode) controlPump(stress);

  publishStatus(data, stress);
  updateLCD(data, stress);
}


