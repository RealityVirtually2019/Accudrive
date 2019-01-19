#include "SpeedCheck.h"


CheckSpeedingListener::CheckSpeedingListener()
{
    speedLimit = 50;
    LOGI("Speed Limit Registered: %f", speedLimit);
}

void CheckSpeedingListener::changed(const std::shared_ptr<double> speed) {
    if(*speed > speedLimit)
    {
        isSpeeding = true;
        LOGI("Speeding!! Current speed: %f", *speed);
    }
    else if(*speed <= speedLimit && isSpeeding)
    {
        isSpeeding = false;
        LOGI("Below speed limit again.");
    }
}



