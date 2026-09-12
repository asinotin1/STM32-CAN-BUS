#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>
#include <stdint.h> 
class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    virtual void updateBodySpeed(float v) {}
    virtual void updateTailEngineTemp(float v) {}
    virtual void updateBodyHum(float v) {}
    virtual void updateBodyTemp(float v) {}
    virtual void updateBodyAirbag(uint8_t v) {}
    virtual void updateTailSteering(float v) {}
    virtual void updateTailDistance(float v) {}
    void bind(Model* m)
    {
        model = m;
    }
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
