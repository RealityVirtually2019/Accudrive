#include "DetectionObjectListener.h"

DetectionObjectListener::DetectionObjectListener() {}
void DetectionObjectListener::showStop()
{
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

void DetectionObjectListener::showParking()
{
    if(parkingObject == nullptr){
        std::shared_ptr<Mesh> arrowMesh = ResourceHelper::loadMesh("correctedPakr5.obj");
        parkingObject = std::make_shared<ARObject>("parkingObject",Context::get()->getScene().getCamera());
        parkingObject->setMesh(arrowMesh);
        parkingObject->setPose(Pose(0, 0.5, -10 ));
        parkingObject->setRotation(std::array<float, 3>{{90.0 ,0.0, 180.0}});
        parkingObject->setScale(std::array<float, 3>{{0.5, 0.5, 0.5}});
        parkingObject->setTexture(std::make_shared<Color>(Color::Palette::Red));
        Context::get()->getScene().add(parkingObject);
    }
    LOGI("Displayed");
}

void DetectionObjectListener::showMinus(){
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

void DetectionObjectListener::removeMinus10(){
        if(minusten!= nullptr){
            Context::get()->getScene().remove(minusten);
            LOGI("PARKING OBJECT IS NULL");
            minusten = nullptr;
        }
}

void DetectionObjectListener::RemoveParking()
{
    if(parkingObject!= nullptr){
        Context::get()->getScene().remove(parkingObject);
        LOGI("PARKING OBJECT IS NULL");
        parkingObject = nullptr;
    }
}
void DetectionObjectListener::changed(std::shared_ptr<DetectionObject> object) 
{

    /* CHECK PARKING TIME
        * TIME FOR PARKING DETECTION: 30 SECONDS (CAN BE ADJUSTED ACCORDING TO REQUIREMENTS)
        */

    //int is 3D warning Visible=0;



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
    else if(object->getType() == DetectionObject::Type::ParkingPlace) {
        /*
            FEATURE 2 : VALID PARKING LOGIC
            */
        double parkDist = distanceEarth(object->getPose().latitude, object->getPose().longitude, Context::get()->getVehicleState().getPose().latitude, Context::get()->getVehicleState().getPose().longitude);
        if (Context::get()->getVehicleState().getSpeed() > 0) {
            start_park_time = time(NULL);
            parkingFlag = false;
            RemoveParking();
            removeMinus10();
            //char* current_time = ctime(&my_time);
            //std::string str(current_time);
            //std::string seconds = str.substr(17,2);
            // = std::stoi(seconds,nullptr,10);
            //LOGI("%d", sec);
        } else if (Context::get()->getVehicleState().getSpeed() == 0) {
            time_t currentTime = time(NULL);
            double parkTime = difftime(currentTime, start_park_time);
            LOGI("Current Park Time: %f", parkTime);
            if (parkTime > 10 && parkingFlag == false)
            {
                parkingFlag = true;
                if(parkDist < 0.018)
                {
                    scoreTracker->AddScore(PARKING);
                    LOGI("You are in good parking mode. Distance: %f", parkDist);
                }
                else {
                    scoreTracker->SubtractScore(PARKING);
                    LOGI("You are in bad parking mode. Distance: %f", parkDist);
                    showParking();
                    showMinus();
                }
            }
        }
    }
    //else
    //{
    //double parkDist = distanceEarth(object->getPose().latitude, object->getPose().longitude, Context::get()->getVehicleState().getPose().latitude, Context::get()->getVehicleState().getPose().longitude);

    //}
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
}