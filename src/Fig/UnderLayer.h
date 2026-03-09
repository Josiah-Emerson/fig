#pragma once

#include "Layer.h"
#include "Core_ImGui/ImGuiContainer.h"

class UnderLayer : public Core::Layer{
   public:
      UnderLayer();

      bool onEvent(Core::Events::Event& event) override;
      void onUpdate() override;
      void onRender() override;

   private: 
      Core::ImGuiContainer m_imGuiContainer;
};
