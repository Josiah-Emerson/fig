#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include <matplot/matplot.h>


void plotTrajectory(const std::vector<std::pair<Particle2D::Position2D, double>>& traj){
   std::vector<double> X(traj.size());
   std::vector<double> Y(traj.size());
   std::vector<double> t(traj.size());

   for(const auto& e : traj){
      X.push_back(e.first.x);
      Y.push_back(e.first.y);
      t.push_back(e.second);
   }

   using namespace matplot;
   plot3(X, Y, t);
   xlabel("x");
   ylabel("y");
   zlabel("t");
   show();

}
int main(){
   // radius of particle in meters
   constexpr double PARTICLE_RADIUS_METERS { 2.5e-6 };
   // width of the screen to be in terms of particles
   //constexpr int WINDOW_WIDTH_PARTICLES{ 1000 };
   constexpr int NUM_TIME_STEPS { 20 };
   constexpr double SIMULATION_LENGTH_SECONDS { 4e-9 };

   Particle2D particle {PARTICLE_RADIUS_METERS};
   Simulation sim { NUM_TIME_STEPS, SIMULATION_LENGTH_SECONDS, particle};
   sim.run();
   plotTrajectory(sim.getTrajectory());
}

