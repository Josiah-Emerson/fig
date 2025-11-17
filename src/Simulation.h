#pragma once
#include "Particle2D.h"
#include <utility>
#include <vector>

class Simulation{
   private: 
      using TrajectoryPoint = std::pair<Particle2D::Position2D, double>;
      int numTimesteps_ { };
      double simulationLengthSeconds_ { };
      Particle2D particle_;
      std::vector<TrajectoryPoint> trajectory_;

   public: 
      Simulation(const int numTimesteps, const double simulationLengthSeconds, const Particle2D& particle)
         : numTimesteps_ { numTimesteps }
         , simulationLengthSeconds_ { simulationLengthSeconds }
         , particle_ { particle }
         {
            trajectory_.reserve(static_cast<std::size_t>(numTimesteps_));
         }

      void run();
      std::vector<TrajectoryPoint> getTrajectory() const { 
         return trajectory_; 
      }

   private: 
      void step();
};
