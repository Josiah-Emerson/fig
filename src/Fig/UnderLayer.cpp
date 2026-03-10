#include "UnderLayer.h"
#include <GL/gl.h>

UnderLayer::UnderLayer()
   : m_imGuiContainer { m_red, m_green, m_blue }
{ }

bool UnderLayer::onEvent(Core::Events::Event& event) {
   event.print();
   return true;
}

void UnderLayer::onUpdate(){
   m_imGuiContainer.draw();
}

void UnderLayer::onRender(){
   glClearColor(  clampAndNormalize(&m_red),
                  clampAndNormalize(&m_green),
                  clampAndNormalize(&m_blue),
                  0 );

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float UnderLayer::clampAndNormalize(int* val){
   if(*val < 0){
      *val = 0;
   } else if(*val > 255){
      *val = 255;
   }

   return *val / 255.f;

}
