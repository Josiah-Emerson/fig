#pragma once

#include <vector>
namespace  Fig{
   enum class EventType{
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

   struct NullEvent{ // if translater cannot figure out to translate native event into Fig::Event
      EventType Type { EventType::NULL_EVENT };
   };

   struct KeyEvent{
      EventType Type; // KEY_PRESS or KEY_RELEASE
      Key key;
   };

   struct ButtonEvent{
      EventType Type; // BUTTON_PRESS or BUTTON_RELEASE 
   };

   struct MouseMotionEvent{
      EventType Type; // MOUSE_MOVEMENT
   };

   struct WindowChangeEvent{
      EventType Type; // WINDOW_RESIZE or WINDOW_MOVE or WINDOW_OTHER
   };

   union Event{
      EventType Type; // common for all Fig::Event structures
      NullEvent nullEvent;
      KeyEvent FKey;
      ButtonEvent FButton;
      MouseMotionEvent FMouseMotion;
      WindowChangeEvent FWindowChange;
   };

   using EventQueue = std::vector<Event>;

}
