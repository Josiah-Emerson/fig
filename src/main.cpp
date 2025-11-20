#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <matplot/matplot.h>
#include <utility>
#include "EulerMaruyama.h"
#include "matplot/core/line_spec.h"
#include "matplot/freestanding/plot.h"


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
   /* 
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
   */

   constexpr double D { 2.6e-3 };
   //std::function<double(double, double)> a = [](double x, double y) -> double {return 0; };
   //std::function<double(double, double)> b = [](double x, double y) -> double { return std::sqrt(2 * D); };
   //EulerMaruyama em {1, 1000, 0, a, b};
   //em.run();

   EulerMaruyama<2>::stepperFunc a = [](const EulerMaruyama<2>::vectorND X, double time) -> EulerMaruyama<2>::vectorND {
      return EulerMaruyama<2>::vectorND { 0 };
   };
   EulerMaruyama<2>::stepperFunc b = [](const EulerMaruyama<2>::vectorND X, double time) -> EulerMaruyama<2>::vectorND {
      EulerMaruyama<2>::vectorND res { };
      res.fill(std::sqrt(2 * D));
      return res;
   };

   std::array<EulerMaruyama<2>::vectorND, 1000> arr {};
   for(std::size_t i { 0 }; i < 1000; ++i){
      //EulerMaruyama<1>::vectorND X { 0 };
      EulerMaruyama<2> em (1e-5, 1000, {0}, a, b);
      auto results = em.run();
      arr[i] = results.back().first;
   }

   std::vector<double> x { };
   std::vector<double> y { };
   for(std::size_t i { 0 }; i < 1000; ++i){
      x.emplace_back(arr[i][0]);
      y.emplace_back(arr[i][1]);
   }

   auto p = matplot::scatter(x, y);
   p->marker_size(5);
   p->marker_face(true);
   matplot::show();
}

