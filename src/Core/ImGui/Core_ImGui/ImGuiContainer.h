#pragma once
#include <imgui.h>

// TODO: Would this be better as a layer type ?
// Container for anything ImGui related 
// handles drawing and end/new frame

// TODO: I think we only need one imgui context in the entire program but if not we should move context creation here

namespace Core{
   class ImGuiContainer{
      public: 
         // NOTE: To check imgui is created (it should be) ImGui::GetCurrentContext() returns a nullptr if its not
         // Also the if it returns a pointer there is a struct member bool called initialized. For some reason it is true right when CreateContext is called 
         // not sure when it would be false

         ImGuiContainer() = default;
         ~ImGuiContainer() = default;

         // NOTE: No render because we should only ImGui::Render() once per frame (Render ends ImGui frame)
         void draw();

         bool m_showDemoWindow { true };
   };

} // namespace Core
