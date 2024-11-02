#include <Arduino.h>
#include <ArduinoJson.h> // Include the Arduino JSON library
#include "Input.h"
#include "tfcw.h"
#include "ttc.h"
#include "tpb.h"
#include "WebSocket.h"

// WiFi and WebSocket server credentials
const char* ssid = "Capstone AEB";          // Replace with your WiFi SSID
const char* password = "capstoneaeb";       // Replace with your WiFi password
const char* serverAddress = "172.20.10.3";  // Replace with WebSocket server address
int port = 3001;                            // Replace with your WebSocket server port

// Global variables
Input inputObject;  
TFCW tfcw(1.5, 3.0);            
TTC ttc;
TPB tpb;

float speed = 0;
float distance = 0;
int distanceBack = 0; // Define distanceBack variable to be used by WebSocket

WebSocket webSocket(ssid, password, serverAddress, port);  // Initialize WebSocket with credentials

void setup() {
  Serial.begin(115200);  
  webSocket.begin();  // Begin WebSocket connection
}

void loop() {
  webSocket.loop();  // Check for WebSocket events and messages

  // Generate random speed and distance for demonstration purposes
  inputObject.generateRandomData(speed, distance);
  
  // Create a JSON object to send current data
  DynamicJsonDocument jsonDoc(200);
  jsonDoc["speed"] = speed;
  jsonDoc["distanceFront"] = distance;
  jsonDoc["distanceBack"] = distanceBack;  // Include the latest distanceBack value

  // Serialize JSON to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Send JSON string over WebSocket
  webSocket.send(jsonString);

  delay(1000);  // Send data every second (adjust as needed)
}
