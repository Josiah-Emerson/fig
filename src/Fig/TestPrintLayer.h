#pragma once
#include "Layer.h"

class TestPrintLayer : public Core::Layer{
   public:
      TestPrintLayer() = default;
      bool onEvent(Core::Events::Event& event) override;
      void onUpdate() override;
      void onRender() override;
};
