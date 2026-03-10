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
      float clampAndNormalize(int* val);

      int m_red { 0 };
      int m_green { 0 };
      int m_blue { 0 };
      Core::ImGuiContainer m_imGuiContainer;
};
