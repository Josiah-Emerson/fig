#include "Application.h"
#include "CameraRotateLayer.h"
#include "OverlayLayer.h"
#include "TestLayer2.h"
#include "UncaughtEventsOutputLayer.h"
#include "TestLayer.h"

int main(){
   Core::ApplicationSpec appSpec;
   appSpec.title = "Josiah's Fig";
   // appSpec.windowSpec.width = 1920/2;
   // appSpec.windowSpec.height = 1080/2;

   Core::Application app { appSpec };
   app.pushLayer<OverlayLayer>();
   // app.pushLayer<TestLayer>();
   // app.pushLayer<TestLayer2>();
   app.pushLayer<CameraRotateLayer>();
   app.pushLayer<UncaughtEventsOutputLayer>(); 
   app.run();
}
