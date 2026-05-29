#include "ImGuiContainer.h"
#include "Application.h"
#include <imgui.h>

namespace Core{
   void ImGuiContainer::draw(){
      // TODO: update with way to pass width/height from calling function
      const int WINDOW_WIDTH = Application::get().getWindow()->getWidth();

      ImGui::SetNextWindowSize(ImVec2(m_settings.width, m_settings.height));
      ImGui::SetNextWindowSize(ImVec2(
               m_settings.leftJustify ? m_settings.x : WINDOW_WIDTH - m_settings.x,
               m_settings.y ));
      if(ImGui::Begin("ImGui Container", m_open))
         internalDraw();

      // NOTE: If we want to handle ImGui::Begin returning false, invert if statement, ensure we add 
      // ImGui::End() in if statement, handle this, and then early return in if block
      ImGui::End();
   }
} // namespace Core
