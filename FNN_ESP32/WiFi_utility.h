#include "HardwareSerial.h"
#include <stdint.h>
#include <WiFi.h>

  //-----Wi-Fi configuration-----
  const char* ssid = "ssid";
  const char* password = "password";
  const uint16_t port = 80;

  IPAddress apIP(192,168,1,184);
  WiFiServer server(port);

void setup_WiFi() {

    WiFi.config(apIP);
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println("Wi-Fi connected");
    Serial.println(WiFi.localIP());

    //Server starts
    server.begin();
}
void swap_weights(uint16_t weight_number , float FlatWeights[]) {

  Serial.print(weight_number);

  for(uint16_t i = 0; i <= weight_number; i++)
    Serial.println(FlatWeights[i] );

  JsonDocument doc;
  JsonArray arr = doc.createNestedArray("floatArray");
  for(uint16_t i = 0; i < 5 ; i++){
      arr.add(FlatWeights[i]);
    }

  // New client
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");

    while (client.connected()) {
      if (client.available()) {

        //Message received
        String message = client.readStringUntil('\n');
        Serial.println(message);

        //Response to client
        serializeJson(doc, client);
        delay(5000);
      }
    }
    //Kill connection with client
    client.stop();
    Serial.println("Client disconnected");
  }
}
