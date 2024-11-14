#pragma once

#include <Arduino.h>

class motor
{
public:
    int pin1, pin2, pin3, pin4;
    int _dutyCycle = 0;

    motor(int pin1, int pin2, int pin3, int pin4);
    void apply3StageBrake(float speed);
    void apply4StageBrake(float speed);
    void apply5StageBrake(float speed);
    void updateDutyCycle(int decrement);
    void rapidBraking();
    void accelerateMotor();

private:
    const int _freq = 5000;
    const int _resolution = 8;

    unsigned long _previousBrakingMillis = 0;
    unsigned long _previousAccelMillis = 0;

    const unsigned long _brakingInterval = 200;
    const unsigned long _rapidBrakingInterval = 200;
    const unsigned long _accelerationInterval = 100;
};
