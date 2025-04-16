#include "HardwareSerial.h"
#include <ArduinoJson.h>
#include <WiFi.h>

//----- Wi-Fi configuration -----
const char *ssid = "TIM-32726474";
const char *password = "PP3hTsb3fzbkEKeD2kK4A5SE";
const uint16_t port = 80;
const char *server_ip = "192.168.1.184";

WiFiServer server(port);
WiFiClient client;

void debugWiFiConnection(uint8_t state) {
  switch (state) {
  case WL_IDLE_STATUS:
    Serial.println("Idle");
    break;
  case WL_NO_SSID_AVAIL:
    Serial.println("SSID not available");
    break;
  case WL_SCAN_COMPLETED:
    Serial.println("Scan completed");
    break;
  case WL_CONNECT_FAILED:
    Serial.println("Connection failed");
    break;
  case WL_CONNECTION_LOST:
    Serial.println("Connection lost");
    break;
  case WL_DISCONNECTED:
    Serial.println("Disconnected");
    break;
  default:
    Serial.printf("Unknown status: %d\n", state);
    break;
  }
}

bool connectToWiFi(uint32_t timeout_ms = 20000) {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < timeout_ms) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("\nWi-Fi connection failed");
    debugWiFiConnection(WiFi.status());
    return false;
  }
}

void setup_WiFi_server() {
  IPAddress apIP(192, 168, 1, 184);
  WiFi.config(apIP);

  if (connectToWiFi()) {
    server.begin();
    Serial.println("Server started");
  }
}

bool setup_WiFi_client() {
  IPAddress apIP(192, 168, 1, 185);
  WiFi.config(apIP);
  if (!connectToWiFi())
    return false;

  Serial.printf("Connecting to server at %s:%u...\n", server_ip, port);
  uint32_t start = millis();
  while (!client.connect(server_ip, port)) {
    if (millis() - start > 5000) {
      Serial.println("Server connection timeout!");
      return false;
    }
    delay(100);
  }

  Serial.println("Connected to server");
  return true;
}

void sendJson(float weights[], uint16_t weight_number, WiFiClient &conn) {
  const size_t capacity =
      JSON_ARRAY_SIZE(weight_number) + weight_number * 16 + 64;
  DynamicJsonDocument doc(capacity);
  JsonArray arr = doc.createNestedArray("floatArray");
  for (uint16_t i = 0; i < weight_number; ++i) {
    arr.add(weights[i]);
  }
  serializeJson(doc, conn);
  conn.println();
}

uint8_t syncWeights_server(uint16_t weight_number, float FlatWeights[]) {
  client = server.available();
  if (!client)
    return 0;

  Serial.println("New client connected");
  while (client.connected()) {
    if (client.available()) {
      String message = client.readStringUntil('\n');
      if (!message.isEmpty()) {
        Serial.print("\nClient data: ");
        Serial.println(message);
        sendJson(FlatWeights, weight_number, client);
        client.stop();
        Serial.println("Client disconnected");
        return 1;
      }
    } else {
      delay(100);
    }
  }
  delay(10000);
  client.stop();
  Serial.println("Client disconnected (timeout)");
  return 0;
}
uint8_t syncWeights_client(uint16_t weight_number, float FlatWeights[]) {
  if (!client.connected()) {
    Serial.println("Client not connected");
    return 0;
  }

  sendJson(FlatWeights, weight_number, client);

  uint32_t start = millis();
  while (!client.available() && millis() - start < 5000) {
    Serial.print(".");
    delay(500);
  }

  if (!client.available()) {
    Serial.println("\nNo response from server");
    return 0;
  }

  String response = client.readStringUntil('\n');
  Serial.print("\nServer response: ");
  Serial.println(response);

  return !response.isEmpty();
}
