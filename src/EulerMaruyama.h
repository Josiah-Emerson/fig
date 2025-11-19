// implementation of Euler-Maruyama method for approximate numerical solution of SDE

#pragma once
#include <functional>
#include <stdexcept>
#include <random>
#include <cmath>

class EulerMaruyama {
   private: 
      const double dt { };
      int n { 0 };
      const int N_ { };
      double Y { };
      using stepperFunc = std::function<double(double Y, double tau)>;
      const stepperFunc a_;
      const stepperFunc b_;
      std::normal_distribution<> W;
      double W_tn; 

   public: 
      EulerMaruyama(const double T, const int N, const double X0, const stepperFunc a, const stepperFunc b)
         : dt { static_cast<double>(T)/static_cast<double>(N)}
         , N_ { N }
         , Y { X0 }
         , a_ { a }
         , b_ { b }
         , W { std::normal_distribution(0.0, std::sqrt(dt))}
         , W_tn { dW() }
         {
            if(T <= 0)
               throw new std::invalid_argument("The length of time to solve for must be greater than 0");

            if(N <= 0)
               throw new std::invalid_argument("There must be a positive non-zero number of steps");

            if(dt <= 0)
               throw new std::invalid_argument("The values of T and N result in a negative or zero dt (T/N) value which is not allowed");


         }

         void run();
         double getPosition() const { return Y; };

   private: 
         void step();
         double dW();
};
