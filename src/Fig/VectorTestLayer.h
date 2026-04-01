#pragma once

#include "Layer.h"

class VectorTestLayer : public Core::Layer{
   public: 
      VectorTestLayer() = default;

      bool onEvent(Core::Events::Event& event) override;

   private: 
      void output();
};
