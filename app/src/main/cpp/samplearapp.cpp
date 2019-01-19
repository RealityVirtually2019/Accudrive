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

#define earthRadiusKm 6371.0

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "truear-detectionobject", __VA_ARGS__))


using namespace WayRay;

class DetectionObjectListener : public Listener<DetectionObject> {
public:
    DetectionObjectListener() {
    }
    double deg2rad(double deg){
        return (deg*M_PI/180);
    }

    double rad2deg(double deg){
        return (deg*M_PI/180);
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
        if (object->getType() == DetectionObject::Type::TrafficLight) {
            if(object->getState() == DetectionObject::TrafficLightState::Red){
                double dist = distanceEarth(object->getPose().latitude,object->getPose().longitude,Context::get()->getVehicleState().getPose().latitude,Context::get()->getVehicleState().getPose().longitude);
                //float distance_of_traffic_light_from_car = object->getDistance();
                double speed_of_car = Context::get()->getVehicleState().getSpeed();
                LOGI("Distance: %f",dist);
                if(dist<=0.5 && speed_of_car>=0.5){
                    LOGI("You crossed the red light.");
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
    }

    std::map<std::shared_ptr<DetectionObject>, std::shared_ptr<ARObject> > arrowMap;

};

class DetectionObjectSampleARApp: public Application {

    virtual void onStart() {
        detectionObjectListener = std::make_shared<DetectionObjectListener>();
        Context::get()->getScene().registerDetectionObjectListener(detectionObjectListener);
        LOGI("DetectionObjectListener registered.");
    }

    virtual void onStop() {
        Context::get()->getScene().unregisterDetectionObjectListener(detectionObjectListener);
        detectionObjectListener.reset();
        LOGI("DetectionObjectListener unregistered.");
    }
private:
    std::shared_ptr<DetectionObjectListener> detectionObjectListener;
};

extern "C" {
Application* truear_app_create() {
    return new DetectionObjectSampleARApp();
}
}