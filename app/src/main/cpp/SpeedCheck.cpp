#include "SpeedCheck.h"


CheckSpeedingListener::CheckSpeedingListener()
{
    speedLimit = KiloPerHourToMetersPerSecond(50.0);
    LOGI("Speed Limit Registered: %f", speedLimit);
}

void CheckSpeedingListener::changed(const std::shared_ptr<double> speed) {
    LOGI("Current speed: %f Speed Limit:%f IsSpeeding:%d", *speed, speedLimit,isSpeeding);
    if(*speed > speedLimit && isSpeeding == false)
    {
        isSpeeding = true;
        LOGI("Speeding!! Current speed: %f", *speed);
        scoreTracker->SubtractScore(SPEEDING);
    }
    else if(*speed <= speedLimit && isSpeeding)
    {
        isSpeeding = false;
        LOGI("Below speed limit again.");
    }
}
void CheckSpeedingListener::SetScoreTracker(std::shared_ptr<ScoreTracker> tracker) {
    scoreTracker = tracker;
}
double CheckSpeedingListener::KiloPerHourToMetersPerSecond(double kph)
{
    return kph * 1000/3600;
}


