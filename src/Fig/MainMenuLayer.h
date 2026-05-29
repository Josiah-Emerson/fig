#pragma once
#include "Events.h"
#include "Layer.h"

class MainMenuLayer : public Core::Layer{
   public:
      MainMenuLayer();
      bool onEvent(Core::Events::Event& event) override;
      void onUpdate(float dt) override;
      void onRender() override;

   private:
};
