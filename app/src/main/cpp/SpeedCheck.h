//
// Created by Niveditha G on 18/01/19.
//

#ifndef DETECTIONOBJECTSAMPLE_SPEEDCHECK_H
#define DETECTIONOBJECTSAMPLE_SPEEDCHECK_H

#include <context.h>
#include <application.h>
#include <android/sensor.h>
#include <android/log.h>
#include <utilities.h>
#include <detectionobject.h>
#include <color.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "truear-detectionobject", __VA_ARGS__))

using namespace WayRay;

class CheckSpeedingListener : public Listener<double> {
public:
    double speedLimit;

    CheckSpeedingListener(double limit);
    void changed(const std::shared_ptr<double> speed);

private:
    bool isSpeeding = false;

};


#endif //DETECTIONOBJECTSAMPLE_SPEEDCHECK_H
