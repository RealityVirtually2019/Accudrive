//
// Created by Vamsi Batchu on 1/20/19.
//

#ifndef ACCUDRIVE_DASHBOARDSPEEDLISTENER_H
#define ACCUDRIVE_DASHBOARDSPEEDLISTENER_H
#include <context.h>
#include <application.h>
#include <android/sensor.h>
#include <android/log.h>
#include <utilities.h>
#include <detectionobject.h>
#include <color.h>

#include "ScoreTracker.h"

using namespace WayRay;

class DashboardSpeedListener : public Listener<double>{
public:
    void changed(const std::shared_ptr<double> speed);
    void showDashboard();
    void RemoveDashboard();
private:
    std::shared_ptr<ARObject> Dashboard1;
    std::shared_ptr<ARObject> Dashboard3;
};


#endif //ACCUDRIVE_DASHBOARDSPEEDLISTENER_H
