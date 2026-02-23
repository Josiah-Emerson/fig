#pragma once
#include "Platform.h"
#include <GL/glx.h>
#include <X11/Xlib.h>

class LinuxPlatform : public Platform {
   private: 
      struct LinuxWindow{
         Display* XDisplay;
         Window XWindow; 
         int x, y;
         int width, height;

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
      void RenderImGuiDrawData() override;
      int GetWindowHeight() override { return Window_.height; }
      int GetWindowWidth() override { return Window_.width; }

   private: 
      // This indicates that ImGui (or other) does not want the input and thus the application should handle it (or decide not to handle)
      bool ApplicationHandleEvent(const XEvent* event);
      Fig::EventQueue TranslateXEventToFigEventQueue(XEvent* event);
      Fig::Key TranslateXKeyToFigKey(XKeyEvent* keyEvent);
      void updateWindowPosition(int x, int y);
      void updateWindowSize(int width, int height);
};
