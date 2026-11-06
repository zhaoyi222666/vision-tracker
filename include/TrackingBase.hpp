#pragma once

#include <cstdint>

#include "data_types.hpp"

namespace rm
{

    class TrackingBase
    {
    public:
       
        TrackingBase(int id, TargetType type, double x, double y, double angle, std::int64_t timestamp_ns);
        virtual ~TrackingBase();
        virtual int lostThreshold() const = 0;  
        virtual bool tracksAngle() const = 0;    
        void updateObserved(const Observation &observation, std::int64_t timestamp_ns, double frame_dt);
        void updateUnobserved(double frame_dt);

        TargetSnapshot snapshot() const;  

        int id() const noexcept { return id_; }
        TargetType type() const noexcept { return type_; }
        TargetStatus status() const noexcept { return status_; }
        int missedCount() const noexcept { return missed_count_; }

        static double secondsBetween(std::int64_t from_ns, std::int64_t to_ns) noexcept;

    private:
        void enterLostState();   

        int id_;                
        TargetType type_;        
        TargetStatus status_;    

        double x_;   
        double y_;
        double velocity_x_;            
        double velocity_y_;
        double predicted_x_;     
        double predicted_y_;

        double angle_;               
        double angular_velocity_;    
        double predicted_angle_;     
        std::int64_t last_observed_ns_;  
        int missed_count_;               
    };

} 