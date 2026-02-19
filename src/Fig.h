#pragma once

enum class FigEventType{
   KEY_PRESS,
   KEY_RELEASE,
   BUTTON_PRESS,
   BUTTON_RELEASE,
   WINDOW_RESIZE,
};

struct FigKeyEvent{
   FigEventType Type; // KEY_PRESS or KEY_RELEASE
};

struct FigButtonEvent{
   FigEventType Type; // BUTTON_PRESS or BUTTON_RELEASE

};

struct FigWindowResizeEvent{
   FigEventType Type;  

};

union FigEvent{
   FigEventType Type; // common for all FigEvent structures
   FigKeyEvent FKey;
   FigButtonEvent FButton;
   FigWindowResizeEvent FWindowResize;
};

