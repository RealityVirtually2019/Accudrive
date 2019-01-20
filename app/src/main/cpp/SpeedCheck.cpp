#include "SpeedCheck.h"


CheckSpeedingListener::CheckSpeedingListener()
{
    speedLimit = KiloPerHourToMetersPerSecond(50.0);
    LOGI("Speed Limit Registered: %f", speedLimit);
}

void CheckSpeedingListener::changed(const std::shared_ptr<double> speed) {

    LOGI("Current speed: %f Speed Limit:%f IsSpeeding:%d", *speed, speedLimit,isSpeeding);
    if(*speed > speedLimit && isSpeeding == false)
    {
        isSpeeding = true;
        LOGI("Speeding!! Current speed: %f", *speed);
        scoreTracker->SubtractScore(SPEEDING);
        showStop();
    }
    else if(*speed <= speedLimit && isSpeeding)
    {
        isSpeeding = false;
        LOGI("Below speed limit again.");
        if (arObject != nullptr) {
            Context::get()->getScene().remove(arObject);
            arObject = nullptr;
        }
    }
}
void CheckSpeedingListener::SetScoreTracker(std::shared_ptr<ScoreTracker> tracker) {
    scoreTracker = tracker;
}
double CheckSpeedingListener::KiloPerHourToMetersPerSecond(double kph)
{
    return kph * 1000/3600;
}


void CheckSpeedingListener::showStop(){
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



