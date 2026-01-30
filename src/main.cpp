// #include "EulerMaruyama.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_x11.h>
#include <iostream>

static bool running = true;

int main(){
   Display* display = XOpenDisplay(NULL);
   int glxfbAttributeList[] = {GLX_DOUBLEBUFFER, True,
                              GLX_RED_SIZE, 8,
                              GLX_GREEN_SIZE, 8,
                              GLX_BLUE_SIZE, 8,
                              GLX_ALPHA_SIZE, 8,
                              GLX_X_RENDERABLE, True,
                              GLX_STENCIL_SIZE, 8,
                              GLX_DEPTH_SIZE, 24,
                              None};
   int numElements {};
   GLXFBConfig* glxfbConfig = glXChooseFBConfig(display, XDefaultScreen(display), glxfbAttributeList, &numElements);
   if(!glxfbConfig){
      std::cout << "No config\n";
      return 1;
   }

   XVisualInfo* vInfo = glXGetVisualFromFBConfig(display, glxfbConfig[0]);

   XSetWindowAttributes setWindowAttributes { };
   setWindowAttributes.event_mask = ButtonPressMask | ButtonReleaseMask | 
                                    KeyPressMask | KeyReleaseMask | 
                                    StructureNotifyMask | PointerMotionMask;
   setWindowAttributes.colormap = XCreateColormap(display, RootWindow(display, vInfo->screen), vInfo->visual, AllocNone);
   Window win = XCreateWindow(display, RootWindow(display, vInfo->screen),
                              0, 0, 1920, 1080, 0, vInfo->depth, InputOutput,
                              vInfo->visual, CWEventMask | CWColormap, &setWindowAttributes);
   XMapWindow(display, win);

   GLXWindow glxWin = glXCreateWindow(display, glxfbConfig[0], win, NULL);

   GLXContext glxContext = glXCreateNewContext(display, glxfbConfig[0], GLX_RGBA_TYPE, NULL, True);
   if(!glXMakeContextCurrent(display, glxWin, glxWin, glxContext)){
      std::cout << "Error making context current\n";
      return 1;
   }

   // setup for ImGui
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

   // style
   ImGui::StyleColorsDark();

   // TODO: Implement a way to adjust the scaling based on the dpi of the system 
   // https://stackoverflow.com/questions/49071914/how-to-get-system-scale-factor-in-x11
   // Talks about how to possibly do that in X11 ^ 
   /* 
   ImGuiStyle& style = ImGui::GetStyle();
   style.ScaleAllSizes(main_scale);
   style.FontScaleDpi = main_scale;
   */


   // Setup Platform/Renderer backends
   ImGui_ImplX11_InitForOpenGL(display, &win);
   ImGui_ImplOpenGL3_Init();

   while(running){
      while(XPending(display) > 0){
         XEvent event { };
         XNextEvent(display, &event);
         ImGui_ImplX11_ProcessEvent(&event);
         if(event.type != StructureNotifyMask){
            std::cout << "None button or key event detected";
         }
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
}

