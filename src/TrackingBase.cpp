#include "TrackingBase.hpp"
#include "data_types.hpp"

namespace rm
{

    
    TrackingBase::TrackingBase(int id, TargetType type, double x, double y, double angle, std::int64_t timestamp_ns)
        : id_(id),
          type_(type),
          status_(TargetStatus::TRACK),  
          x_(x),                             
          velocity_x_(0.0),             
          velocity_y_(0.0),
          predicted_x_(x),              
          predicted_y_(y),
          angle_(angle),
          angular_velocity_(0.0),
          predicted_angle_(angle),
          last_observed_ns_(timestamp_ns),
          missed_count_(0)
    {
    }

    TrackingBase::~TrackingBase() = default;

    
    double TrackingBase::secondsBetween(std::int64_t from_ns, std::int64_t to_ns) noexcept
    {
        return static_cast<double>(to_ns - from_ns) / 1.0e9;
    }

    
    void TrackingBase::updateObserved(const Observation &observation, std::int64_t timestamp_ns, double frame_dt)
    {
        
        const bool was_lost = (status_ == TargetStatus::LOST);

        if (was_lost)
        {
            
            velocity_x_ = 0.0;
            velocity_y_ = 0.0;
            angular_velocity_ = 0.0;
            x_ = observation.x;
            y_ = observation.y;
            angle_ = tracksAngle() ? observation.angle : 0.0;
            predicted_x_ = x_;
            predicted_y_ = y_;
            predicted_angle_ = angle_;
        }
        else
        {
            
            const double dt_obs = secondsBetween(last_observed_ns_, timestamp_ns);
            if (dt_obs > 0.0)
            {
                velocity_x_ = (observation.x - x_) / dt_obs;
                velocity_y_ = (observation.y - y_) / dt_obs;
                if (tracksAngle())
                {
                    angular_velocity_ = (observation.angle - angle_) / dt_obs;
                }
            }

            
            x_ = observation.x;
            y_ = observation.y;
            angle_ = tracksAngle() ? observation.angle : 0.0;

            
            predicted_x_ = x_ + velocity_x_ * frame_dt;
            predicted_y_ = y_ + velocity_y_ * frame_dt;
            predicted_angle_ = tracksAngle() ? angle_ + angular_velocity_ * frame_dt : 0.0;
        }

        
        status_ = TargetStatus::TRACK;
        missed_count_ = 0;
        last_observed_ns_ = timestamp_ns;
    }

    
    void TrackingBase::updateUnobserved(double frame_dt)
    {
        if (status_ != TargetStatus::TRACK)
        {
            return;  
        }

        ++missed_count_;
        if (missed_count_ >= lostThreshold())
        {
            enterLostState();   
            return;   
        }

        
        predicted_x_ += velocity_x_ * frame_dt;
        predicted_y_ += velocity_y_ * frame_dt;

        if (tracksAngle())
        {
            angle_ += angular_velocity_ * frame_dt;
            predicted_angle_ = angle_ + angular_velocity_ * frame_dt;
        }
        
    }

    
    void TrackingBase::enterLostState()
    {
        status_ = TargetStatus::LOST;
        x_ = 0.0;
        y_ = 0.0;
        angle_ = 0.0;
        velocity_x_ = 0.0;
        velocity_y_ = 0.0;
        angular_velocity_ = 0.0;
        predicted_x_ = 0.0;
        predicted_y_ = 0.0;
        predicted_angle_ = 0.0;
    }

    
    TargetSnapshot TrackingBase::snapshot() const
    {
        TargetSnapshot result{}; //建一个空的结构体
        result.id = id_;
        result.type = type_;
        result.status = status_;
        result.x = x_;
        result.y = y_;
        result.velocity_x = velocity_x_;
        result.velocity_y = velocity_y_;
        result.predicted_x = predicted_x_;
        result.predicted_y = predicted_y_;
        result.angle = angle_;
        result.angular_velocity = angular_velocity_;
        result.predicted_angle = predicted_angle_;
        result.missed_count = missed_count_;
        return result;
    }

} 