#include "TrackingBase.hpp"
#pragma once
namespace rm
{

    class ArmorTarget : public TrackingBase
    {
    public:
        ArmorTarget(int id, const Observation &observation, std::int64_t timestamp_ns);
            
        int lostThreshold() const override { return 3; }  
        bool tracksAngle() const override { return false; }  
    };

}