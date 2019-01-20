#include "RoadParkingListener.h"

RoadParkingListener::RoadParkingListener()
{
    start_park_time = time(NULL);
}
void RoadParkingListener::changed(const std::shared_ptr<double> speed)
{
    if(*speed > 0.0)
    {
        start_park_time = time(NULL);
        isParked = false;
        RemoveParking();
        removeMinus10();
    }
    else if(*speed == 0)
    {
        time_t currentTime = time(NULL);
        double parkTime = difftime(currentTime, start_park_time);
        LOGI("current park time: %f", parkTime);
        if(parkTime >= parkTimeLimit && !isParked && !parkingListener->parkingFlag)
        {
            isParked = true;
            scoreTracker->SubtractScore(PARKING);
            LOGI("You are parked in the middle of the road!");
            ShowParking();
            showMinus();
        }
    }
}



void RoadParkingListener::ShowParking()
{
    if(parkingObject == nullptr){
        std::shared_ptr<Mesh> arrowMesh = ResourceHelper::loadMesh("correctedPakr5.obj");
        parkingObject = std::make_shared<ARObject>("parkingObject",Context::get()->getScene().getCamera());
        parkingObject->setMesh(arrowMesh);
        parkingObject->setPose(Pose(0, 0.5, -10 ));
        parkingObject->setRotation(std::array<float, 3>{{90.0 ,0.0, 180.0}});
        parkingObject->setScale(std::array<float, 3>{{0.5, 0.5, 0.5}});
        parkingObject->setTexture(std::make_shared<Color>(Color::RGBA((int8_t)192,(int8_t)57,(int8_t)43,(int8_t)255)));
        Context::get()->getScene().add(parkingObject);
    }
    LOGI("Displayed");
}

void RoadParkingListener::RemoveParking()
{
    if(parkingObject!= nullptr){
        Context::get()->getScene().remove(parkingObject);
        LOGI("PARKING OBJECT IS NULL");
        parkingObject = nullptr;
    }
}
void RoadParkingListener::SetParkingObject(std::shared_ptr<ARObject> parkingObj)
{
    parkingObject = parkingObj;
}
void RoadParkingListener::SetMinusTen(std::shared_ptr<ARObject> mten)
{
    minusten = mten;
}

void RoadParkingListener::SetScoreTracker(std::shared_ptr<ScoreTracker> tracker)
{
    scoreTracker = tracker;
}
void RoadParkingListener::SetParkingDetectionListener(std::shared_ptr<DetectionObjectListener> pListener)
{
    parkingListener = pListener;
}
void RoadParkingListener::showMinus(){
    if(minusten == nullptr){
        std::shared_ptr<Mesh> arrowMesh = ResourceHelper::loadMesh("MinusScore.obj");
        minusten = std::make_shared<ARObject>("minusten",Context::get()->getScene().getCamera());
        minusten->setMesh(arrowMesh);
        minusten->setPose(Pose(0, -0.75, -10 ));
        //arObject->setPose(Pose(1, 0, 0 ));
        minusten->setRotation(std::array<float, 3>{{90.0 ,0.0, 90.0}});
        minusten->setScale(std::array<float, 3>{{0.5, 0.5, 0.5}});
        minusten->setTexture(std::make_shared<Color>(Color::Palette::Red));
        Context::get()->getScene().add(minusten);
    }
    LOGI("minusten");
}

void RoadParkingListener::removeMinus10(){
    if(minusten!= nullptr){
        Context::get()->getScene().remove(minusten);
        LOGI("PARKING OBJECT IS NULL");
        minusten = nullptr;
    }
}