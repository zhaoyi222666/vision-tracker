#pragma once
#include "TrackingBase.hpp"
namespace rm
{
    class SpinningTarget : public TrackingBase
    {
        public:
        SpinningTarget(int id, const Observation &observation, std::int64_t timestamp_ns);
        int lostThreshold() const override { return 5; }
        bool tracksAngle() const override { return true; }
    };
}