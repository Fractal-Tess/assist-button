#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <wifi.hpp>
#include <ESPAsyncWebServer.h>

#define BUTTON_PIN (uint8_t)13
#define LED_PIN (uint8_t)2

// Create AsyncWebServer object on port 80
AsyncWebServer server(32199);
bool requires_assistance = false;

void init_wifi()

{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void init_web_server()
{
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request)

            {
              char msg[128];
              sprintf(msg, "{\"requiresAssistance\":\"%s\"}", requires_assistance ? "YES" : "NO");
               request->send_P(200, "application/json", msg ); });

  server.begin();
}

void init_pins()
{
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Initilizing WiFi");
  init_wifi();

  Serial.println("Initilizing WebServer");
  init_web_server();

  Serial.println("Initilizing Pins");
  init_pins();

  Serial.println("Initilization done.");
}

void loop()
{
  const int button_value = digitalRead(BUTTON_PIN);
  if (button_value)
  {
    requires_assistance = true;
    digitalWrite(LED_PIN, HIGH);

    delay(10000);
    requires_assistance = false;
    digitalWrite(LED_PIN, LOW);
  }
}