#include "motor.h"

motor::motor(int pin1, int pin2, int pin3, int pin4)
{
    ledcAttachPin(pin1, 0);
    ledcSetup(0, _freq, _resolution);
    ledcWrite(pin1, _dutyCycle);
    digitalWrite(pin2, HIGH);

    ledcAttachPin(pin3, 0);
    ledcSetup(0, _freq, _resolution);
    ledcWrite(pin3, _dutyCycle);
    digitalWrite(pin4, HIGH);
}

void motor::apply3StageBrake(float distance)
{
    int decrement = 0;
    if (distance > 450 && distance <= 700)
    {
        decrement = 35;
    }
    else if (distance > 200 && distance <= 450)
    {
        decrement = 75;
    }
    else if (distance <= 200)
    {
        decrement = 90;
    }
    Serial.print("3 stage");
    // Serial.println(decrement);
    updateDutyCycle(decrement);
}

void motor::apply4StageBrake(float distance)
{
    int decrement = 0;
    if (distance > 600 && distance <=750)
    {
        decrement = 38;
    }
    else if (distance > 450 && distance <= 600)
    {
        decrement = 48;
    }
    else if (distance > 250 && distance <= 450)
    {
        decrement = 68;
    }
    else if (distance <= 250)
    {
        decrement = 83;
    }

    Serial.print("4 stage");
    // Serial.println(decrement);
    updateDutyCycle(decrement);
}

void motor::apply5StageBrake(float distance)
{
    int decrement = 0;
    if (distance > 740 && distance <= 820)
    {
        decrement = 28;
    }
    else if (distance > 600 && distance <= 740)
    {
        decrement = 37;
    }
    else if (distance > 400 && distance <= 600)
    {
        decrement = 45;
    }
    else if (distance > 250 && distance <= 400)
    {
        decrement = 64;
    }
    else if (distance <= 250)
    {
        decrement = 88;
    }

    Serial.print("5 stage");
    // Serial.println(decrement);
    updateDutyCycle(decrement);
}

void motor::updateDutyCycle(int decrement)
{
    unsigned long currentMillis = millis();
    if (currentMillis - _previousBrakingMillis >= _brakingInterval)
    {
        _previousBrakingMillis = currentMillis;
        _dutyCycle = max(0, _dutyCycle - decrement);
        ledcWrite(pin1, _dutyCycle);
        ledcWrite(pin3, _dutyCycle);
    }
}

void motor::rapidBraking()
{
    unsigned long currentMillis = millis();
    if (currentMillis - _previousBrakingMillis >= _rapidBrakingInterval)
    {
        _previousBrakingMillis = currentMillis;
        _dutyCycle = max(0, _dutyCycle - 115);
        ledcWrite(pin1, _dutyCycle);
        ledcWrite(pin3, _dutyCycle);
    }
}

void motor::accelerateMotor()
{
    unsigned long currentMillis = millis();
    if (currentMillis - _previousAccelMillis >= _accelerationInterval)
    {
        _previousAccelMillis = currentMillis;
        if (_dutyCycle < maxSpeed)
        {
            _dutyCycle = min(maxSpeed, _dutyCycle + 20);
            ledcWrite(pin1, _dutyCycle);
            ledcWrite(pin3, _dutyCycle);
        }
    }
}
