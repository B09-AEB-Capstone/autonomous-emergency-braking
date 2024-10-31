#include <Arduino.h>
#include "Input.h"
#include "tfcw.h"
#include "ttc.h"
#include "tpb.h"
#include "WebSocket.h"

const char* ssid = "Capstone AEB";          // Replace with your WiFi SSID
const char* password = "capstoneaeb";  // Replace with your WiFi password
const char* serverAddress = "172.20.10.3"; // Replace with WebSocket server address
int port = 3000;  // Replace with your WebSocket server port

Input inputObject;  
TFCW tfcw(1.5, 3.0);            
TTC ttc;
TPB tpb;

float velocity = 0;
float distance = 0;

WebSocket webSocket(ssid, password, serverAddress, port);

void setup() {
  Serial.begin(115200);  

  webSocket.begin();
}

void loop() {
  webSocket.loop();

  inputObject.generateRandomData(velocity, distance);
  
  // Send data over WebSocket
  String message = String("Velocity: ") + velocity + ", Distance: " + distance;
  webSocket.send(message);

  delay(1000); 
}
