//#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <cstddef>
#include <matplot/matplot.h>
#include <numeric>
#include <ostream>
#include <set>
#include <string>
#include <vector>
#include "EulerMaruyama.h"
#include "matplot/freestanding/axes_functions.h"
#include "matplot/freestanding/plot.h"
#include "matplot/util/colors.h"
#include "matplot/util/handle_types.h"


double norm(double N){
   if(N >= 10)
      return norm(N/10);
   else if(N < 1.0)
      return norm(N * 10);

   return N;
}

int main(){

   constexpr double D { 2.2e-5 };

   /*
   const int NUM_TESTS { 10 };
   double results[NUM_TESTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
   const int steps[NUM_TESTS] = {1, 5, 10, 50, 100, 500, 1000, 5000, 10000, 50000};
   constexpr int NUM_ITER { 10000 };
   for(std::size_t i { 0 }; i < NUM_TESTS; ++i){
      for(int j { 0 }; j < NUM_ITER; ++j){
         EulerMaruyama<2> em (1, steps[i], { 0 }, 0, std::sqrt(2 * D));
         const double finalX = em.run().back().first[0];
         double MSD = finalX * finalX;
         results[i] += MSD;

         if(j % (NUM_ITER/10) == 0)
            std::cout << "Iteration #" << j << " on step #" << i << '\n';
      }
   }

   for(int i { 0 }; i < 10; ++i)
      std::cout <<'\n';

   constexpr double MSD {2*D};
   std::cout << "The expected MSD is: " << MSD <<'\n';
   for(int i {0}; i < NUM_TESTS; ++i){
      std::cout <<"The average MSD after " <<NUM_ITER << " iterations for " << steps[i] << " time steps is: " << results[i]/NUM_ITER << '\n';
   }
   */

   const int timesteps[] = {10, 100, 1000, 10000, 100000};

   matplot::colormap(matplot::palette::winter());
   auto cmap = matplot::colormap();
   matplot::hold(matplot::on);

   for(std::size_t i { 0 }; i < std::size(timesteps); ++i){
      EulerMaruyama<2> em = {1, timesteps[i], { 0 }, 0, std::sqrt(2 * D)};
      auto res = em.run();

      std::vector<double> x, t;
      x.reserve(res.size());
      t.reserve(res.size());

      for(const auto& e : res){
         x.push_back(e.first[0]);
         t.push_back(e.second);
      }

      matplot::plot(t, x);
   }

   matplot::show();
}

