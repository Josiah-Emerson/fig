#include "Application.h"
#include "Platform/LinuxPlatform.h"

int main(){
   Application app { std::make_unique<LinuxPlatform>()};
   app.run();

   return 0;
}
