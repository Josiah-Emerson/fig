#include "TestPrintLayer.h"

bool TestPrintLayer::onEvent(Core::Events::Event& event){
   event.toString();
   return true;
}

void TestPrintLayer::onUpdate(){
   std::cout << "Update\n";
}

void TestPrintLayer::onRender(){
   std::cout << "Render\n";
}
