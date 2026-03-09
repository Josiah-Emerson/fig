#include "OverlayLayer.h"
#include "Events.h"
#include "Application.h"

bool OverlayLayer::onEvent(Core::Events::Event& event){
   using namespace Core::Events;

   switch(event.type){
      case(KEY_PRESS):
         if(event.keyEvent.key == Key::ESCAPE){
            Core::Application::get().stop();
            return true;
         }

         break;
      default:
         break;
   }

   return false;
}
