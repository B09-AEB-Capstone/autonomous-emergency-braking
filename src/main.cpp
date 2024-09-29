#include <Arduino.h>
#include "Input.h"

Input inputObject;  // Create an instance of the Input class

void setup() {
  Serial.begin(115200);  // Initialize Serial Monitor
}

void loop() {
  inputObject.printMessage();  // Call the method from the Input class
  // Your main loop code
}
