#include <Arduino.h>
#include "Input.h"
#include "tfcw.h"
#include"ttc.h"
#include "tpb.h"

Input inputObject;  
TFCW tfcw(1.5, 3.0);            
TTC ttc;
TPB tpb;

void setup() {
  Serial.begin(115200);  // Initialize Serial Monitor
}

void loop() {
  inputObject.printMessage();  // Call the method from the Input class
  // Your main loop code
}

