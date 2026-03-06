#pragma once
#include "Fig_Windows/IWindow.h"
#include <GL/glx.h>
#include <X11/Xlib.h>

class LinuxRootWindow : public PlatformRootWindow {
   public: 
      LinuxRootWindow();
      ~LinuxRootWindow(); // TODO: implement big 5

      bool isVisible() override;
      bool updatePos(Fig::Vec2i pos) override;
      bool updateSize(Fig::Vec2i size) override;
      Fig::EventQueue pollEvents() override;
      void swapBuffers() override;
      void newImGuiFrame() override;
      void renderImGuiDrawData() override;

   private: 
      // This indicates that ImGui (or other) does not want the input and thus the application should handle it (or decide not to handle)
      bool applicationHandleEvent(const XEvent* event);
      Fig::EventQueue translateXEventToFigEventQueue(XEvent* event);
      Fig::Key translateXKeyToFigKey(XKeyEvent* keyEvent);

   private: 
      Display* XDisplay_;
      Window XWindow_;
      GLXWindow glxWindow_;
      GLXContext glxContext_;
};
