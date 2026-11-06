#include "TargetManager.hpp"
#include "ArmorTarget.hpp"
#include "SpinningTarget.hpp"
namespace rm
{
    static bool wasObserved(const InputFrame &frame, int id)
    {
        for (const Observation &observation: frame.observations)
        {
            if (observation.id == id)
            {
                return true;
            }
        }
        return false;
    }
    TargetManager::~TargetManager()
    {
        for (TrackingBase *target: targets_)
        {
            delete target;
        }
    }
    Result TargetManager::onFrame(const InputFrame &frame)
    {
        if (!checkFrame(frame))
        {
            return makeResult(frame.frame_id, false);
        }
        double dt=0.0;
        if (has_last)
        {
            dt=TrackingBase::secondsBetween(last_timestamp_ns, frame.timestamp_ns);
        }
        updateTargets(frame, dt);
        has_last=true;
        last_frame_id=frame.frame_id;
        last_timestamp_ns=frame.timestamp_ns;
        return makeResult(frame.frame_id, true);
    }
    bool TargetManager::checkFrame(const InputFrame &frame) const
    {
        if (has_last)
        {
            if (frame.frame_id <= last_frame_id)
            {
                return false;
            }
            if (frame.timestamp_ns <= last_timestamp_ns)
            {
                return false;
            }
        }
        
        
            
           const std::vector<Observation> &observations=frame.observations;
            for (std::size_t i = 0; i < observations.size(); ++i)
            {
                if(observations[i].id < 0)
                {
                    return false;
                }
                for (std::size_t j = i + 1; j < observations.size(); ++j)
                {
                    if (observations[i].id == observations[j].id )
                    {
                        return false;
                    }
                }
                for (const TrackingBase *target : targets_)
                {
                if (target->id() == observations[i].id && target->type() != observations[i].type)
                {
                    return false;
                }
                }
       
            }
            return true;
    }
    void TargetManager::updateTargets(const InputFrame &frame, double dt)
    {
        for (const Observation &observation: frame.observations)
        {
           TrackingBase *target = findTarget(observation.id);
           if(target==nullptr)
           {
            if (observation.type==TargetType::SPINNING)
            {
                targets_.push_back(new SpinningTarget(observation.id, observation, frame.timestamp_ns));
            }
            else
            {
                targets_.push_back(new ArmorTarget(observation.id, observation, frame.timestamp_ns));
            }
          }
          else
          {
            target->updateObserved(observation, frame.timestamp_ns, dt);
          }
        }
        for(TrackingBase *target: targets_)
        {
            if (wasObserved(frame, target->id()))
            {
               continue;
            }
            if(target->status()==TargetStatus::TRACK)
            {
                target->updateUnobserved(dt);
            }
           
            }
        }
        TrackingBase *TargetManager::findTarget(int id)
{
    for(TrackingBase *target: targets_)
    {
        if (target->id() == id)
        {
            return target;
        }
    }
    return nullptr; 
}
Result TargetManager::makeResult(int frame_id,bool accepted) const
{
    Result result{};
    result.accepted=accepted;
    result.frame_id=frame_id;
    for (const TrackingBase *target: targets_)
    {
        result.targets.push_back(target->snapshot());
        if (target->status() == TargetStatus::TRACK)
        {
            ++result.tracking_target_count;
        }
        else if (target->status() == TargetStatus::LOST)
        {
            ++result.lost_target_count;
        }
        if (target->type() == TargetType::ARMOR)
        {
            ++result.armor_target_count;
        }
        else if (target->type() == TargetType::SPINNING)
        {
            ++result.spinning_target_count;
        }
    }
    return result;
}
}