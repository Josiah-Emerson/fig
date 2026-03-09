#pragma once
#include "Window.h"
#include <GL/glx.h>
#include <X11/Xlib.h>

namespace Core{
   class LinuxWindow : public Window {
      public: 
         LinuxWindow(const WindowSpec& spec = WindowSpec() );
         ~LinuxWindow();

         void update() override; // swapBuffers
         void pollEvents() override;
         void raiseEvent(Events::Event& event) override; // TODO: why public?
         void newImGuiFrame() override;

      private: 
         // returns an array of Core::Events::Event of size out_numEvents in out_events
         // NOTE: delete[] out_events because out_events is dynamically allocated on heap
         std::vector<Events::Event> translateXEventToFigEvents(XEvent* event);
         Events::Key translateXKeyToFigKey(XKeyEvent* keyEvent);
         void initImGui() override;

      private:
         Display* m_XDisplay;
         ::Window m_XWindow; // We want X window not Core::Window
         GLXWindow m_glxWindow;
         GLXContext m_glxContext;
   };
} // namespace Core
