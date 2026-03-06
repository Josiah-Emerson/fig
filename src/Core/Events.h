#pragma once
#include <iostream>

namespace Core{
   namespace Events{
      enum Type{
         NULL_EVENT, // Should only happen if a translate to fig event cannot figure out what to translate to
         KEY_PRESS,
         KEY_RELEASE,
         BUTTON_PRESS,
         BUTTON_RELEASE,
         MOUSE_MOVEMENT,
         WINDOW_RESIZE, 
         WINDOW_MOVE,
         WINDOW_OTHER, // TODO: Just here for now to indicate a window structure change thats not WINDOW_MOVE or WINDOW_RESIZE
                       // NOTE: This is coupled heavily to the X reports events. Might need to look into how windows does it 
      };

      enum class Key{
         ESCAPE,
         OTHER,
      };

      struct NullEvent{ // if translator cannot figure out to translate native event into Core::Event
         Type type { NULL_EVENT };
      };

      struct KeyEvent{
         Type type; // KEY_PRESS or KEY_RELEASE
         Key key;
      };

      struct ButtonEvent{
         Type type; // BUTTON_PRESS or BUTTON_RELEASE
      };

      struct MouseMotionEvent{
         Type type { MOUSE_MOVEMENT }; // MOUSE_MOVEMENT
      };

      struct WindowChangeEvent{
         Type type; // WINDOW_RESIZE or WINDOW_MOVE or WINDOW_OTHER
      };

      union Event{
         Type type; // common for all Core::Events::Event structures
         NullEvent nullEvent;
         KeyEvent keyEvent;
         ButtonEvent buttonEvent;
         MouseMotionEvent mouseMotionEvent;
         WindowChangeEvent windowChangeEvent;

         Event(){ // TODO: constructor is needed here for LinuxWindow to be able to create a dynamically allocated array of these events
            type = Type::NULL_EVENT;
         }

         // TODO: Delete this when we don't need it 
         void toString(){
            switch(type){
               case(NULL_EVENT):
                  std::cout << "NULL_EVENT\n";
                  break;
               case(KEY_PRESS):
                  std::cout << "KEY_PRESS\n";
                  break;
               case(KEY_RELEASE):
                  std::cout << "KEY_RELEASE\n";
                  break;
               case(BUTTON_PRESS):
                  std::cout << "BUTTON_PRESS\n";
                  break;
               case(BUTTON_RELEASE):
                  std::cout << "BUTTON_RELEASE\n";
                  break;
               case(MOUSE_MOVEMENT):
                  std::cout << "MOUSE_MOVEMENT\n";
                  break;
               case(WINDOW_RESIZE):
                  std::cout << "WINDOW_RESIZE\n";
                  break;
               case(WINDOW_MOVE):
                  std::cout << "WINDOW_MOVE\n";
                  break;
               case(WINDOW_OTHER):
                  std::cout << "WINDOW_OTHER\n";
                  break;
               default: 
                  std::cout << "WTF!!\n";
                  break;
            }
         } // End of TODO: Delete

      };

   } // Namespace Events
} // Namespace Type
