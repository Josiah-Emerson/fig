#include "Application.h"
#include "TestPrintLayer.h"

int main(){
   Core::ApplicationSpec appSpec;
   appSpec.title = "Josiah's Fig";
   /*
   appSpec.windowSpec.width = 1920/2;
   appSpec.windowSpec.height = 1080/2
   */

   Core::Application app { appSpec };
   app.pushLayer<TestPrintLayer>();
   app.run();
}
