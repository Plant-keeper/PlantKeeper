#include <Arduino.h>
#include "DHT.h"
#include "ArduinoJson.h"

// Instantiate DHT and JSon object
DHT dht(2, DHT11);
JsonDocument doc;
// Global variables used in loop
int val1;
int val2;
int percentage1;
int percentage2;
float h;
float t;

const int dry = 1023;
const int wet = 700;

void setup()
{

  dht.begin();
  Serial.begin(115200);
}

void loop()
{
  // Read sensor values
  val1 = analogRead(0);
  val2 = analogRead(1);

  h = dht.readHumidity();
  t = dht.readTemperature();

  // Map sensor values to percentage
  percentage1 = map(val1, dry, wet, 0, 100);
  percentage2 = map(val2, dry, wet, 0, 100);

  // Create JSON object
  doc["temperature"] = t;
  doc["humidity"] = h;
  doc["val1"] = val1;
  doc["val2"] = val2;
  doc["moisture1"] = percentage1;
  doc["moisture2"] = percentage2;

  Serial.print("Humidity: ");
  Serial.println(h);
  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("Moisture Level: ");
  Serial.println(val1);
  Serial.println("Percentage1: ");
  Serial.println(percentage1);
  Serial.print("Moisture Level2: ");
  Serial.println(val2);
  Serial.println("Percentage2: ");
  Serial.println(percentage2);
  Serial.print("Pot : ");
  Serial.println(analogRead(A2));

  // Serialize JSON object to Serial
  serializeJson(doc, Serial);
  Serial.println();

  delay(500);
}
