#include "Application.h"
#include "CameraRotateLayer.h"
#include "OverlayLayer.h"
#include "UncaughtEventsOutputLayer.h"

int main(){
   Core::ApplicationSpec appSpec;
   appSpec.title = "Josiah's Fig";

   Core::Application app { appSpec };
   app.pushLayer<OverlayLayer>();
   app.pushLayer<CameraRotateLayer>();
   app.pushLayer<UncaughtEventsOutputLayer>(); 
   app.run();
}
