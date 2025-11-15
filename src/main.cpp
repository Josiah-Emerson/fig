#include <SFML/Graphics.hpp>
#include "Simulation.h"


int main(){
   // radius of particle in meters
   constexpr double PARTICLE_RADIUS_METERS { 2.5e-6 };
   // width of the screen to be in terms of particles
   constexpr int WINDOW_WIDTH_PARTICLES{ 1000 };

   Particle2D particle {PARTICLE_RADIUS_METERS};
   Simulation sim {{particle.radius() * WINDOW_WIDTH_PARTICLES, 0}, particle };
   sim.run();

   /*
   auto window = sf::RenderWindow(sf::VideoMode::getFullscreenModes().at(0), "brownian");
   window.setFramerateLimit(144);

   sf::CircleShape circle(3.f);
   circle.setFillColor(sf::Color(255, 255, 255));
   circle.setOrigin({circle.getRadius(), circle.getRadius()});
   sf::Vector2u windowSize {window.getSize()};
   circle.setPosition({ 0, windowSize.y/2.f});

   while(window.isOpen()){
      while(const std::optional event = window.pollEvent()){
         if(event->is<sf::Event::Closed>())
            window.close();
      }

      window.clear();

      sf::Vector2<float> currPos { circle.getPosition()};
      circle.setPosition( { currPos.x + 1, currPos.y } );
      window.draw(circle);

      window.display();
   }

   window.clear();
   window.display();
   */

}
