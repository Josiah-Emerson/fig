#pragma once

#include "Layer.h"

class OverlayLayer : public Core::Layer{
   public:
      OverlayLayer() = default;

      bool onEvent(Core::Events::Event& event) override;
      void onUpdate() override { }
      void onRender() override {}

};
