#include "Simulation.h"

void Simulation::run(){
   while(window_.isOpen()){
      while(const std::optional event = window_.pollEvent()){
         if(event->is<sf::Event::Closed>())
            window_.close();
      }

      window_.clear();
      updateParticle();
      drawParticle();

      window_.display();
   }
}

void Simulation::updateParticle(){
   particle_.updatePosition( {particle_.position().x + particle_.radius(), particle_.position().y + (particle_.radius())} );
}

void Simulation::drawParticle(){
   sf::CircleShape circle { metersToPixels(particle_.radius()) };
   //circle.setFillColor({ 255, 255, 255 } );
   circle.setFillColor(sf::Color(255, 255, 255));
   circle.setOrigin({ circle.getRadius(), circle.getRadius()} );
   circle.setPosition({
         window_.getSize().x/2.f + metersToPixels(particle_.position().x),
         window_.getSize().y/2.f + metersToPixels(particle_.position().y)
         });

   window_.draw(circle);
}

float Simulation::metersToPixels(double val) const {
   const double PIXELS_PER_METER { window_.getSize().x / sett_.viewWidthMeters };
   return PIXELS_PER_METER * val;
}
