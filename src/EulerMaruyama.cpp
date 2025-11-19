#include "EulerMaruyama.h"
#include <cstddef>

// IDEK whats going on with typenames and allt
template<std::size_t NDim>
using vectorND = typename EulerMaruyama<NDim>::vectorND;

template<std::size_t NDim>
std::vector<std::pair<std::array<double, NDim> ,double>> EulerMaruyama<NDim>::run(){
   // start at 1 because we already have initial position
   for(std::size_t i { 1 }; i < results_.size(); ++i){ // for each timestep
      results_[i].first = step(results_[i-1].first, dW(i));
   }

   return results_;
}

template<std::size_t NDim>
EulerMaruyama<NDim>::vectorND EulerMaruyama<NDim>::step(const EulerMaruyama<NDim>::vectorND&, const double DELTA_W){

}

/*
#include "EulerMaruyama.h"
#include <chrono>
#include <iostream>

void EulerMaruyama::run(){
   auto start = std::chrono::high_resolution_clock::now();
   while(n < N_){
      step();
      ++n;
   }
   auto end = std::chrono::high_resolution_clock::now();

   std::chrono::duration<double, std::milli> duration = end - start;
   std::cout << "Total time taken to simulate: " << duration.count() << "ms\n";
   std::cout << "An average of " << duration.count()/N_ << "ms per step\n";
}

void EulerMaruyama::step(){
   const double TAU { n * dt };
   const double NEW_DW { dW() };
   const double DELTA_W { NEW_DW - W_tn};
   W_tn = NEW_DW;
   Y += a_(Y, TAU) + (b_(Y, TAU) * DELTA_W);
}

double EulerMaruyama::dW(){
   static std::random_device rd {};
   static std::mt19937 gen { rd() };
   return W(gen);
}
*/
