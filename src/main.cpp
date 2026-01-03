//#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <cstddef>
#include <matplot/matplot.h>
#include <numeric>
#include <ostream>
#include <set>
#include <vector>
#include "EulerMaruyama.h"
#include "matplot/freestanding/plot.h"


double norm(double N){
   if(N >= 10)
      return norm(N/10);
   else if(N < 1.0)
      return norm(N * 10);

   return N;
}

int main(){

   constexpr double D { 2.2e-5 };

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
   /*
   std::vector<double> x { };
   std::vector<double> t { };
   
   for(std::size_t i { 0 }; i < 5; ++i){
      EulerMaruyama<2> em (1, 100, {0}, 0, std::sqrt(2 * D));
      auto results = em.run();
      for(const auto& e : results){
         x.emplace_back(e.first[0]);
         t.emplace_back(e.second);
      }
      std::set<double> a;

      auto p = matplot::plot(t, x);
   }

   matplot::show();
   */
}

