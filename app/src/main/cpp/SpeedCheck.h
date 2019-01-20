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

#include "ScoreTracker.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "truear-accudrive", __VA_ARGS__))

using namespace WayRay;

class CheckSpeedingListener : public Listener<double> {
public:
    double speedLimit;
    CheckSpeedingListener();
    void changed(const std::shared_ptr<double> speed);
    void SetScoreTracker(std::shared_ptr<ScoreTracker> tracker);
private:
    bool isSpeeding = false;
    std::shared_ptr<ScoreTracker> scoreTracker;
};


#endif //DETECTIONOBJECTSAMPLE_SPEEDCHECK_H
