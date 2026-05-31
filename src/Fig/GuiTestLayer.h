#pragma once
#include "Layer.h"
#include "Core_UI/Gui.h"

class GuiTestLayer : public Core::Layer {
   public: 
      GuiTestLayer();
      ~GuiTestLayer();

      void onUpdate(float dt) override;
      void onRender() override;

   private: 
      std::unique_ptr<Core::Gui> m_gui;
};
