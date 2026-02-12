// #include "EulerMaruyama.h"
#include "System.h"
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_x11.h>


//TODO: Delete this and ifdefs/endif
//#define ABCD

static bool running = true;
void processEvent(XEvent* event);

int main(){
   System sys { };
   sys.run();
   return 0;

#ifdef ABCD
   while(running){
      while(XPending(display) > 0){
         XEvent event { };
         XNextEvent(display, &event);
         ImGui_ImplX11_ProcessEvent(&event);
      }

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplX11_NewFrame();
      ImGui::NewFrame();

      bool showDemoWindow = true;
      ImGui::ShowDemoWindow(&showDemoWindow);

      ImGui::Render();

      glClearColor(1, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glXSwapBuffers(display, glxWin);
   }
   return 0;
#endif
}

