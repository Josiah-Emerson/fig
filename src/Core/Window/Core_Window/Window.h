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

   // TODO: Probably keep this setup, but for now Events are sent everytime theres an event 
   // Furthermore, for key presses, the Window implementation should send an event, and also 
   // set an array of bools which layers can use to check if that button is pressed. For the X 
   // windowing system this makes sense because button presses come in events. As for Windows/Mac 
   // I am not sure if it is events or its interface uses a polling system for checking which keys 
   // are pressed. If for example, Windows doesn't 'send and event' if a key is pressed, there will 
   // likely need to be extra logic for that impl to then send an event when that key is pressed
   // TODO: similar to above, the impl needs to set and update the m_pointerPosition. Not sure if 
   // this is best approach or what

   class Window{
      public: 
         // Upper left is (0, 0) and increases down
         struct PointerPosition { float x, y; };

         Window(const WindowSpec& spec = WindowSpec());
         virtual ~Window() = default;

         // virtual void create(); // TODO: most of it is done in constructor
         // virtual void destroy(); // TODO: done by destructor?
         virtual void update() = 0; // swapBuffers
         virtual void pollEvents() = 0; // For now this should be common window function regardless of OS
         virtual void raiseEvent(Core::Events::Event& event) = 0;
         virtual void newImGuiFrame() = 0;
         virtual int getWidth() { return m_windowSpec.width; }
         virtual int getHeight() { return m_windowSpec.height; }
         
         // TODO: add logic for ensuring new position works 
         virtual void setPointerPosition(PointerPosition newPos);

         // return procedure address of a specified procedure
         // returns nullptr if address not found
         // NOTE: This may be deeply coupled to openGL loading functions but for now oh well
         virtual void (*getProcAddress(char* procName)) () = 0;

         bool isKeyDown(Events::Key key) const;
         const PointerPosition& pointerPosition() const;

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

         virtual void internalSetPointerPosition(PointerPosition newPos) = 0;

      protected: 
         WindowSpec m_windowSpec;
         bool m_keyState[static_cast<std::size_t>(Events::Key::OTHER) + 1] { false };
         PointerPosition m_pointerPosition;
   };

} // namespace Core
