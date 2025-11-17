#include "Simulation.h"
#include <chrono>
#include <iostream>

void Simulation::run() {
   std::size_t i { 0 };

   auto start = std::chrono::high_resolution_clock::now();
   while(i < numTimesteps_){
      step();
      trajectory_.emplace_back( particle_.position(), (simulationLengthSeconds_ / numTimesteps_) * i );
      ++i;
   }
   auto end = std::chrono::high_resolution_clock::now();

   std::chrono::duration<double, std::milli> duration = end - start;
   std::cout << "Total time taken to simulate: " << duration.count() << "ms\n";
   std::cout << "An average of " << duration.count()/numTimesteps_ << "ms per step\n";
}


void Simulation::step(){
   particle_.updatePosition( {particle_.position().x * 2, particle_.position().y + 1 }) ;
}
