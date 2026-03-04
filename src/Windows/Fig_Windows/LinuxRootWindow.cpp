#include "LinuxRootWindow.h"
#include <stdexcept> // std::runtime_error
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_x11.h>

LinuxRootWindow::LinuxRootWindow()
   : PlatformRootWindow { }
{
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

   int numElements;
   GLXFBConfig* glxfbConfig = glXChooseFBConfig(XDisplay_, XDefaultScreen(XDisplay_), glxfbAttributeList, &numElements);
   // TODO: is throwing an error best way for this? Init func? IDK
   if(!glxfbConfig)
      throw std::runtime_error("Error choosing framebuffer config for GLX in LinuxPlatform constructor");
   XVisualInfo* vInfo = glXGetVisualFromFBConfig(XDisplay_, glxfbConfig[0]);

   XSetWindowAttributes setWindowAttributes { };
   // TODO: I think comment in IMGUI x11 implementation, but these are all needed for IMGUI to work, 
   // however, there might be a way for imgui impl to just subscribe to events it needs on its own,
   // and thus we can only define ones we want here? TBD
   setWindowAttributes.event_mask = ButtonPressMask | ButtonReleaseMask |
                                    KeyPressMask | KeyReleaseMask |
                                    StructureNotifyMask | PointerMotionMask | 
                                    EnterWindowMask | LeaveWindowMask;
   setWindowAttributes.colormap = XCreateColormap(XDisplay_, RootWindow(XDisplay_, vInfo->screen), vInfo->visual, AllocNone);
   XWindow_ = XCreateWindow(XDisplay_, RootWindow(XDisplay_, vInfo->screen),
                           0, 0, 1920, 1080, 0, vInfo->depth, InputOutput,
                           vInfo->visual, CWEventMask | CWColormap, &setWindowAttributes);
   // TODO: NOT A GOOD WAY TO SET THESE HERE OR IN CREATE WINDOW FUNC
   updatePos({0, 0});
   updateSize({ 0, 0 });
   XMapWindow(XDisplay_, XWindow_);

   glxWindow_ = glXCreateWindow(XDisplay_, glxfbConfig[0], XWindow_, NULL);
   glxContext_ = glXCreateNewContext(XDisplay_, glxfbConfig[0], GLX_RGBA_TYPE, NULL, True);
   if(!glXMakeContextCurrent(XDisplay_, glxWindow_, glxWindow_, glxContext_))
      throw std::runtime_error("Error making glxContext_ current in LinuxPlatform constructor");

   // ImGui Setup
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // ImGuiConfigFlags_NavEnableGamepad
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

LinuxRootWindow::~LinuxRootWindow(){
   if(XDisplay_)
      XCloseDisplay(XDisplay_);

   // TODO: Anything else? Window auto closes I believe?
}

bool LinuxRootWindow::isVisible() { return true; } // TODO: actual logic
                                  
// TODO: actual logic (maybe define some stuff in IWindow or PlatformRootWindow?)
bool LinuxRootWindow::updatePos(Fig::Vec2i pos){
   pos_ = pos;
   return true;
}

// TODO: logic
bool LinuxRootWindow::updateSize(Fig::Vec2i size){
   size_ = size;
   return true;
}

Fig::EventQueue LinuxRootWindow::pollEvents(){
   Fig::EventQueue queue { };
   while(XPending(XDisplay_) > 0){
      XEvent event {};
      XNextEvent(XDisplay_, &event);
      ImGui_ImplX11_ProcessEvent(&event);

      if(applicationHandleEvent(&event)){
         Fig::EventQueue tempQueue  = translateXEventToFigEventQueue(&event); // TODO: is this performant enough? Does it matter?
         for(Fig::Event e : tempQueue){
            queue.push_back(e);
         }
      }
   }

   return queue;
}

void LinuxRootWindow::swapBuffers(){
   glXSwapBuffers(XDisplay_, glxWindow_);
}

void LinuxRootWindow::newImGuiFrame(){
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplX11_NewFrame();
   ImGui::NewFrame();
}

void LinuxRootWindow::renderImGuiDrawData(){
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool LinuxRootWindow::applicationHandleEvent(const XEvent* event){
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

Fig::EventQueue LinuxRootWindow::translateXEventToFigEventQueue(XEvent* event){
   Fig::Event Fevent { };
   Fig::EventQueue queue { };

   switch(event->type){
      case(KeyPress):
      case(KeyRelease):
         Fevent.Type = event->type == KeyPress ? Fig::EventType::KEY_PRESS : Fig::EventType::KEY_RELEASE;
         Fevent.FKey.key = translateXKeyToFigKey(&event->xkey);
         queue.push_back(Fevent);
         break;
      case(ButtonPress):
         Fevent.Type = Fig::EventType::BUTTON_PRESS;
         queue.push_back(Fevent);
         break;
      case(ButtonRelease):
         Fevent.Type = Fig::EventType::BUTTON_RELEASE;
         queue.push_back(Fevent);
         break;
      case(ConfigureNotify):
         if(event->xconfigure.x != pos_.x || event->xconfigure.y != pos_.y){
            Fevent.Type = Fig::EventType::WINDOW_MOVE;
            updatePos({ event->xconfigure.x, event->xconfigure.y} );
            queue.push_back(Fevent);
         }

         if(event->xconfigure.width != size_.x || event->xconfigure.height != size_.y){
            Fevent.Type = Fig::EventType::WINDOW_RESIZE;
            updateSize({ event->xconfigure.width, event->xconfigure.height });
            queue.push_back(Fevent);
         }

         if(queue.size() == 0){ // TODO: Temp for there is a chance we get a ConfigureNotify event which is not a window move or resize
            Fevent.Type = Fig::EventType::WINDOW_OTHER;
            queue.push_back(Fevent);
         }
         break;
      case(MotionNotify):
         Fevent.Type = Fig::EventType::MOUSE_MOVEMENT;
         queue.push_back(Fevent);
         break;
      default: 
         Fevent.Type = Fig::EventType::NULL_EVENT;
         queue.push_back(Fevent);
         break;
   }

   return queue;
}

Fig::Key LinuxRootWindow::translateXKeyToFigKey(XKeyEvent* keyEvent){
   KeySym keySym = XLookupKeysym(keyEvent, 0);

   using namespace Fig;
   switch(keySym){
      case(XK_Escape): return Key::ESCAPE;
      default: return Key::OTHER;
   }
}
