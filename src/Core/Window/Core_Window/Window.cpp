#include "Window.h"
#include <imgui.h>

// TODO: these includes 
#if defined(WIN32)
#elif defined(LINUX)
#include "LinuxWindow.h"
#elif defined(OSX)
#endif

namespace Core{
   // NOTE: spec has default value 
   Window::Window(const WindowSpec& spec)
      : m_windowSpec { spec }
   { }

   std::shared_ptr<Window> Window::createWindow(const WindowSpec& spec ){
      std::shared_ptr<Window> ret = nullptr;

      // TODO: add other OS's when ready
#if defined(WIN32)
#elif defined(LINUX)
      ret = std::make_shared<LinuxWindow>(spec);
#elif defined(OSX)
#endif
      ret->initImGui(); // NOTE: If this gets moved from here, need to think about where to put it otherwise there will likely be calls to an unitialized imgui (for example in raise event)
      return ret;
   }
} // namespace Core
