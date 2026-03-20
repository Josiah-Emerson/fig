#pragma once

#include "Events.h"
#include "Layer.h"
#include "Core_Graphics/tempGLDevice.cpp"

class TempLayer : public Core::Layer{
   public: 
      TempLayer() 
         : m_gl {}
      { }

      bool onEvent(Core::Events::Event& event) override{
         if(event.type == Core::Events::KEY_PRESS){
            if(event.keyEvent.key == Core::Events::Key::OTHER){
               std::cout << "In here\n";
               m_gl.printVersion();
               return true;
            }
         }

         return false;
      }

   private: 
      Core::GL m_gl;
};
