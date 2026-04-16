#include "LinuxWindow.h"
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cassert>
#include <cstring>
#include <stdexcept>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_x11.h>

namespace Core{
   // NOTE: spec can be a default param (see header file), but cannot be redefined here
   LinuxWindow::LinuxWindow(const WindowSpec& spec )
      : Window { spec }
   {
      m_XDisplay = XOpenDisplay(NULL);

      // TODO: More flexible way to choose options (i.e. if these aren't available)
      int glxfbAttributeList[] = {GLX_DOUBLEBUFFER, True,
                                 GLX_RED_SIZE, 8,
                                 GLX_GREEN_SIZE, 8,
                                 GLX_BLUE_SIZE, 8,
                                 GLX_ALPHA_SIZE, 8,
                                 GLX_X_RENDERABLE, True, // TODO: Does this matter?
                                 GLX_STENCIL_SIZE, 8,
                                 GLX_DEPTH_SIZE, 24,
                                 None};
      int numElements;
      GLXFBConfig* glxfbConfig = glXChooseFBConfig(m_XDisplay, XDefaultScreen(m_XDisplay), glxfbAttributeList, &numElements);
      // TODO: is throwin an error best way for this? init func returning bool better? idk
      if(!glxfbConfig)
         throw std::runtime_error("Error choosing framebuffer config for GLX in LinuxWindow constructor.");
      XVisualInfo* vInfo = glXGetVisualFromFBConfig(m_XDisplay, glxfbConfig[0]);

      XSetWindowAttributes setWindowAttributes { };
      // TODO: I think comment in IMGUI x11 implementation, but these are all needed for IMGUI to work, 
      // however, there might be a way for imgui impl to just subscribe to events it needs on its own,
      // and thus we can only define ones we want here? TBD
      setWindowAttributes.event_mask = ButtonPressMask | ButtonReleaseMask |
                                       KeyPressMask | KeyReleaseMask |
                                       StructureNotifyMask | PointerMotionMask | 
                                       EnterWindowMask | LeaveWindowMask;
      setWindowAttributes.colormap = XCreateColormap(m_XDisplay, RootWindow(m_XDisplay, vInfo->screen), vInfo->visual, AllocNone);

      m_XWindow = XCreateWindow(m_XDisplay, RootWindow(m_XDisplay, vInfo->screen), 
            spec.x, spec.y, spec.width, spec.height, 0, vInfo->depth, InputOutput, 
            vInfo->visual, CWEventMask | CWColormap, &setWindowAttributes);

      XMapWindow(m_XDisplay, m_XWindow);

      m_glxWindow = glXCreateWindow(m_XDisplay, glxfbConfig[0], m_XWindow, NULL);
      m_glxContext = glXCreateNewContext(m_XDisplay, glxfbConfig[0], GLX_RGBA_TYPE, NULL, True);
      if(!glXMakeContextCurrent(m_XDisplay, m_glxWindow, m_glxWindow, m_glxContext))
         throw std::runtime_error("Error making m_glxContext current in LinuxWindow constructor");

      // TODO: Where should we put ImGui stuff?
   }

   LinuxWindow::~LinuxWindow(){
      if(m_XDisplay)
         XCloseDisplay(m_XDisplay);
      // TODO: Anything else? Window should auto close?
   }



   void LinuxWindow::update(){
      // TODO: Should ImGui::Render() be here?
      ImGui::Render();
      ImDrawData* imGuiDrawData = ImGui::GetDrawData();
      if(imGuiDrawData){
         ImGui_ImplOpenGL3_RenderDrawData(imGuiDrawData);
      }

      glXSwapBuffers(m_XDisplay, m_glxWindow);
   }

   void LinuxWindow::pollEvents(){
      while(XPending(m_XDisplay) > 0){
         XEvent event {};
         XNextEvent(m_XDisplay, &event);
         // TODO: The funky part here is that we need to be able to pass imgui the XEvent
         // Does it matter that we pass it here? Will layers be able to check if imgui wants it? What if imgui isn't init here?
         // Do we need to pass underlying XEvent as part of the Core::Events::Event? How would this work for other OS's which don't need it passed in automatically?
         // For now we will just pass the event here and the layer can either choose to listen if io wants it or not. 
         // NOTE: Possible issue: ImGui uses this event but a layer higher up which doesn't care about imgui handles the event. In this case both the layer 
         // and imgui 'handle' or use this event which is probably not what we want
         ImGui_ImplX11_ProcessEvent(&event);
         std::vector<Events::Event> events = translateXEventToFigEvents(&event);

         for(Events::Event& e : events){
            raiseEvent(e);
         }
      }
   }

   void LinuxWindow::raiseEvent(Core::Events::Event& event){
      if(m_windowSpec.eventCallback)
         m_windowSpec.eventCallback(event);
   }

   // PRIVATE METHODS
   std::vector<Events::Event> LinuxWindow::translateXEventToFigEvents(XEvent* event){
      std::vector<Events::Event> events;
      Events::Event figEvent;

      switch(event->type){
         case(KeyPress):
         case(KeyRelease):
            figEvent.type = event->type == KeyPress ? Events::KEY_PRESS : Events::KEY_RELEASE;
            figEvent.keyEvent.key = translateXKeyToFigKey(&event->xkey);
            events.push_back(figEvent);
            break;
         case(ButtonPress):
         case(ButtonRelease):
            figEvent.type = event->type == ButtonPress ? Events::BUTTON_PRESS : Events::BUTTON_RELEASE;
            events.push_back(figEvent);
            break;
         case(ConfigureNotify):
            if(event->xconfigure.x != m_windowSpec.x || event->xconfigure.y != m_windowSpec.y){
               figEvent.type = Events::WINDOW_MOVE;
               updatePosition( event->xconfigure.x, event->xconfigure.y ); // TODO: this should be here right?
               events.push_back(figEvent);
            }

            if(event->xconfigure.width != m_windowSpec.width || event->xconfigure.height != m_windowSpec.height){
               figEvent.type = Events::WINDOW_RESIZE;
               updateSize( event->xconfigure.width, event->xconfigure.height ); // TODO: this should be here right?
               events.push_back(figEvent);
            }

            if(events.size() == 0){ // TODO: Temp for if we geta ConfigureNotify event not handled by above two if statements
               figEvent.type = Events::WINDOW_OTHER;
               events.push_back(figEvent);
            }
            break;
         case(MotionNotify):
            figEvent.type = Events::MOUSE_MOVEMENT;
            events.push_back(figEvent);
            break;
         default: 
            figEvent.type = Events::NULL_EVENT;
            events.push_back(figEvent);
            break;
      }

      return events;
   }

   void LinuxWindow::newImGuiFrame(){
      ImGui_ImplOpenGL3_NewFrame(); // TODO: Coupled here to openGL for ImGui, but this should be easy enough to change when we add other support?
      ImGui_ImplX11_NewFrame();
      ImGui::NewFrame();
   }

   void (*LinuxWindow::getProcAddress(char* procName)) () {
      /*
      // TODO: Is this fine here in the LinuxWIndow? What if we don't use openGL?
      // Query extensions list to ensure it is supported
      const char* extensionsList = glXQueryExtensionsString(m_XDisplay, XDefaultScreen(m_XDisplay));
      char* extensionsCopy = new char[strlen(extensionsList) + 1]; // TODO: IMPORTANT!!! WHEN THIS FUNC GETS REFACTORED (and will probably best to store this as a member var of the window class?) move the creation and delete[] (if using c strings) into constructor and destructor!!!!
      strcpy(extensionsCopy, extensionsList);
      if(strcmp(extensionsCopy, "") == 0){ // empty string
         delete[] extensionsCopy;
         return nullptr;
      }
      char* token = strtok(extensionsCopy, " ");
      while(token){
         puts(token);
         if(strcmp(token, procName) == 0){ // procName is supported
            delete[] extensionsCopy;
            return glXGetProcAddress((GLubyte*) procName);
         }
         token = strtok(NULL, " ");
      }

      delete[] extensionsCopy;
      return nullptr;
      */ 
      // The above commented out code parses and extension list to see if the procName is supported in the available extensions. Could be useful in the future

      return glXGetProcAddress((GLubyte*) procName);
   }

   Events::Key LinuxWindow::translateXKeyToFigKey(XKeyEvent* keyEvent){
      KeySym keySym = XLookupKeysym(keyEvent, 0);

      using namespace Events;
      switch(keySym){
         case(XK_Escape): return Key::ESCAPE;
         default: return Key::OTHER;
      }
   }


   // TODO: If more complicated and need multiple different contexts, may need to move imgui from being a window controlled thing
   void LinuxWindow::initImGui(){
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // ImGuiConfigFlags_NavEnableGamepad
      ImGui::StyleColorsDark();

      ImGui_ImplX11_InitForOpenGL(m_XDisplay, &m_XWindow);
      ImGui_ImplOpenGL3_Init();
   }

} // namespace Core
