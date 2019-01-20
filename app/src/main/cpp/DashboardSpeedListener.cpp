//
// Created by Vamsi Batchu on 1/20/19.
//

#include "DashboardSpeedListener.h"
#include "SpeedCheck.h"

void DashboardSpeedListener::changed(const std::shared_ptr<double> speed) {
    if(*speed>2.0){
        RemoveDashboard();
    }
}

void DashboardSpeedListener::showDashboard(){
    if(Dashboard1== nullptr){
        std::shared_ptr<Mesh> arrowMesh = ResourceHelper::loadMesh("Dash1.obj");
        Dashboard1 = std::make_shared<ARObject>("Dashboard1",Context::get()->getScene().getCamera());
        Dashboard1->setMesh(arrowMesh);
        Dashboard1->setPose(Pose(0, 1.0, -10 ));
        Dashboard1->setRotation(std::array<float, 3>{{-90.0 ,180.0, 180.0}});
        Dashboard1->setScale(std::array<float, 3>{{1.5, 1.5, 1.5}});
        Dashboard1->setTexture(std::make_shared<Color>(Color::Palette::Red));
        Context::get()->getScene().add(Dashboard1);
        LOGI("Displayed Dash");
    }
}

void DashboardSpeedListener::RemoveDashboard(){
    if(Dashboard1!= nullptr){
        Context::get()->getScene().remove(Dashboard1);
        LOGI("RemovedDashboard");
        Dashboard1 = nullptr;
    }
}