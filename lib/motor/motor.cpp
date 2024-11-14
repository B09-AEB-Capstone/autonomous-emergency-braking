#include "motor.h"

motor::motor(int pin1, int pin2, int pin3, int pin4)
{
    ledcAttachPin(pin1, 0);
    ledcSetup(0, _freq, _resolution);
    ledcWrite(pin1, _dutyCycle);
    digitalWrite(pin2, LOW);

    ledcAttachPin(pin3, 0);
    ledcSetup(0, _freq, _resolution);
    ledcWrite(pin3, _dutyCycle);
    digitalWrite(pin4, LOW);
}

void motor::apply3StageBrake(float speed)
{
    int decrement = 0;
    if (speed > -75 && speed <= -30)
    {
        decrement = 15;
    }
    else if (speed > -150 && speed <= -75)
    {
        decrement = 30;
    }
    else if (speed <= -150)
    {
        decrement = 45;
    }

    Serial.print("3 stage, dec: ");
    Serial.println(decrement);
    updateDutyCycle(decrement);
}

void motor::apply4StageBrake(float speed)
{
    int decrement = 0;
    if (speed > -50 && speed <= -25)
    {
        decrement = 10;
    }
    else if (speed > -100 && speed <= -50)
    {
        decrement = 20;
    }
    else if (speed > -150 && speed <= -100)
    {
        decrement = 30;
    }
    else if (speed <= -150)
    {
        decrement = 45;
    }

    Serial.print("4 stage, dec: ");
    Serial.println(decrement);
    updateDutyCycle(decrement);
}

void motor::apply5StageBrake(float speed)
{
    int decrement = 0;
    if (speed > -60 && speed <= -30)
    {
        decrement = 9;
    }
    else if (speed > -90 && speed <= -60)
    {
        decrement = 18;
    }
    else if (speed > -120 && speed <= -90)
    {
        decrement = 27;
    }
    else if (speed > -150 && speed <= -120)
    {
        decrement = 36;
    }
    else if (speed <= -150)
    {
        decrement = 45;
    }

    Serial.print("5 stage, dec: ");
    Serial.println(decrement);
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
        _dutyCycle = max(0, _dutyCycle - 50);
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
        if (_dutyCycle < 255)
        {
            _dutyCycle = min(255, _dutyCycle + 10);
            ledcWrite(pin1, _dutyCycle);
            ledcWrite(pin3, _dutyCycle);
        }
    }
}
