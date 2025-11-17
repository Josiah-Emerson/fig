#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include <iostream>


int main(){
   // radius of particle in meters
   constexpr double PARTICLE_RADIUS_METERS { 2.5e-6 };
   // width of the screen to be in terms of particles
   constexpr int WINDOW_WIDTH_PARTICLES{ 1000 };

   constexpr int NUM_TIME_STEPS { 24 };
   constexpr double SIMULATION_LENGTH_SECONDS { 4e-9 };

   Particle2D particle {PARTICLE_RADIUS_METERS};
   Simulation sim { NUM_TIME_STEPS, SIMULATION_LENGTH_SECONDS, particle};
   sim.run();
   auto traj = sim.getTrajectory();

   for(const auto& point : traj){
      std::cout << "(" << point.first.x << ", " << point.first.y << ") at time " << point.second << '\n';
   }
}
