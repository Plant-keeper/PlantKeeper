#include <Arduino.h>
#include "DHT.h"
#include "ArduinoJson.h"

DHT dht(2, DHT11);

const int dry = 1023;
const int wet = 700;

// put function declarations here:
int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  dht.begin();
  Serial.begin(115200);
}

void loop()
{

  int val1 = analogRead(0);
  int val2 = analogRead(1);

  int percentage1 = map(val1, dry, wet, 0, 100);
  int percentage2 = map(val2, dry, wet, 0, 100);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

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
  delay(50);
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}