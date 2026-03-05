#include "Application.h"
#if defined(WIN32)// TODO: Update to Fig_Window/RootWindow/XXXRootWindow.h if we stick with it
#include "Fig_Platform/Win32Platform.h"
#elif defined(LINUX)
#include "Fig_Windows/RootWindow/LinuxRootWindow.h"
#elif defined(OSX)
#include "Fig_Platform/OSXPlatform.h"
#endif

int main(){
   std::unique_ptr<PlatformRootWindow> platform;
#if defined(WIN32)
   platform = std::make_unique<Win32Platform>();
#elif defined(LINUX)
   platform = std::make_unique<LinuxRootWindow>();
#elif defined(OSX)
   platform = std::make_unique<OSXPlatform>();
#endif

   Application app { std::move(platform) };
   app.run();

   return 0;
}
