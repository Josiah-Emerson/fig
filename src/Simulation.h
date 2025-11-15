#pragma once
#include "Particle2D.h"
#include <SFML/Graphics.hpp>

class Simulation{
   public: 
      struct ViewSettings{
         double viewWidthMeters { 0 };
         double timestepSeconds { 0 };
      };

   private: 
      const ViewSettings& sett_;
      Particle2D particle_;
      sf::RenderWindow window_;

   public: 
      Simulation(const ViewSettings& settings, const Particle2D& particle)
         : sett_ { settings }
         , particle_ { particle }
      { 
         window_ = sf::RenderWindow(sf::VideoMode::getFullscreenModes().at(0), "brownian");
         window_.setFramerateLimit(144);
      }

      void run();

   private: 
      void updateParticle();
      void drawParticle();
      float metersToPixels(double val) const;
};
