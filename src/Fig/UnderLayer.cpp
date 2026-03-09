#include "UnderLayer.h"

UnderLayer::UnderLayer()
   : m_imGuiContainer { }
{ }

bool UnderLayer::onEvent(Core::Events::Event& event) {
   event.print();
   return true;
}

void UnderLayer::onUpdate(){
   m_imGuiContainer.draw();
}

void UnderLayer::onRender(){

}
