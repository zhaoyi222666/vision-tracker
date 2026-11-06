#include "ArmorTarget.hpp"
namespace rm
{

    ArmorTarget::ArmorTarget(int id, const Observation &observation, std::int64_t timestamp_ns)
        : TrackingBase(id, observation.type, observation.x, observation.y, 0.0, timestamp_ns)
    {
    }

}