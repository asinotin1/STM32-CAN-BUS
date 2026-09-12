#include <gui/screen_screen/screenView.hpp>
#include <stdint.h>
#include <stdio.h>
#include <images/BitmapDatabase.hpp> 
extern "C"
{
    #include "ec800m.h"
}

screenView::screenView()
{

}

void screenView::setupScreen()
{
    screenViewBase::setupScreen();
   
}

void screenView::tearDownScreen()
{
    screenViewBase::tearDownScreen();
}

void screenView::handleTickEvent()
{
    screenViewBase::handleTickEvent();

   
}
void screenView::setSpeedText(int value)
{
    Unicode::snprintf(speedBuffer, SPEED_SIZE, "%d", value);
    speed.invalidate();
}

void screenView::setHumidityText(int value)
{
    Unicode::snprintf(humidityBuffer, HUMIDITY_SIZE, "%d", value);
    humidity.invalidate();
}

void screenView::setThermaticText(float value)
{
    int ther_int = (int)value;
    int ther_dec  = (int)((value - ther_int) * 10  );   // vd : 23.6 - 23 = 0.6 * 10 = 6 
    Unicode::snprintf(thermaticBuffer, THERMATIC_SIZE, "%d.%d", ther_int, ther_dec);
    thermatic.invalidate();
}

void screenView::setTemperatureText(int value)
{
    Unicode::snprintf(temperatureBuffer, TEMPERATURE_SIZE, "%d", value);   
    temperature.invalidate();
}
void screenView::setSteering(int value)
{
    left.setBitmap(touchgfx::Bitmap(value < 14 ? BITMAP_LEFTRED_ID : BITMAP_LEFTWHITE_ID));
    left.invalidate();
    right.setBitmap(touchgfx::Bitmap(value > 16 ? BITMAP_RIGHTRED_ID : BITMAP_RIGHTWHITE_ID));
    right.invalidate();
}
void screenView::setDistance(int value)
{
    waring.setBitmap(touchgfx::Bitmap(value < 20 ? BITMAP_WARINGRED_ID : BITMAP_WARINGWHITE_ID));
    waring.invalidate();
}
void screenView::setAirbag(uint8_t active)
{
    tuikhi.setBitmap(touchgfx::Bitmap(active ? BITMAP_TUIKHICOLOR_ID : BITMAP_TUIKHIWHITE_ID));
    tuikhi.invalidate();
}
void screenView::triggerCall()
{
    EC800M_Call();
}

void screenView::triggerHangup()
{
    EC800M_HangUp();
}