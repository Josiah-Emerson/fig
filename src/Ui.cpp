#include "Ui.h"
#include <imgui.h>

void Ui::updateGui(int* R, int* G, int* B){
   platform_.NewImGuiFrame();

   constexpr int WIDTH { 200 };
   ImGui::SetNextWindowSize(ImVec2(WIDTH, platform_.GetWindowHeight()));
   ImGui::SetNextWindowPos(ImVec2(platform_.GetWindowWidth() - WIDTH, 0));
   if(ImGui::Begin("Hello, World", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)){
      ImGui::DragInt("R", R, 1.0f, 0, 255); 
      ImGui::DragInt("G", G, 1.0f, 0, 255); 
      ImGui::DragInt("B", B, 1.0f, 0, 255); 
   }  
   ImGui::End();
   ImGui::Render();
}
