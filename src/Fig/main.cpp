#include "Application.h"
#include "CameraRotateLayer.h"
#include "GuiTestLayer.h"
#include "OverlayLayer.h"
#include "UncaughtEventsOutputLayer.h"

int main(){
   Core::ApplicationSpec appSpec;
   appSpec.title = "Josiah's Fig";

   Core::Application app { appSpec };
   app.pushLayer<OverlayLayer>();
   app.pushLayer<GuiTestLayer>();
   app.pushLayer<CameraRotateLayer>();
   app.pushLayer<UncaughtEventsOutputLayer>(); 
   app.run();
}
