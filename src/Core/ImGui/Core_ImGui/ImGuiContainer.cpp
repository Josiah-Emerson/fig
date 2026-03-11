#include "ImGuiContainer.h"
#include "Application.h"
#include <imgui.h>

namespace Core{
   void ImGuiContainer::draw(){
      // TODO: update with way to pass width/height from calling function
      const int WINDOW_HEIGHT = Application::get().getWindow()->getHeight();
      const int WINDOW_WIDTH = Application::get().getWindow()->getWidth();
      const int HEIGHT { WINDOW_HEIGHT / 2 };
      const int WIDTH { 400 };

      ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
      ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - WIDTH, 0));
      if(ImGui::Begin("ImGui Container", m_open))
         internalDraw();

      // NOTE: If we want to handle ImGui::Begin returning false, invert if statement, ensure we add 
      // ImGui::End() in if statement, handle this, and then early return in if block
      ImGui::End();
   }
} // namespace Core
