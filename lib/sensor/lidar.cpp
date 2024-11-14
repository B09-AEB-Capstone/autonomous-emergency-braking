#include "sensor.h"

lidar::lidar() : lastDistance(0), lastTime(0), relativeSpeed(0.0) {}

bool lidar::initialize()
{
    if (!vl53.begin(0x29, &Wire))
    {
        return false;
    }
    vl53.startRanging();
    vl53.setTimingBudget(15);
    lastTime = millis();
    lastDistance = vl53.distance();
    return true;
}

float lidar::update()
{
    if (vl53.dataReady())
    {
        int16_t currentDistance = vl53.distance();
        unsigned long currentTime = millis();

        if (currentDistance != -1)
        {
            float deltaTime = (currentTime - lastTime) / 1000.0;
            relativeSpeed = (currentDistance - lastDistance) / deltaTime;

            lastDistance = currentDistance;
            lastTime = currentTime;
        }
    }
    return relativeSpeed;
}

int16_t lidar::getCurrentDistance() const
{
    return lastDistance;
}

float lidar::getRelativeSpeed() const
{
    return relativeSpeed;
}
