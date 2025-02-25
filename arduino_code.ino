#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

// Wi-Fi credentials
const char *ssid = "My-2G";       // Replace with your Wi-Fi SSID
const char *password = "XXXXXXXX"; // Replace with your Wi-Fi password

// REST API endpoint
const char *base_rest_url = "http://192.168.29.86:5000/api/sensors"; // Replace with your REST API server URL

// DHT11 sensor setup
#define DHTPIN D1          // Pin where the DHT11 is connected
#define DHTTYPE DHT11      // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor

// Read interval (5 seconds)
unsigned long previousMillis = 0;
const long readInterval = 5000;

// Struct to store DHT11 readings
struct DHT11Readings {
  float temperature;
  float humidity;
  float heat_index_c;
  float heat_index_f;
};

// Size of the JSON document
const int JSON_DOC_SIZE = 256;

// Wi-Fi and HTTP client objects
WiFiClient client;
HTTPClient http;

// Function to calculate heat index
float calculateHeatIndex(float temperature, float humidity, bool isFahrenheit = false) {
  return dht.computeHeatIndex(temperature, humidity, isFahrenheit);
}

// Function to send DHT11 readings to the REST API
void sendDHT11Readings(DHT11Readings dhtReadings) {
  // Prepare JSON data
  String jsondata = "";
  StaticJsonDocument<JSON_DOC_SIZE> doc;

  doc["temperature_in_c"] = dhtReadings.temperature;
  doc["temperature_in_f"] = dhtReadings.temperature * 1.8 + 32; // Convert Celsius to Fahrenheit
  doc["humidity"] = dhtReadings.humidity;
  doc["heat_index_in_c"] = dhtReadings.heat_index_c;
  doc["heat_index_in_f"] = dhtReadings.heat_index_f;

  // Serialize JSON to a string
  if (serializeJson(doc, jsondata) == 0) {
    Serial.println("Failed to serialize JSON!");
    return;
  }

  Serial.println("JSON Data:");
  Serial.println(jsondata);

  // Send HTTP POST request
  http.begin(client, base_rest_url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsondata);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);
    Serial.print("Response: ");
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

// Function to read DHT11 sensor data
DHT11Readings readDHT11() {
  // Read humidity and temperature
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Calculate heat index
  float heat_index_c = calculateHeatIndex(temperature, humidity, false);
  float heat_index_f = calculateHeatIndex(temperature, humidity, true);

  // Return the readings
  return {temperature, humidity, heat_index_c, heat_index_f};
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to read the sensor and send data
  if (currentMillis - previousMillis >= readInterval) {
    previousMillis = currentMillis;

    // Read DHT11 sensor data
    Serial.println("Reading DHT11 sensor...");
    DHT11Readings readings = readDHT11();

    // Check if the readings are valid
    if (isnan(readings.humidity) || isnan(readings.temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Print the readings to the Serial Monitor
    Serial.print("Temperature: ");
    Serial.println(readings.temperature);
    Serial.print("Humidity: ");
    Serial.println(readings.humidity);
    Serial.print("Heat Index (C): ");
    Serial.println(readings.heat_index_c);
    Serial.print("Heat Index (F): ");
    Serial.println(readings.heat_index_f);

    // Send the readings to the REST API
    Serial.println("Sending data to REST API...");
    sendDHT11Readings(readings);
  }

  // Reconnect to Wi-Fi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Wi-Fi reconnected.");
  }
}
