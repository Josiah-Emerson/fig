#include "GuiTestLayer.h"
#include <memory>

GuiTestLayer::GuiTestLayer(){
   m_gui = std::make_unique<Core::Gui>();
}
GuiTestLayer::~GuiTestLayer() = default;

void GuiTestLayer::onUpdate(float dt){
   std::cout << "GuiTestLayer::onUpdate\n";
}

void GuiTestLayer::onRender(){
   std::cout << "GuiTestLayer::onRender\n";
}
