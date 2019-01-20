
#ifndef ACCUDRIVE_DETECTION_OBJECT_LISTENER
#define ACCUDRIVE_DETECTION_OBJECT_LISTENER

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

#define earthRadiusKm 6371.0

using namespace WayRay;


class DetectionObjectListener : public Listener<DetectionObject> {
public:

    std::shared_ptr<ARObject> arObject;
    std::shared_ptr<ARObject> parkingObject;
    bool canRunLight = false;
    bool parkingFlag = false;
    /*
     * PARKING VARIABLES
     */
    time_t start_park_time;

    // Time checker
    time_t my_time;


    DetectionObjectListener();
    double deg2rad(double deg){
        return (deg*M_PI/180);
    }

    double rad2deg(double deg){
        return (deg*M_PI/180);
    }

    static double now_ms(void) {
        struct timespec res;
        clock_gettime(CLOCK_REALTIME, &res);
        return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
    }

    double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
        double lat1r, lon1r, lat2r, lon2r, u, v;
        lat1r = deg2rad(lat1d);
        lon1r = deg2rad(lon1d);
        lat2r = deg2rad(lat2d);
        lon2r = deg2rad(lon2d);
        u = sin((lat2r - lat1r) / 2);
        v = sin((lon2r - lon1r) / 2);
        return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
    }
    void SetScoreTracker(std::shared_ptr<ScoreTracker> tracker)
    {
        scoreTracker = tracker;
    }

    void showStop();


    void showParking();


    void RemoveParking();


    void changed(std::shared_ptr<DetectionObject> object);

private:
    std::map<std::shared_ptr<DetectionObject>, std::shared_ptr<ARObject> > arrowMap;
    std::shared_ptr<ScoreTracker> scoreTracker;
};

#endif