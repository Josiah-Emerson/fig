#pragma once

class Particle2D{
   public: 
      struct Position2D{
         double x { };
         double y { };
      };

   private: 
      double radius_ { };
      Position2D position_ { };

   public: 
      Particle2D(double radius, Position2D position = { 0, 0 } )
         : radius_ { radius }
         , position_ { position }
         { }

      double radius() const { return radius_; }
      Position2D position() const { return position_; }

      void updatePosition(const Position2D& newPosition) { position_ = newPosition; }
};

