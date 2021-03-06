#ifndef ACCUDRIVE_ROAD_PARKING_H
#define ACCUDRIVE_ROAD_PARKING_H

#include <context.h>
#include <application.h>
#include <android/sensor.h>
#include <android/log.h>
#include <utilities.h>
#include <detectionobject.h>
#include <color.h>
#include <chrono>
#include <ctime>
#include <thread>

#include "DetectionObjectListener.h"
#include "ScoreTracker.h"
#include "SpeedCheck.h"

using namespace WayRay;

class RoadParkingListener : public Listener<double> {
public:
    RoadParkingListener();
    void changed(const std::shared_ptr<double> speed);
    void SetScoreTracker(std::shared_ptr<ScoreTracker> tracker);
    void SetParkingDetectionListener(std::shared_ptr<DetectionObjectListener> pListener);
    void SetParkingObject(std::shared_ptr<ARObject> parkingObj);
    void SetMinusTen(std::shared_ptr<ARObject> mten);
    void RemoveParking();
    void ShowParking();
    void showMinus();

    void removeMinus10();
    std::shared_ptr<ARObject> parkingObject;
    std::shared_ptr<ARObject> minusten;
private:
    bool isParked = true;
    double parkTimeLimit = 15.0;
    time_t start_park_time;
    std::shared_ptr<DetectionObjectListener> parkingListener;
    std::shared_ptr<ScoreTracker> scoreTracker;
};


#endif //ACCUDRIVE_ROAD_PARKING_H
