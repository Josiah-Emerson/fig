#include "Application.h"
// #include "CameraRotateLayer.h"
#include "GuiTestLayer.h"
#include "OverlayLayer.h"

int main(){
   Core::ApplicationSpec appSpec;
   appSpec.title = "Josiah's Fig";

   Core::Application app { appSpec };
   app.pushLayer<OverlayLayer>();
   app.pushLayer<GuiTestLayer>();
   // app.pushLayer<CameraRotateLayer>();
   app.run();
}
