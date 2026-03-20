#pragma once

#include "Layer.h"
#include "Core_ImGui/ImGuiPropertyEditor.h"
#include <memory>

class UncaughtEventsOutputLayer : public Core::Layer{
   public:
      UncaughtEventsOutputLayer();

      bool onEvent(Core::Events::Event& event) override;
      void onUpdate() override;
      void onRender() override;

   private: 
      float clampAndNormalize(int* val);

      int m_red { 0 };
      int m_green { 0 };
      int m_blue { 0 };
      bool m_open = true;
      std::unique_ptr<Core::ImGuiPropertyEditor> m_imGuiPropertyEditor;
};
