/**
 * @file main.cpp
 * @brief: Program that reads temperature, humidity and soil moisture values from sensors and sends them to a server
 * 
 * @author: Rafael Dousse
 * @date 23.08.2024
 * @version 1.0
 */

#include <Arduino.h>
#include "DHT.h"
#include "ArduinoJson.h"

#include <WiFiNINA.h>
#include <arduino_secrets.h>
#include <webpage.h>

// Function prototypes
void enable_WiFi();
void connect_WiFi();
void printWifiStatus();
void printWEB(float t,float h ,int percentage1,int percentage2);

// Credentials
const char *wifi[] = {SECRET_SSID, SECRET_PASS};

// Network parameters
int keyIndex = 0;            // Network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS; // Connection status
WiFiServer server(80);       // Server socket
WiFiClient client = server.available();
String header;

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
  Serial.begin(115200);

  enable_WiFi();
  connect_WiFi();
  server.begin();
  printWifiStatus();
  dht.begin();

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


  client = server.available();

    if (client) {
        printWEB(t, h, percentage1, percentage2);
    }

  delay(500);
}


/**
 * @brief: Function that prints Wi-Fi connection information to the serial monitor.
 */
void printWifiStatus() {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());  

    IPAddress ip = WiFi.localIP(); 
    Serial.print("IP Address: ");
    Serial.println(ip); 

    long rssi = WiFi.RSSI(); 
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");  

    Serial.print("To see this page in action, open a browser to http://");
    Serial.println(ip); 
}

/**
 * Function that checks if the Wi-Fi module is available.
 * If the module is not found, the program stops.
 */
void enable_WiFi() {
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        // TODO: Change to a more elegant solution
        while (true);  // Infinite loop if Wi-Fi module is absent 
    }

    String fv = WiFi.firmwareVersion();
    if (fv < "1.0.0") {
        Serial.println("Please upgrade the firmware");  // Warn if the firmware is outdated
    }
}
// TODO: Change the wifi[0] to another system
/**
 * Function that attempts to connect to the Wi-Fi network.
 */
void connect_WiFi() {
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(wifi[0]); 
        status = WiFi.begin(wifi[0], wifi[1]);
        delay(5000);  

        if (status == WL_CONNECTED) {
            break;  // Exit the loop if connected
        }

    }
}


/**
 * Function that handles the HTTP response to display a web page.
 * It receives client requests, sends the HTML page, and manages the disconnection.
 */
void printWEB(float t, float h, int percentage1, int percentage2) {
    if (client) {  
        Serial.println("new client");
        String currentLine = "";  
        while (client.connected()) {  
            if (client.available()) {  
                char c = client.read();  // Read a character from the request
                Serial.write(c); 
                header += c;  // Append the character to the header

                if (c == '\n') { 
                    if (currentLine.length() == 0) {
                        // If the line is blank, it's the end of the HTTP request, so send the response
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        // Send the HTML page to the client
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        String webpage = generateWebpage(t, h, percentage1, percentage2);
                        client.println(webpage);  // Send the generated webpage
                      
                        client.println();

                        break;  
                    } else {  
                        currentLine = "";  // Reset the current line
                    }
                } else if (c != '\r') {   
                    currentLine += c;  // Append the character to the current line if it's not a carriage return
                }
            }
        }
        header = "";  // Reset the header after processing
        client.stop();  // Close the connection with the client
        Serial.println("client disconnected");
    }
}