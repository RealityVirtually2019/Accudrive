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

#define earthRadiusKm 6371.0



using namespace WayRay;

class DetectionObjectListener : public Listener<DetectionObject> {
public:


    std::shared_ptr<ARObject> arObject;


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
    void SetScoreTracker(std::shared_ptr<ScoreTracker> tracker)
    {
        scoreTracker = tracker;
    }

    void showStop(){
        if(arObject == nullptr){
            std::shared_ptr<Mesh> arrowMesh = ResourceHelper::loadMesh("correctedPakr2.obj");
            arObject = std::make_shared<ARObject>("arObject",Context::get()->getScene().getCamera());
            arObject->setMesh(arrowMesh);
            arObject->setPose(Pose(0, 0, -10 ));
            //arObject->setPose(Pose(1, 0, 0 ));
            arObject->setRotation(std::array<float, 3>{{90.0 ,0.0, 180.0}});
            arObject->setScale(std::array<float, 3>{{1.5, 1.5, 1.5}});
            arObject->setTexture(std::make_shared<Color>(Color::Palette::Red));
            Context::get()->getScene().add(arObject);
        }
        LOGI("Displayed");
    }

    void changed(std::shared_ptr<DetectionObject> object) {

        /* CHECK PARKING TIME
         * TIME FOR PARKING DETECTION: 30 SECONDS (CAN BE ADJUSTED ACCORDING TO REQUIREMENTS)
         */

        //int is3DwarningVisible=0;

        if(start_park_time>30 && !parkingFlag){
            LOGI("Your in parking mode");
            parkingFlag = true;
            scoreTracker->AddScore(PARKING);
        }


        /* FEATURE 1 : TRAFFIC LIGHT VIOLATION CODE START:
         *
         */
        double speed_of_car = 0;

        if (object->getType() == DetectionObject::Type::TrafficLight) {

            if(object->getState() == DetectionObject::TrafficLightState::Red){
                double dist = distanceEarth(object->getPose().latitude,object->getPose().longitude,Context::get()->getVehicleState().getPose().latitude,Context::get()->getVehicleState().getPose().longitude);
                //float distance_of_traffic_light_from_car = object->getDistance();
                speed_of_car = Context::get()->getVehicleState().getSpeed();
                //LOGI("Distance: %f",dist);
                if(dist<=0.5 && speed_of_car>=0.5 && canRunLight == true){
                    LOGI("You crossed the red light.");
                    //initHUDModel();
                    showStop();
                    scoreTracker->SubtractScore(TRAFFIC_SIGNAL);
                    canRunLight = false;
                }else if(speed_of_car<1.4){
                    if (arObject != nullptr) {
                        Context::get()->getScene().remove(arObject);
                        arObject = nullptr;
                    }
                }

                if (object->getStatus() == DetectionObject::Status::New || object->getStatus() == DetectionObject::Status::Changed || object->getStatus() == DetectionObject::Status::NotChanged) {
                    std::map<std::shared_ptr<DetectionObject>,std::shared_ptr<ARObject> >::iterator i = arrowMap.find(object);
                    if (i == arrowMap.end()) {
                        std::shared_ptr<Mesh> arrowMesh = ResourceHelper::loadMesh("correctedHadn.obj");
                        std::shared_ptr<ARObject> newArrow = std::make_shared<ARObject>(
                                object->getName() + "_arrow", object);
                        newArrow->setMesh(arrowMesh);
                        newArrow->setPose(Pose(0.0, 0.0, 1.5));
                        newArrow->setRotation(std::array<float, 3>{{0.0, 90.0, 90.0}});
                        newArrow->setScale(std::array<float, 3>{{0.25, 0.25, 0.25}});
                        newArrow->setTexture(std::make_shared<Color>(Color::RGBA((int8_t)255,(int8_t)165,(int8_t)2,(int8_t)255)));
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
                        if (arObject != nullptr) {
                            Context::get()->getScene().remove(arObject);
                            arObject = nullptr;
                        }
                    }
                    canRunLight = true;
                }
            }else if(object->getState() == DetectionObject::TrafficLightState::Yellow){
                std::map<std::shared_ptr<DetectionObject>,std::shared_ptr<ARObject> >::iterator i = arrowMap.find(object);
                if (i != arrowMap.end()) {
                    std::shared_ptr<ARObject> arrow = i->second;
                    arrow->setParent(nullptr);
                    Context::get()->getScene().remove(arrow);
                    arrowMap.erase(object);
                }
            }else{
                std::map<std::shared_ptr<DetectionObject>,std::shared_ptr<ARObject> >::iterator i = arrowMap.find(object);
                if (i != arrowMap.end()) {
                    std::shared_ptr<ARObject> arrow = i->second;
                    arrow->setParent(nullptr);
                    Context::get()->getScene().remove(arrow);
                    arrowMap.erase(object);
                }
                if (object->getStatus() == DetectionObject::Status::New || object->getStatus() == DetectionObject::Status::Changed || object->getStatus() == DetectionObject::Status::NotChanged) {
                    std::map<std::shared_ptr<DetectionObject>,std::shared_ptr<ARObject> >::iterator i = arrowMap.find(object);
                    if (i == arrowMap.end()) {
                        std::shared_ptr<Mesh> arrowMesh = ResourceHelper::loadMesh("correctedPakr4.obj");
                        std::shared_ptr<ARObject> newArrow = std::make_shared<ARObject>(
                                object->getName() + "_arrow", object);
                        newArrow->setMesh(arrowMesh);
                        newArrow->setPose(Pose(0.0, 0.0, 1.5));
                        newArrow->setRotation(std::array<float, 3>{{0.0, 90.0, 90.0}});
                        newArrow->setScale(std::array<float, 3>{{0.25, 0.25, 0.25}});
                        newArrow->setTexture(std::make_shared<Color>(Color::RGBA((int8_t)106,(int8_t)176,(int8_t)76,(int8_t)255)));
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
                        if (arObject != nullptr) {
                            Context::get()->getScene().remove(arObject);
                            arObject = nullptr;
                        }
                    }
                    canRunLight = true;
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
        }else if (parkingFlag == true &&  Context::get()->getVehicleState().getSpeed() > 0){
            start_park_time=0;
            parkingFlag = false;
        }

        /*
         PARKING LOGIC ENDS
         */


        if (object->getType() == DetectionObject::Type::ParkingPlace) {
                //double dist = distanceEarth(object->getPose().latitude,object->getPose().longitude,Context::get()->getVehicleState().getPose().latitude,Context::get()->getVehicleState().getPose().longitude);
                if (object->getStatus() == DetectionObject::Status::New || object->getStatus() == DetectionObject::Status::Changed || object->getStatus() == DetectionObject::Status::NotChanged) {
                    std::map<std::shared_ptr<DetectionObject>,std::shared_ptr<ARObject> >::iterator i = arrowMap.find(object);
                    if (i == arrowMap.end()) {
                        std::shared_ptr<Mesh> arrowMesh = ResourceHelper::loadMesh("correctedPakr1.obj");
                        std::shared_ptr<ARObject> newArrow = std::make_shared<ARObject>(
                                object->getName() + "_arrow", object);
                        newArrow->setMesh(arrowMesh);
                        newArrow->setPose(Pose(0.0, 0.0, 2.5));
                        newArrow->setRotation(std::array<float, 3>{{0.0, 90, 90.0}});
                        newArrow->setScale(std::array<float, 3>{{1.25, 1.25, 1.25}});
                        newArrow->setTexture(std::make_shared<Color>(Color::RGBA((int8_t)255,(int8_t)255,(int8_t)255,(int8_t)255)));
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
        }
//        if(speed_of_car<1.4){
//            if (arObject != nullptr) {
//                Context::get()->getScene().remove(arObject);
//                arObject = nullptr;
//            }
//        }
    }

private:
    std::map<std::shared_ptr<DetectionObject>, std::shared_ptr<ARObject> > arrowMap;
    bool canRunLight = false;
    bool parkingFlag = false;
    std::shared_ptr<ScoreTracker> scoreTracker;
};

class DetectionObjectSampleARApp: public Application {


    virtual void onStart() {
        detectionObjectListener = std::make_shared<DetectionObjectListener>();
        speedCheckListener = std::make_shared<CheckSpeedingListener>();
        scoreTracker = std::make_shared<ScoreTracker>();
        detectionObjectListener->SetScoreTracker(scoreTracker);
        speedCheckListener->SetScoreTracker(scoreTracker);
        Context::get()->getScene().registerDetectionObjectListener(detectionObjectListener);
        Context::get()->getVehicleState().registerSpeedChangeListener(speedCheckListener);
        LOGI("ON START COMPLETE");
    }

    virtual void onStop() {
        Context::get()->getScene().unregisterDetectionObjectListener(detectionObjectListener);
        Context::get()->getVehicleState().unregisterSpeedChangeListener(speedCheckListener);
        scoreTracker->SaveReport();
        LOGI("FINAL SCORE: %f", scoreTracker->CalcTotalScore());
        detectionObjectListener.reset();
        speedCheckListener.reset();
        scoreTracker.reset();
        LOGI("ON STOP COMPLETE");
    }
private:
    std::shared_ptr<DetectionObjectListener> detectionObjectListener;
    std::shared_ptr<CheckSpeedingListener> speedCheckListener;
    std::shared_ptr<ScoreTracker> scoreTracker;
};

extern "C" {
Application* truear_app_create() {
    return new DetectionObjectSampleARApp();
}
}