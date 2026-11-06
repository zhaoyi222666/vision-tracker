#include "SpinningTarget.hpp"
namespace rm
{

    SpinningTarget::SpinningTarget(int id, const Observation &observation, std::int64_t timestamp_ns)
        : TrackingBase(id, observation.type, observation.x, observation.y, observation.angle, timestamp_ns)
    {
    }

}   