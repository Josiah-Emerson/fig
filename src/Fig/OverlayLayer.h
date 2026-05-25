#pragma once

#include "Layer.h"

class OverlayLayer : public Core::Layer{
   public:
      OverlayLayer() = default;

      bool onEvent(Core::Events::Event& event) override;
      void onUpdate(float dt) override { }
      void onRender() override {}

};
