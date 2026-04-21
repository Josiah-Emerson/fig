#include "Application.h"
#include "OverlayLayer.h"
#include "UncaughtEventsOutputLayer.h"
#include "VectorTestLayer.h"

int main(){
   Core::ApplicationSpec appSpec;
   appSpec.title = "Josiah's Fig";
   // appSpec.windowSpec.width = 1920/2;
   // appSpec.windowSpec.height = 1080/2;

   Core::Application app { appSpec };
   app.pushLayer<OverlayLayer>();
   std::cout << "Pushing VectorTestLayer\n";
   app.pushLayer<VectorTestLayer>();
   std::cout << "Finished pushing VectorTestLayer\n";
   app.pushLayer<UncaughtEventsOutputLayer>(); 
   app.run();
}
