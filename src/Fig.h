#pragma once

#include <vector>
namespace  Fig{
   enum class EventType{
      NULL_EVENT, // Should only happen if a translate to fig event cannot figure out what to translate to
      KEY_PRESS,
      KEY_RELEASE,
      BUTTON_PRESS,
      BUTTON_RELEASE,
      WINDOW_RESIZE,
   };

   struct NullEvent{ // if translater cannot figure out to translate native event into Fig::Event
      EventType Type { EventType::NULL_EVENT };
   };

   struct KeyEvent{
      EventType Type; // KEY_PRESS or KEY_RELEASE
   };

   struct ButtonEvent{
      EventType Type; // BUTTON_PRESS or BUTTON_RELEASE

   };

   struct WindowResizeEvent{
      EventType Type;  

   };

   union Event{
      EventType Type; // common for all Fig::Event structures
      NullEvent nullEvent;
      KeyEvent FKey;
      ButtonEvent FButton;
      WindowResizeEvent FWindowResize;
   };

   using EventQueue = std::vector<Event>;
}
