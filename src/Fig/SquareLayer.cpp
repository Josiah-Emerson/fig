#include "SquareLayer.h"

// NOTE: numSquares has default arg
SquareLayer::SquareLayer(const int numSquares)
   : m_propertyEditor { nullptr }
   , m_rootSceneNode { buildSceneTree(numSquares) }
{ }

bool SquareLayer::onEvent(Core::Events::Event& event){
   return false;
}

void SquareLayer::onUpdate(){

}

void SquareLayer::onRender(){
   if(m_propertyEditorOpen)
      m_propertyEditor->draw();

   // draw squares
}
