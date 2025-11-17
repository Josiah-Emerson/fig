#pragma once
#include <stdexcept>

class Particle2D{
   public: 
      struct Position2D{
         double x { };
         double y { };

         bool operator==(const Position2D& rhs) const { return x == rhs.x && y == rhs.y; }
         bool operator!=(const Position2D& rhs) const { return !(*this==rhs);}
      };

   private: 
      double radius_ { };
      Position2D position_ { };

   public: 
      Particle2D(double radius, Position2D position = { 0, 0 } )
         : radius_ { radius }
         , position_ { position }
         {
            if(radius_ <= 0)
               throw std::invalid_argument("Radius must be greater than 0");
         }

      double radius() const { return radius_; }
      Position2D position() const { return position_; }

      void updatePosition(const Position2D& newPosition) { position_ = newPosition; }
};


