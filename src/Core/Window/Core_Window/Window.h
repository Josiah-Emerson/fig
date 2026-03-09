#pragma once
#include "../Events.h"
#include <functional> // std::function
#include <memory>

namespace Core{
   struct WindowSpec{
      int width = 1920;
      int height = 1080;
      int x = 0;
      int y = 0; // TODO: I believe this is relative to something for X Server. Should test to see differneces in OS's

      // TODO: This gives a way for the window to raise events to the application. 
      // i.e. app calls void pollEvents() and the window calls this callback with event 
      // What advantage (or disadvantages) does this have from having pollEvents return an array of events?
      std::function<void(Core::Events::Event& event)> eventCallback; 
   };

   class Window{
      public: 
         Window(const WindowSpec& spec = WindowSpec());
         virtual ~Window() = default;

         // virtual void create(); // TODO: most of it is done in constructor
         // virtual void destroy(); // TODO: done by destructor?
         virtual void update() = 0; // swapBuffers
         virtual void pollEvents() = 0; // For now this should be common window function regardless of OS
         virtual void raiseEvent(Core::Events::Event& event) = 0;
         virtual void newImGuiFrame() = 0;

         // does ifdefs to return proper platform window
         // returns nullptr if nothing
         static std::shared_ptr<Window> createWindow(const WindowSpec& spec = WindowSpec() );

         // TODO: Funcs from cherno's architeture series 
         // getFrameBufferSize();
         // getMousePos;
         // bool shouldClose()
         // getHandle(); 

      protected: 
         virtual void initImGui() = 0;
         virtual void updatePosition(int x, int y) { 
            m_windowSpec.x = x; 
            m_windowSpec.y = y; 
         }

         virtual void updateSize(int width, int height) { 
            m_windowSpec.width = width; 
            m_windowSpec.height = height; 
         }

      protected: 
         WindowSpec m_windowSpec;
   };

} // namespace Core
