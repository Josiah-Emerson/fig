#include <iostream>
#include "imgui_impl_x11.h"

bool ImGui_ImplX11_InitForOpenGL(Display* display, Window* window, GLXContext* x_gl_context){
   std::cout << "init\n";
   return true;
}

IMGUI_IMPL_API void ImGui_ImplX11_NewFrame(){
   std::cout << "new frame\n";
}

IMGUI_IMPL_API void ImGui_ImplX11_Shutdown();
IMGUI_IMPL_API void ImGui_ImplX11_ProcessEvent(const XEvent* event){
   switch(event->type){
      case ButtonPress: 
         std::cout << "ButtonPress";
         break;
      case ButtonRelease: 
         std::cout << "ButtonRelease";
         break;
      case KeyPress: 
         std::cout << "KeyPress";
         break;
      case KeyRelease: 
         std::cout << "KeyRelease";
         break;
      default: 
         std::cout << "Not handled here\n";
         break;
   }
}
