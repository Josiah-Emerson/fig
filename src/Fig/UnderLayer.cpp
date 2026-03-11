#include "UnderLayer.h"
#include <GL/gl.h>

UnderLayer::UnderLayer()
   : m_imGuiPropertyEditor { nullptr }
{ 
   using namespace Core;
   ImGuiPropertyEditor::DataNode rootNode;
   ImGuiPropertyEditor::DataInfo redData{
      "Red",
      ImGuiDataType_S32,
      &m_red
   };
   ImGuiPropertyEditor::DataInfo blueData{
      "Blue",
      ImGuiDataType_S32,
      &m_blue
   };
   ImGuiPropertyEditor::DataInfo greenData{
      "Green",
      ImGuiDataType_S32,
      &m_green
   };
   rootNode.name = "Colors";
   rootNode.dataInfoArr.push_back( std::make_shared<ImGuiPropertyEditor::DataInfo>(redData) );
   rootNode.dataInfoArr.push_back( std::make_shared<ImGuiPropertyEditor::DataInfo>(blueData) );
   rootNode.dataInfoArr.push_back( std::make_shared<ImGuiPropertyEditor::DataInfo>(greenData) );

   m_imGuiPropertyEditor = std::make_unique<ImGuiPropertyEditor>(&rootNode, &m_open);
}

bool UnderLayer::onEvent(Core::Events::Event& event) {
   event.print();
   return true;
}

void UnderLayer::onUpdate(){
   if(m_open)
      m_imGuiPropertyEditor->draw();
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
