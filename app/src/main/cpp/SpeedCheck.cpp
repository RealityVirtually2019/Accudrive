#include <context.h>
#include <application.h>
#include <android/sensor.h>
#include <android/log.h>
#include <utilities.h>
#include <detectionobject.h>
#include <color.h>
#include <ScoreTracker.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "SpeedingListener", __VA_ARGS__))

using namespace WayRay;

class CheckSpeedingListener : public Listener<double> 
{
    public:
        double speedLimit;
        CheckSpeedingListener(&ScoreTracker tracker, double limit)
        {
            speedLimit = limit;
        }
        void CheckSpeedingListener::changed(const std::shared_ptr<double> speed)
        {
            if(*speed > speedLimit)
            {
                isSpeeding = true;
                LOGI("Speeding!! Current speed: " + *speed);
            }
            else if(*speed <= speedLimit && isSpeeding)
            {
                isSpeeding = false;
                LOGI("Below speed limit again.");
            }
        }
    private:
        bool isSpeeding = false;
}

