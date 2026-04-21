#pragma once
#include "Layer.h"
#include "Core_ECS/Registry.h"
#include "Components/PositionComponent.h"

class VectorTestLayer : public Core::Layer{
   public: 
      VectorTestLayer();

      bool onEvent(Core::Events::Event& event) override;

   private: 
      void output();

   private: 
      Core::Registry<PositionComponent> m_registry;
};
