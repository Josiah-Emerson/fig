#include "Application.h"
#include "OverlayLayer.h"
#include "UncaughtEventsOutputLayer.h"
#include "TestLayer.h"

int main(){
   Core::ApplicationSpec appSpec;
   appSpec.title = "Josiah's Fig";
   // appSpec.windowSpec.width = 1920/2;
   // appSpec.windowSpec.height = 1080/2;

   Core::Application app { appSpec };
   app.pushLayer<OverlayLayer>();
   app.pushLayer<TestLayer>();
   app.pushLayer<UncaughtEventsOutputLayer>(); 
   app.run();
}
