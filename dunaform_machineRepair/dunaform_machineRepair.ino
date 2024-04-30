#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webpage.h"
#include <AsyncElegantOTA.h>
#include "piezo.h"

// #include <Arduino.h>
#define RELAY_PIN 23
#define LIMITSWITCH_PIN 16
#define BUZZER 15

//? PIEZO
const int piezoChannel = 8;
Piezo piezo(BUZZER, piezoChannel);

//? WIFI
const char *ssid = "dentartwork";
const char *password = "tutititkos444";
IPAddress staticIP(192, 168, 5, 51);
IPAddress gateway(192, 168, 5, 1);
IPAddress subnet(255, 255, 255, 0);

int duration = 5;
bool inProgress = false;
bool progressEnd = false;
unsigned long timeWhenActivated = 0;

AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LIMITSWITCH_PIN, INPUT_PULLUP);
  piezo.begin();

  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, password);
  ;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/set_duration", HTTP_GET, handleSetDuration);

  AsyncElegantOTA.begin(&server);
  server.begin();
  //? PIEZO
  piezo.playStartup();
}

void handleRoot(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", webpageCode);
}

void loop()
{
  AsyncElegantOTA.loop();

  if (!digitalRead(LIMITSWITCH_PIN))
  {
    if (!progressEnd)
    {
      if (!inProgress)
      {
        digitalWrite(RELAY_PIN, HIGH);
        inProgress = true;
        timeWhenActivated = millis();
      }
      if (millis() - timeWhenActivated > duration * 1000)
      {
        
        digitalWrite(RELAY_PIN, LOW);
        inProgress = false;
        progressEnd = true;
        delay(200);
        piezo.playBatteryFull();
      }
    }
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
    inProgress = false;
    progressEnd = false;
    timeWhenActivated = 0;
  }
  delay(20);
}

void handleSetDuration(AsyncWebServerRequest *request)
{
  if (request->hasParam("duration"))
  {
    String durationStr = request->getParam("duration")->value();
    duration = durationStr.toInt(); // Update global duration variable

    Serial.println(duration);

    request->send(200, "text/plain", "Duration set successfully");
  }
  else
  {
    request->send(400, "text/plain", "Invalid request");
  }
}