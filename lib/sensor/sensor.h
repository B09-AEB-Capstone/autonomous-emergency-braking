#pragma once

#include <Adafruit_VL53L1X.h>
#include <Wire.h>

class lidar
{
public:
    lidar();
    bool initialize();
    float update();
    int16_t getCurrentDistance() const;
    float getRelativeSpeed() const;

private:
    Adafruit_VL53L1X vl53;
    int16_t lastDistance;
    unsigned long lastTime;
    float relativeSpeed;
};