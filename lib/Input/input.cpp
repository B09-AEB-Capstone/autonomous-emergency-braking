#include "Input.h"
#include <Arduino.h>

// Constructor
Input::Input() {
  // Constructor code (if needed)
}

// Function to print a message
void Input::printMessage() {
  Serial.println("Hello from Input class!");
}

void Input::generateRandomData(float &velocity, float &distance) {
  velocity = random(2000, 5000) / 100.0; 

  distance = random(5000, 20000) / 100.0; 
}
