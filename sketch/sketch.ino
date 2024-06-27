#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <DHT.h>

#define DHTPIN 4     
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "";
const char* password = "";
const char* serverAddress = "http://192.168.0.168:5000/data"; 

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  dht.begin(); 

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  JSONVar payload;
  payload["temperature"] = temperature;
  payload["humidity"] = humidity;

  HTTPClient http;
  http.begin(serverAddress);

  http.addHeader("Content-Type", "application/json");

  String jsonString = JSON.stringify(payload);

  int httpResponseCode = http.POST(jsonString);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST request: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void loop() {
  delay(60000);
}
