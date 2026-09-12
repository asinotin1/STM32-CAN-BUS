// TouchGFX/model/Model.cpp
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern "C" {
    #include "data.h"   
}

Model::Model() : modelListener(0)
{
}

void Model::tick()
{
    if (can_data.check_data_update == 0)   // == 0 nghĩa là vừa có dữ liệu mới
    {
        can_data.check_data_update = 1;    

        if (modelListener)
        {
            modelListener->updateBodySpeed(can_data.bodySpeed);
            modelListener->updateTailEngineTemp(can_data.tailEngineTemp);
            modelListener->updateBodyHum(can_data.bodyHum);
            modelListener->updateBodyTemp(can_data.bodyTemp);
            modelListener->updateBodyAirbag(can_data.bodyAirbag);
            modelListener->updateTailSteering(can_data.tailSteering);
            modelListener->updateTailDistance(can_data.tailDistance);
        }
    }
}