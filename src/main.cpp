//#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <cstddef>
#include <matplot/matplot.h>
#include <set>
#include "EulerMaruyama.h"
#include "matplot/freestanding/plot.h"


int main(){

   constexpr double D { 2.2e-5 };
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
}

