#include "GuiTestLayer.h"

GuiTestLayer::GuiTestLayer()
   : m_gui { }
{}
GuiTestLayer::~GuiTestLayer() = default;

void GuiTestLayer::onUpdate(float dt){
}

void GuiTestLayer::onRender(){
   static bool open = true;
   m_gui.beginFrame();
   if(open){
      Core::Gui::Window window {&m_gui, "Hello, World!", open, {200, 400}, {0,0}};
      window.text("Hello!");
   }
   m_gui.render();
}
