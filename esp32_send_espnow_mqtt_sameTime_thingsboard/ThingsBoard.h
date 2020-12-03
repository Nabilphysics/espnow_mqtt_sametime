#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

WiFiClient   wifiClient;
PubSubClient client(wifiClient);

#define WIFI_CHANNEL 3
// your network information
const char *ssid     = "HAC";
const char *password = "python";

// Replace with your own token and server, get it from thingsboard

char device[] = "nabil";
char token[]  = "Pyq9R7k5*******anUi";
char server[] = "demo.thingsboard.io";// or IP address of your server, if you set up your own either IP or Domain

bool thingsboard_Publish(String data) {
  if (!client.connected())
  {
    Serial.println("Thingsboard Not Connected");
    while (!client.connect(device, token, NULL))
    {
      delay(100);
    }
  }

  String payload = data;
  char   attributes[400];//attributes[400] = This value will be changed according to data size
  payload.toCharArray(attributes, 400);
  client.publish("v1/devices/me/telemetry", attributes);
}

void thingsboard_Init() {
  if (strcmp(WiFi.SSID().c_str(), ssid) != 0)
  {
    WiFi.begin(ssid, password, 1); //WiFi.begin(ssid, password, WIFI_CHANNEL);
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("WiFi Not Connected");
  }
  client.setServer(server, 1883);
}
