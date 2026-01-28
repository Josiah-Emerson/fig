// implementation of Euler-Maruyama method for approximate numerical solution of SDE
#pragma once
#include <stdexcept>
#include <random>
#include <array>
#include <functional>

template<std::size_t NDim>
class EulerMaruyama{
   public:
      using vectorND = std::array<double, NDim>;
      using stepperFunc = std::function<vectorND(const vectorND&, double time)>;

   private: 
      std::vector<std::pair<vectorND, double>> results_; // the second argument in pair (double) represents the timestamp. the vector will be of size N + 1, where N represents the number of timesteps to compute
      stepperFunc a_;
      stepperFunc b_;
      std::vector<vectorND> W_; // populated during construction to have a size of the number of timesteps + 1
      // TEMP 
      double totalWalk_ { 0 };

   public: 
      // T is length in seconds of sim to run 
      // N is number of steps to divide time T into
      // X0 is an array of doubles with N elements representing the inital values for the system
      // a and b are functions which satisfy the SDE dX_t = a(X_t, t)dt + b(X_t, t)dW_t
      EulerMaruyama(const double T, const int N, const vectorND& X0, const stepperFunc a, stepperFunc b)
         : a_ { a }
         , b_ { b }
         {
            if(T <= 0)
               throw std::invalid_argument("T (length of time to simulate) must be positive and non-zero");
            if(N <= 0)
               throw std::invalid_argument("N (number of time steps) must be positive and non-zero");

            results_.reserve(static_cast<std::size_t>(N + 1));
            results_.emplace_back(vectorND {X0}, 0);

            const double DELTA_TIME { T/static_cast<double>(N) };
            for(std::size_t i { 1 }; i < N + 1; ++i)
               results_.emplace_back(vectorND { 0 }, (i * DELTA_TIME));

            W_.resize(static_cast<std::size_t>(N + 1));
            std::random_device rd  { };
            std::mt19937 gen { rd() };
            std::normal_distribution<double> W {0.0, std::sqrt(DELTA_TIME)}; // normal distribution with mean 0 and variance dt
                                                                             
            for(std::size_t i { 1 }; i < N + 1; ++i){
               for(std::size_t k { 0 }; k < NDim; ++k){
                  W_[i][k] = W(gen);
               }
            }

         }

      EulerMaruyama(const double T, const int N, const vectorND& X0, const double a, const double b)
         : EulerMaruyama(T, N, X0, 
               [a](const vectorND& X, double time) -> vectorND{
                  vectorND res { };
                  res.fill(a);
                  return res;
               },
               [b](const vectorND& X, double time) -> vectorND{
                  vectorND res { };
                  res.fill(b);
                  return res;
               })
      { }

      std::vector<std::pair<vectorND, double>> run();
      double getTotalWalk() const { return totalWalk_; } // TEMP

   private: 
      vectorND step(const vectorND& X, const vectorND& DELTA_W, const double t);
      vectorND dW(const std::size_t n);
};

template<std::size_t NDim>
std::vector<std::pair<typename EulerMaruyama<NDim>::vectorND, double>> EulerMaruyama<NDim>::run(){
   // start at 1 because we already have initial position
   for(std::size_t i { 1 }; i < results_.size(); ++i){ // for each timestep
      results_[i].first = step(results_[i-1].first, dW(i), results_[i-1].second);
   }

   return results_;
}

template<std::size_t NDim>
typename EulerMaruyama<NDim>::vectorND EulerMaruyama<NDim>::step(
      const EulerMaruyama<NDim>::vectorND& X, 
      const EulerMaruyama<NDim>::vectorND& DELTA_W, 
      const double t){

   const vectorND A { a_(X, t)};
   const vectorND B { b_(X, t)};

   vectorND res { };
   for(std::size_t i { 0 }; i < NDim; ++i){
      res[i] = X[i] + A[i] + (B[i] * DELTA_W[i]);
   }

   return res;

}

template<std::size_t NDim>
typename EulerMaruyama<NDim>::vectorND EulerMaruyama<NDim>::dW(const std::size_t n){
   vectorND res { };

   for(std::size_t i { 0 }; i < NDim; ++i){
      res[i] = W_[n][i];
   }

   totalWalk_ += std::abs(res[0]); // TEMP
   return res;
}

