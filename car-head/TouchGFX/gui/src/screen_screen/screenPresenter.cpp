#include <gui/screen_screen/screenView.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

screenPresenter::screenPresenter(screenView& v)
    : view(v)
{

}

void screenPresenter::activate()
{

}

void screenPresenter::deactivate()
{

}
void screenPresenter::updateBodySpeed(float v){ 
    view.setSpeedText(v); 
}
void screenPresenter::updateTailEngineTemp(float v){ 
    view.setThermaticText(v);
}
void screenPresenter::updateBodyHum(float v){
    view.setHumidityText(v);
}
void screenPresenter::updateBodyTemp(float v){
    view.setTemperatureText(v); 
}
void screenPresenter::updateBodyAirbag(uint8_t v){ 
    view.setAirbag(v);
}
void screenPresenter::updateTailSteering(float v){
     view.setSteering(v); 
    }
void screenPresenter::updateTailDistance(float v){
     view.setDistance(v); 
    }