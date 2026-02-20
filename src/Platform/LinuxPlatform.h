#pragma once
#include "Platform.h"
#include <GL/glx.h>
#include <X11/Xlib.h>

class LinuxPlatform : public Platform {
   private: 
      struct LinuxWindow{
         Display* XDisplay;
         Window XWindow; 

         // TODO: Should these be here? Lowkey probably not
         GLXWindow glxWindow;
         GLXContext glxContext;
      } Window_;

   public: 
      LinuxPlatform();
      ~LinuxPlatform(); // TODO: implement big 5

      Fig::EventQueue PollEvents() override;
      void SwapBuffers() override;
      void NewImGuiFrame() override;
      void RenderImguiDrawData() override;

   private: 
      bool ApplicationHandleEvent(const XEvent* event);
      Fig::Event TranslateXEventToFigEvent(const XEvent* event);
};
