#ifndef SCREENVIEW_HPP
#define SCREENVIEW_HPP

#include <gui_generated/screen_screen/screenViewBase.hpp>
#include <gui/screen_screen/screenPresenter.hpp>

class screenView : public screenViewBase
{
public:
    screenView();
    virtual ~screenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    void setSpeedText(int value);
    void setThermaticText(float value);
    void setHumidityText(int value);
    void setTemperatureText(int value);
    void setSteering(int value);
    void setDistance(int value);
    void setAirbag(uint8_t active);
    virtual void triggerCall();
    virtual void triggerHangup();
protected:
};

#endif // SCREENVIEW_HPP
