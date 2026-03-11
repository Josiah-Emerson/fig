#pragma once

#include "Core_ImGui/ImGuiPropertyEditor.h"
#include "Layer.h"

class SquareLayer : public Core::Layer{
   public: 
      SquareLayer(const int numSquares = 3);

      bool onEvent(Core::Events::Event& event) override;
      void onUpdate() override;
      void onRender() override;

   private: 
      struct sceneNode;
      std::unique_ptr<sceneNode> buildSceneTree(const int numSquares = 3);

      std::unique_ptr<Core::ImGuiPropertyEditor> m_propertyEditor;
      bool m_propertyEditorOpen { false };

      std::unique_ptr<sceneNode> m_rootSceneNode;
};
