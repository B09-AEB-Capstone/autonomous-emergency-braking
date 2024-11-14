#include <Adafruit_VL53L1X.h>
#include <ArduinoJson.h>

#include "websocket.h"
#include "motor.h"
#include "fuzzy.h"

const char *ssid = "akangcimoll";
const char *password = "anakayammakansapi2";

/* websocket */
const char *ip = "192.168.132.21";
const uint16_t port = 3000;
WebSocket webSocket(ssid, password, ip, port);

/* lidar */
Adafruit_VL53L1X vl53;
unsigned long lastTime = 0;
int16_t lastDistance = 0;
float relativeSpeed = 0.0;

const int brakeDistance = 300;
const int emergencyDistance = 50;

float speed = 0;
float distance = 0;
int distanceBack = 0;

motor myMotor(27, 26, 33, 32);

FuzzyBrakeController brakeController;
int brakingCategory;

void setup()
{
  Serial.begin(9600);

  // Wire.begin(22, 21);
  // if (!vl53.begin(0x29, &Wire))
  // {
  //   Serial.println("Error initializing VL53L1X sensor");
  //   while (1)
  //     delay(10);
  // }

  // vl53.startRanging();
  // vl53.setTimingBudget(15);

  // lastTime = millis();
  // lastDistance = vl53.distance();

  webSocket.begin();
}

void loop()
{
  webSocket.loop();

  // String message = "speed: " + String(speed) + ", distance to front: " + String(distance_to_front) + ", distance to back: " + String(distance_to_back);
  // webSocket.send(message);

  // if (vl53.dataReady())
  // {
  //   int16_t currentDistance = vl53.distance();
  //   unsigned long currentTime = millis();

  //   if (currentDistance != -1)
  //   {
  //     float deltaTime = (currentTime - lastTime) / 1000.0;
  //     relativeSpeed = (currentDistance - lastDistance) / deltaTime;

  //     if (currentDistance < emergencyDistance)
  //     {
  //       myMotor.rapidBraking();
  //     }
  //     else if (currentDistance < brakeDistance)
  //     {
  //       if (brakingCategory = 3)
  //       {
  //         myMotor.apply3StageBrake(relativeSpeed);
  //       }
  //       else if (brakingCategory = 4)
  //       {
  //         myMotor.apply4StageBrake(relativeSpeed);
  //       }
  //       else if (brakingCategory = 5)
  //       {
  //         myMotor.apply5StageBrake(relativeSpeed);
  //       }

  //       if (relativeSpeed > -30 && currentDistance >= emergencyDistance)
  //       {
  //         myMotor.accelerateMotor();
  //       }

  //       lastDistance = currentDistance;
  //       lastTime = currentTime;
  //     }
  //     brakingCategory = brakeController.getBrakeLevel(currentDistance, myMotor._dutyCycle);
  //     String outputName = brakeController.getBrakeLevelName(brakingCategory);

  //     Serial.println(brakingCategory);
  //     Serial.println(outputName);
  //     Serial.println();

  //     Serial.print("Distance: ");
  //     Serial.print(currentDistance);
  //     Serial.print(" mm, Relative Speed: ");
  //     Serial.print(relativeSpeed);
  //     Serial.print(" mm/s, Current PWM Duty Cycle: ");
  //     Serial.print(myMotor._dutyCycle);
  //     Serial.println(" (0-255)");

  //     String message = "speed: " + String(abs(relativeSpeed)) + ", distance: " + String(currentDistance);
  //     webSocket.send(message);

  //     myMotor.accelerateMotor();
  //   }
  // }

  speed = random(2000, 5000) / 100.0;
  distance = random(5000, 20000) / 100.0;

  // Create a JSON object to send current data
  DynamicJsonDocument jsonDoc(200);
  jsonDoc["speed"] = speed;
  jsonDoc["distanceFront"] = distance;
  jsonDoc["distanceBack"] = distanceBack;
  // Serialize JSON to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Send JSON string over WebSocket
  webSocket.send(jsonString);

  delay(200);
}
