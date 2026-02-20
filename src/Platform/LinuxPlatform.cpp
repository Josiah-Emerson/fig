#include "LinuxPlatform.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_x11.h>
#include <imgui.h>
#include <GL/glx.h>
#include <stdexcept> // std::runtime_error

LinuxPlatform::LinuxPlatform()
   : Platform { }
{
   Window_.XDisplay = XOpenDisplay(NULL);

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

   int numElements;
   GLXFBConfig* glxfbConfig = glXChooseFBConfig(Window_.XDisplay, XDefaultScreen(Window_.XDisplay), glxfbAttributeList, &numElements);
   // TODO: is throwing an error best way for this? Init func? IDK
   if(!glxfbConfig)
      throw std::runtime_error("Error choosing framebuffer config for GLX in LinuxPlatform constructor");
   XVisualInfo* vInfo = glXGetVisualFromFBConfig(Window_.XDisplay, glxfbConfig[0]);

   XSetWindowAttributes setWindowAttributes { };
   // TODO: I think comment in IMGUI x11 implementation, but these are all needed for IMGUI to work, 
   // however, there might be a way for imgui impl to just subscribe to events it needs on its own,
   // and thus we can only define ones we want here? TBD
   setWindowAttributes.event_mask = ButtonPressMask | ButtonReleaseMask |
                                    KeyPressMask | KeyReleaseMask |
                                    StructureNotifyMask | PointerMotionMask | 
                                    EnterWindowMask | LeaveWindowMask;
   setWindowAttributes.colormap = XCreateColormap(Window_.XDisplay, RootWindow(Window_.XDisplay, vInfo->screen), vInfo->visual, AllocNone);
   Window_.XWindow = XCreateWindow(Window_.XDisplay, RootWindow(Window_.XDisplay, vInfo->screen),
                           0, 0, 1920, 1080, 0, vInfo->depth, InputOutput,
                           vInfo->visual, CWEventMask | CWColormap, &setWindowAttributes);
   XMapWindow(Window_.XDisplay, Window_.XWindow);

   Window_.glxWindow = glXCreateWindow(Window_.XDisplay, glxfbConfig[0], Window_.XWindow, NULL);
   Window_.glxContext = glXCreateNewContext(Window_.XDisplay, glxfbConfig[0], GLX_RGBA_TYPE, NULL, True);
   if(!glXMakeContextCurrent(Window_.XDisplay, Window_.glxWindow, Window_.glxWindow, Window_.glxContext))
      throw std::runtime_error("Error making glxContext current in LinuxPlatform constructor");

   // ImGui Setup
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // ImGuiConfigFlags_NavEnableGamepad
   ImGui::StyleColorsDark();

   // platform and graphics backend stuff
   ImGui_ImplX11_InitForOpenGL(Window_.XDisplay, &Window_.XWindow);
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

LinuxPlatform::~LinuxPlatform(){
   if(Window_.XDisplay)
      XCloseDisplay(Window_.XDisplay);

   // TODO: Anything else? Window auto closes I believe?
}

Fig::EventQueue LinuxPlatform::PollEvents(){
   Fig::EventQueue queue { };
   while(XPending(Window_.XDisplay) > 0){
      XEvent event {};
      XNextEvent(Window_.XDisplay, &event);
      ImGui_ImplX11_ProcessEvent(&event);

      if(ApplicationHandleEvent(&event))
         queue.push_back(TranslateXEventToFigEvent(&event));
   }

   return queue;
}

void LinuxPlatform::SwapBuffers(){
   glXSwapBuffers(Window_.XDisplay, Window_.glxWindow);
}

void LinuxPlatform::NewImGuiFrame(){
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplX11_NewFrame();
   ImGui::NewFrame();
}

void LinuxPlatform::RenderImguiDrawData(){
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

///////////////////////
/// Private Methods ///
///////////////////////

bool LinuxPlatform::ApplicationHandleEvent(const XEvent* event){
   ImGuiIO& io = ImGui::GetIO();

   // TODO: io.WantSetMousePos: MousePos has been altered, backend should reposition mouse on next frame. Rarely used! Set only when io.ConfigNavMoveSetMousePos is enabled.
   
   // ImGui can want: 
      // CaptureMouse
      // CaptureKeyboard 
   switch(event->type){
      case(KeyPress):
      case(KeyRelease):
         if(io.WantCaptureKeyboard)
            return false;
         break;
      case(ButtonPress):
      case(ButtonRelease):
      case(MotionNotify):
         if(io.WantCaptureMouse)
            return false;
         break;
   };

   // TODO: would this be affected by other events like Enter/LeaveNotify? 
   // rn we are just checkign the basic ones which are obviosu but not sure about other ones
   // TODO: Might be a good idea to look more in depth later

   return true;
}

Fig::Event LinuxPlatform::TranslateXEventToFigEvent(const XEvent* event){
   Fig::Event Fevent { };
   switch(event->type){
      case(KeyPress):
         Fevent.Type = Fig::EventType::KEY_PRESS;
         break;
      case(KeyRelease):
         Fevent.Type = Fig::EventType::KEY_RELEASE;
         break;
      case(ButtonPress):
         Fevent.Type = Fig::EventType::BUTTON_PRESS;
         break;
      case(ButtonRelease):
         Fevent.Type = Fig::EventType::BUTTON_RELEASE;
         break;
      case(ConfigureNotify):
         // TODO: see X docs, but this does not mean there is necessarily a resize. 
         Fevent.Type = Fig::EventType::WINDOW_RESIZE;
         break;
      default: 
         Fevent.Type = Fig::EventType::NULL_EVENT;
         break;
   }

   return Fevent;
}
