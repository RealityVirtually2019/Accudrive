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


#include "SpeedCheck.h"

#define earthRadiusKm 6371.0



using namespace WayRay;

class DetectionObjectListener : public Listener<DetectionObject> {
public:

    /*
     * PARKING VARIABLES
     */
    int start_park_time = 0;

    // Time checker
    time_t my_time;


    DetectionObjectListener() {
    }
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

    void changed(std::shared_ptr<DetectionObject> object) {

        /* CHECK PARKING TIME
         * TIME FOR PARKING DETECTION: 30 SECONDS (CAN BE ADJUSTED ACCORDING TO REQUIREMENTS)
         */

        if(start_park_time>30){
            LOGI("Your in parking mode");
        }

        /* FEATURE 1 : TRAFFIC LIGHT VIOLATION CODE START:
         *
         */

        if (object->getType() == DetectionObject::Type::TrafficLight) {
            if(object->getState() == DetectionObject::TrafficLightState::Red){
                double dist = distanceEarth(object->getPose().latitude,object->getPose().longitude,Context::get()->getVehicleState().getPose().latitude,Context::get()->getVehicleState().getPose().longitude);
                //float distance_of_traffic_light_from_car = object->getDistance();
                double speed_of_car = Context::get()->getVehicleState().getSpeed();
                // LOGI("Distance: %f",dist);
                if(dist<=0.5 && speed_of_car>=0.5){
                    //LOGI("You crossed the red light.");
                }
                if (object->getStatus() == DetectionObject::Status::New || object->getStatus() == DetectionObject::Status::Changed || object->getStatus() == DetectionObject::Status::NotChanged) {
                    std::map<std::shared_ptr<DetectionObject>,std::shared_ptr<ARObject> >::iterator i = arrowMap.find(object);
                    if (i == arrowMap.end()) {
                        std::shared_ptr<Mesh> arrowMesh = ResourceHelper::loadMesh("arrow.obj");
                        std::shared_ptr<ARObject> newArrow = std::make_shared<ARObject>(
                                object->getName() + "_arrow", object);
                        newArrow->setMesh(arrowMesh);
                        newArrow->setPose(Pose(0.0, 0.0, 1.5));
                        newArrow->setRotation(std::array<float, 3>{{90.0, 0.0, 270.0}});
                        newArrow->setScale(std::array<float, 3>{{1.5, 1.5, 1.5}});
                        newArrow->setTexture(std::make_shared<Color>(Color::Palette::Green));
                        arrowMap[object] = newArrow;
                        Context::get()->getScene().add(newArrow);
                    }
                }else if (object->getStatus() == DetectionObject::Status::Disappeared) {
                    std::map<std::shared_ptr<DetectionObject>,std::shared_ptr<ARObject> >::iterator i = arrowMap.find(object);
                    if (i != arrowMap.end()) {
                        std::shared_ptr<ARObject> arrow = i->second;
                        arrow->setParent(nullptr);
                        Context::get()->getScene().remove(arrow);
                        arrowMap.erase(object);
                    }
                }
            }else{
                std::map<std::shared_ptr<DetectionObject>,std::shared_ptr<ARObject> >::iterator i = arrowMap.find(object);
                if (i != arrowMap.end()) {
                    std::shared_ptr<ARObject> arrow = i->second;
                    arrow->setParent(nullptr);
                    Context::get()->getScene().remove(arrow);
                    arrowMap.erase(object);
                }
            }

        }

        /*
         FEATURE 2 : PARKING VIOLATION LOGIC
         */

        if (object->getType() != DetectionObject::Type::ParkingPlace && Context::get()->getVehicleState().getSpeed()==0){
            my_time = time(NULL);
            char* current_time = ctime(&my_time);
            std::string str(current_time);
            std::string seconds = str.substr(17,2);
            start_park_time = std::stoi(seconds,nullptr,10);
            //LOGI("%d", sec);
        }else{
            start_park_time=0;
        }

        /*
         PARKING LOGIC ENDS
         */
    }
    std::map<std::shared_ptr<DetectionObject>, std::shared_ptr<ARObject> > arrowMap;

};

class DetectionObjectSampleARApp: public Application {

    virtual void onStart() {
        detectionObjectListener = std::make_shared<DetectionObjectListener>();
        std::shared_ptr<CheckSpeedingListener> speedCheckListener = std::make_shared<CheckSpeedingListener>(CheckSpeedingListener(50));
        Context::get()->getScene().registerDetectionObjectListener(detectionObjectListener);
        Context::get()->getVehicleState().registerSpeedChangeListener(speedCheckListener);
        //LOGI("DetectionObjectListener registered.");
    }

    virtual void onStop() {
        Context::get()->getScene().unregisterDetectionObjectListener(detectionObjectListener);
        detectionObjectListener.reset();
        //LOGI("DetectionObjectListener unregistered.");
    }
private:
    std::shared_ptr<DetectionObjectListener> detectionObjectListener;
};

extern "C" {
Application* truear_app_create() {
    return new DetectionObjectSampleARApp();
}
}