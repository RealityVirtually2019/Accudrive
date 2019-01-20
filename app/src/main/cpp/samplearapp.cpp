#include <context.h>
#include <string>
#include <application.h>
#include <android/sensor.h>
#include <android/log.h>
#include <utilities.h>
#include <detectionobject.h>
#include <color.h>
#include <map>
#include <cmath>
#include <vector>
#include <time.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>


#include "SpeedCheck.h"
#include "ScoreTracker.h"
#include "DashboardSpeedListener.h"
#include "RoadParkingListener.h"
#include "DetectionObjectListener.h"

#define earthRadiusKm 6371.0

using namespace WayRay;

class DetectionObjectSampleARApp: public Application {

    virtual void onStart() {

        //showDashboard();
        detectionObjectListener = std::make_shared<DetectionObjectListener>();
        speedCheckListener = std::make_shared<CheckSpeedingListener>();
        dashboardSpeedListener = std::make_shared<DashboardSpeedListener>();
        roadParkingListener = std::make_shared<RoadParkingListener>();
        scoreTracker = std::make_shared<ScoreTracker>();
        detectionObjectListener->SetScoreTracker(scoreTracker);
        speedCheckListener->SetScoreTracker(scoreTracker);
        roadParkingListener->SetParkingDetectionListener(detectionObjectListener);
        roadParkingListener->SetScoreTracker(scoreTracker);
        roadParkingListener->SetParkingObject(detectionObjectListener->parkingObject);
        roadParkingListener->SetMinusTen(detectionObjectListener->minusten);
        Context::get()->getScene().registerDetectionObjectListener(detectionObjectListener);
        Context::get()->getVehicleState().registerSpeedChangeListener(speedCheckListener);
        Context::get()->getVehicleState().registerSpeedChangeListener(dashboardSpeedListener);
        Context::get()->getVehicleState().registerSpeedChangeListener(roadParkingListener);
        dashboardSpeedListener->showDashboard();
        LOGI("ON START COMPLETE");
    }

    virtual void onStop() {
        Context::get()->getScene().unregisterDetectionObjectListener(detectionObjectListener);
        Context::get()->getVehicleState().unregisterSpeedChangeListener(speedCheckListener);
        Context::get()->getVehicleState().unregisterSpeedChangeListener(dashboardSpeedListener);
        Context::get()->getVehicleState().unregisterSpeedChangeListener(roadParkingListener);
        scoreTracker->SaveReport();
        LOGI("FINAL SCORE: %f", scoreTracker->CalcTotalScore());
        detectionObjectListener.reset();
        speedCheckListener.reset();
        dashboardSpeedListener.reset();
        roadParkingListener.reset();
        scoreTracker.reset();
        LOGI("ON STOP COMPLETE");
    }
private:
    std::shared_ptr<DetectionObjectListener> detectionObjectListener;
    std::shared_ptr<CheckSpeedingListener> speedCheckListener;
    std::shared_ptr<DashboardSpeedListener> dashboardSpeedListener;
    std::shared_ptr<RoadParkingListener> roadParkingListener;
    std::shared_ptr<ScoreTracker> scoreTracker;
};

extern "C" {
Application* truear_app_create() {
    return new DetectionObjectSampleARApp();
}
}