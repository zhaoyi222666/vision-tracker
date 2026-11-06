#pragma once
#include "TrackingBase.hpp"
#include "data_types.hpp"
#include <vector>
#include <cstdint>
#include <cstddef>
namespace rm
{
    class TargetManager
    {
    public:
        TargetManager() = default;
        ~TargetManager() ;

       Result onFrame(const InputFrame &frame);
       

    private:
       bool checkFrame(const InputFrame &frame) const;
       void updateTargets(const InputFrame &frame, double dt);
        TrackingBase* findTarget(int id);
        Result makeResult(int frame_id,bool accepted) const;
        std::vector<TrackingBase*> targets_;
        bool has_last=false;
        int last_frame_id=0;
        std::int64_t last_timestamp_ns=0;


    };
}
