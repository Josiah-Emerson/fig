#include "ImGuiContainer.h"
#include "imgui.h"

namespace Core{
   void ImGuiContainer::draw(){
      if(m_showDemoWindow){
         ImGui::ShowDemoWindow(&m_showDemoWindow);
      }
   }

} // namespace Core
