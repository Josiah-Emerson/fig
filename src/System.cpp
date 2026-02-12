#include "System.h"
#include "imgui.h"
#include <GL/gl.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_x11.h>
#include <stdexcept> // for std::runtime_error

System::System(){
   XDisplay_ = XOpenDisplay(NULL);

   // TODO: More flexible way to choose options
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
   GLXFBConfig* glxfbConfig = glXChooseFBConfig(XDisplay_, XDefaultScreen(XDisplay_), glxfbAttributeList, &numElements);
   if(!glxfbConfig)
      throw std::runtime_error("Error choosing framebuffer config for GLX in System constructor");
   XVisualInfo* vInfo = glXGetVisualFromFBConfig(XDisplay_, glxfbConfig[0]);

   XSetWindowAttributes setWindowAttributes { };
   setWindowAttributes.event_mask = ButtonPressMask | ButtonReleaseMask |
                                    KeyPressMask | KeyReleaseMask |
                                    StructureNotifyMask | PointerMotionMask | 
                                    EnterWindowMask | LeaveWindowMask;
   setWindowAttributes.colormap = XCreateColormap(XDisplay_, RootWindow(XDisplay_, vInfo->screen), vInfo->visual, AllocNone);
   XWindow_ = XCreateWindow(XDisplay_, RootWindow(XDisplay_, vInfo->screen),
                           0, 0, 1920, 1080, 0, vInfo->depth, InputOutput,
                           vInfo->visual, CWEventMask | CWColormap, &setWindowAttributes);
   XMapWindow(XDisplay_, XWindow_);

   glxWindow_ = glXCreateWindow(XDisplay_, glxfbConfig[0], XWindow_, NULL);
   glxContext_ = glXCreateNewContext(XDisplay_, glxfbConfig[0], GLX_RGBA_TYPE, NULL, True);
   if(!glXMakeContextCurrent(XDisplay_, glxWindow_, glxWindow_, glxContext_))
      throw std::runtime_error("Error making glxContext current in system constructor");

   // Setup ImGui

   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // ImGuiConfigFlags_NavEnableGamepad
   ImGui::StyleColorsDark();

   // platform and graphics backend stuff
   ImGui_ImplX11_InitForOpenGL(XDisplay_, &XWindow_);
   ImGui_ImplOpenGL3_Init();

   // TODO: Implement a way to adjust the scaling based on the dpi of the system 
   // https://stackoverflow.com/questions/49071914/how-to-get-system-scale-factor-in-x11
   // Talks about how to possibly do that in X11 ^ 
   /* 
   ImGuiStyle& style = ImGui::GetStyle();
   style.ScaleAllSizes(main_scale);
   style.FontScaleDpi = main_scale;
   */
}

System::~System(){
   XCloseDisplay(XDisplay_);
   // TODO: Do we need to do anything special for other stuff?
}

#include <iostream>
void System::run(){
   // main loop: Poll events from X, send to imgui, if they don't want,
   // translate and send to application layer
   while(running_){
      while(XPending(XDisplay_) > 0 ){
         XEvent event { };
         XNextEvent(XDisplay_, &event);
         ImGui_ImplX11_ProcessEvent(&event);
         processEvent(event);
      }

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplX11_NewFrame();
      ImGui::NewFrame();

      static bool b = true;
      if(b)
         ImGui::ShowDemoWindow(&b);
      ImGui::Render();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glXSwapBuffers(XDisplay_, glxWindow_);
   }
}

void System::processEvent(const XEvent& event){
   switch(event.type){
      case KeyPress:
      case KeyRelease:
         if(ImGui::GetIO().WantCaptureKeyboard)
            return;
         break;
      case ButtonPress: 
      case ButtonRelease:
         if(ImGui::GetIO().WantCaptureMouse)
            return;
         break;
   }
}
