#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include "../src/Particle2D.h"


TEST_CASE("Position2D struct", "[Position2D]"){
   Particle2D::Position2D position { };
   Particle2D::Position2D positive {1, 1};
   Particle2D::Position2D negative { -1, -1 };

   SECTION("Initializers work correctly"){
      REQUIRE(position.x == 0);
      REQUIRE(position.y == 0);
      REQUIRE(positive.x == 1);
      REQUIRE(positive.y == 1);
      REQUIRE(negative.x == -1);
      REQUIRE(negative.y == -1);
   }

   SECTION("Equality and inequality operator checks"){
      Particle2D::Position2D position2 { };
      Particle2D::Position2D positive2 {1, 1};
      Particle2D::Position2D negative2 { -1, -1 };

      SECTION("Equality operator"){
         REQUIRE(position == position2);
         REQUIRE(positive == positive2);
         REQUIRE(negative == negative2);

         REQUIRE(position2 == position);

         REQUIRE_FALSE(position == positive);
         REQUIRE_FALSE(position == negative);
         REQUIRE_FALSE(positive == negative);

         position2.x++;
         REQUIRE_FALSE(position == position2);
         REQUIRE_FALSE(positive == position2);
         REQUIRE_FALSE(negative == position2);

         position2.y++;
         REQUIRE_FALSE(position == position2);
         REQUIRE(positive == position2);
         REQUIRE_FALSE(negative == position2);
      }

      SECTION("Inequality operator"){
         REQUIRE_FALSE(position != position2);
         REQUIRE_FALSE(positive != positive);
         REQUIRE_FALSE(negative != negative2);
      }

   }
}

TEST_CASE("Particle2D constructor test", "[Particle2D]"){
   using Position = Particle2D::Position2D;
   constexpr double RADIUS { 1.5 };
   Particle2D particle { RADIUS } ;

   REQUIRE(particle.radius() == RADIUS);
   REQUIRE(particle.position() == Position { 0, 0} );
   REQUIRE(particle.position() == Position {} );


   Particle2D particle2 { RADIUS, {1, 1} };
   REQUIRE(particle2.radius() == RADIUS);
   REQUIRE(particle2.position() == Position {1, 1} );

   REQUIRE_THROWS_AS(Particle2D(0), std::invalid_argument);
   REQUIRE_THROWS_AS(Particle2D(-1), std::invalid_argument);
}

TEST_CASE("Particle2D::updatePosition", "[Particle2D]"){
   constexpr double RADIUS { 1 };
   Particle2D particle { RADIUS };

   particle.updatePosition({1, 1});
   REQUIRE(particle.position() == Particle2D::Position2D {1, 1});
}
