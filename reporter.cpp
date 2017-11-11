#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <DHT.h>

const char* node = "attic";        // Node label value. Just make something up.
const char* ssid = "mywifi";       // WiFi SSID.
const char* pass = "password";     // WiFi password.
const char* addr = "123.456.78.9"; // InfluxDB address.
const int   port = 8089;           // InfluxDB UDP port.
const float tickrate = 1.0;        // Ticks per minute.

const int serialRate = 9600;
const int dhtpin = 13;

WiFiUDP UDP;
DHT dht(dhtpin, DHT22);

void setup() {
  WiFi.begin(ssid, pass);
  Serial.begin(serialRate);
}

void loop()
{
  ensureConnected();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("DHT read failed");
    return;
  }

  String hMsg = "humidity,node=" + String(node) + " value=" + String(h, 2);
  String tMsg = "temperature,node=" + String(node) + " value=" + String(t, 2);

  Serial.println(hMsg);
  Serial.println(tMsg);

  sendUDP(addr, port, hMsg);
  sendUDP(addr, port, tMsg);

  delay(60000/tickrate);
}

void ensureConnected() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(500);
  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void sendUDP(const char* addr, int port, String msg) {
  byte bytes[msg.length()+1];
  msg.getBytes(bytes, msg.length()+1);

  UDP.beginPacket(addr, port);
  UDP.write(bytes, msg.length());
  UDP.endPacket();
}

