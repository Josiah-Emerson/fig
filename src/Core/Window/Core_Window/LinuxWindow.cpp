#include "LinuxWindow.h"
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cassert>
#include <stdexcept>

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
      glXSwapBuffers(m_XDisplay, m_glxWindow);
   }

   void LinuxWindow::pollEvents(){
      while(XPending(m_XDisplay) > 0){
         XEvent event {};
         XNextEvent(m_XDisplay, &event);
         // ImGui_ImplX11_ProcessEvent(&event);
         // TODO: The funky part here is that we need to be able to pass imgui the XEvent
         // Does it matter that we pass it here? Will layers be able to check if imgui wants it? What if imgui isn't init here?
         // Do we need to pass underlying XEvent as part of the Core::Events::Event? How would this work for other OS's which don't need it passed in automatically?
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

   Events::Key LinuxWindow::translateXKeyToFigKey(XKeyEvent* keyEvent){
      KeySym keySym = XLookupKeysym(keyEvent, 0);

      using namespace Events;
      switch(keySym){
         case(XK_Escape): return Key::ESCAPE;
         default: return Key::OTHER;
      }
   }

} // namespace Core
