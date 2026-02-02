#include "imgui_impl_x11.h"
#include "imgui.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cfloat>
#include <chrono>
#include <cstring> // for strcpy
#include <iostream>

// time in milliseconds and a type of double
typedef std::chrono::duration<double, std::ratio<1, 1000>> SystemTime;
// TODO: look into why changing from float to double worked. I assume because of better floating point precision? 

struct ImGui_ImplX11_Data{
   Window* XWindow;
   Display* XDisplay;
   SystemTime Time;
   // TODO: As mentioned with time before, it might be a good idea to include a member with at least the most recent 
   // time received in an event structure from X to verify that we disregard events that are out of order (not sure if that can happen)
   char BackendPlatformName[15];
   bool inWindow;
};

static SystemTime ImGui_ImplX11_GetCurrentTime(){
   auto now = std::chrono::system_clock::now();
   return std::chrono::duration_cast<SystemTime>(now.time_since_epoch());
}

static ImGui_ImplX11_Data* ImGui_ImplX11_GetBackendData(){
   return ImGui::GetCurrentContext() ? (ImGui_ImplX11_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}

static bool ImGui_ImplX11_Init(Display* display, Window* window){
   // TODO: I remember something in the Xlib about being able to choose events you want to listen to after creation
   // There are certain events we need to get to be functional, so we can either just mandate they pass in a window 
   // with these events and check to make sure that those events are selected, or try and add these events if possible?
   // Also, if its possible for the user to change their event structure later, do we need to check everytime (maybe frame) 
   // about if we can still get these events, and if not add ourselves to the update?

   ImGuiIO& io = ImGui::GetIO();
   IMGUI_CHECKVERSION();
   IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

   // Setup backend capabilities flags
   ImGui_ImplX11_Data* bd = IM_NEW(ImGui_ImplX11_Data)();
   strcpy(bd->BackendPlatformName, "imgui_impl_x11");
   io.BackendPlatformUserData = (void*)bd;
   io.BackendPlatformName = bd->BackendPlatformName;
   /* 
   Set io.BackendFlags with supported optional features:
   ImGuiBackendFlags_HasGamepad: supports gamepad and currently has one connected.
   ImGuiBackendFlags_HasMouseCursors: supports honoring GetMouseCursor() value to change the OS cursor shape.
   ImGuiBackendFlags_HasSetMousePos: supports io.WantSetMousePos requests to reposition the OS mouse position (only used if io.ConfigNavMoveSetMousePos is set).
   ImGuiBackendFlags_PlatformHasViewports supports multiple viewports. (multi-viewports only)
   ImGuiBackendFlags_HasMouseHoveredViewport supports calling io.AddMouseViewportEvent() with the viewport under the mouse. IF POSSIBLE, ignore viewports with the ImGuiViewportFlags_NoInputs flag. If this cannot be done, Dear ImGui needs to use a flawed heuristic to find the viewport under mouse position, as it doesn't know about foreign windows. (multi-viewports only)
   */

   bd->XWindow = window;
   bd->XDisplay = display;
   bd->Time = ImGui_ImplX11_GetCurrentTime();
   bd->inWindow = true; // TODO: probably cannot guarentee pointer is in window on init? does it matter

   return true;
}

static ImVec2 ImGui_ImplX11_GetDisplaySize(Display* display, Window* window){
   // TODO - Not sure if the display size needs to include the border of the window or not? 
   // I would assume not but you never know

   XWindowAttributes winAttr;
   IM_ASSERT(XGetWindowAttributes(display, *window, &winAttr) != 0
               && "Error retrieving window attributes in ImGui_ImplX11_GetDisplaySize. \
               Please ensure Display and Window are valid for lifetime of ImGui_ImplX11");

   return ImVec2(winAttr.width, winAttr.height);
}

static void ImGui_ImplX11_SetKeyModifiers(unsigned int XModMask){
   ImGuiIO& io = ImGui::GetIO();
   io.AddKeyEvent(ImGuiMod_Ctrl, XModMask & ControlMask);
   io.AddKeyEvent(ImGuiMod_Shift, XModMask & ShiftMask);
   // TODO: What mask is alt maske for X?
   //io.AddKeyEvent(ImGuiMod_Alt, XModMask & );
   io.AddKeyEvent(ImGuiMod_Ctrl, XModMask & ControlMask);

   // TODO: Do we need to do something about the below valus? probably super
   /* 
    ImGuiMod_None                   = 0,
    ImGuiMod_Super                  = 1 << 15, // Windows/Super (non-macOS), Ctrl (macOS)
    ImGuiMod_Mask_                  = 0xF000,  // 4-bits
    */

}

static ImGuiKey ImGui_ImplX11_TranslateKeyPressed(XKeyEvent* keyEvent){
   KeySym keySym = XLookupKeysym(keyEvent, 0);// TODO: index is 0, is this right/does it matter?
                                              // not a lot of info online for X server

   // TODO: Should be most of the imgui keys, lots more of keysyms so not sure if we need to handle them
   // also should test to make sure the right ones are pressed, not unlikely that i messed up typing
   switch (keySym){
      case(XK_Tab): return ImGuiKey_Tab;
      case(XK_Left): return ImGuiKey_LeftArrow;
      case(XK_Right): return ImGuiKey_RightArrow;
      case(XK_Up): return ImGuiKey_UpArrow;
      case(XK_Down): return ImGuiKey_DownArrow;
      case(XK_Page_Up): return ImGuiKey_PageUp;
      case(XK_Page_Down): return ImGuiKey_PageDown;
      case(XK_Home): return ImGuiKey_Home;
      case(XK_End): return ImGuiKey_End;
      case(XK_Insert): return ImGuiKey_Insert;
      case(XK_Delete): return ImGuiKey_Delete;
      case(XK_BackSpace): return ImGuiKey_Backspace;
      case(XK_space): return ImGuiKey_Space;
      case(XK_Return): return ImGuiKey_Enter;
      case(XK_Escape): return ImGuiKey_Escape;
      case(XK_Control_L): return ImGuiKey_LeftCtrl;
      case(XK_Shift_L): return ImGuiKey_LeftShift;
      case(XK_Alt_L): return ImGuiKey_LeftAlt;
      case(XK_Super_L): return ImGuiKey_LeftSuper;
      case(XK_Control_R): return ImGuiKey_RightCtrl;
      case(XK_Shift_R): return ImGuiKey_RightShift;
      case(XK_Alt_R): return ImGuiKey_RightAlt;
      case(XK_Super_R): return ImGuiKey_RightSuper;
      case(XK_Menu): return ImGuiKey_Menu;
      case(XK_0): return ImGuiKey_0;
      case(XK_1): return ImGuiKey_1;
      case(XK_2): return ImGuiKey_2;
      case(XK_3): return ImGuiKey_3;
      case(XK_4): return ImGuiKey_4;
      case(XK_5): return ImGuiKey_5;
      case(XK_6): return ImGuiKey_6;
      case(XK_7): return ImGuiKey_7;
      case(XK_8): return ImGuiKey_8;
      case(XK_9): return ImGuiKey_9;
      case(XK_a): 
      case(XK_A): return ImGuiKey_A;
      case(XK_b): 
      case(XK_B): return ImGuiKey_B;
      case(XK_c): 
      case(XK_C): return ImGuiKey_C;
      case(XK_d): 
      case(XK_D): return ImGuiKey_D;
      case(XK_e): 
      case(XK_E): return ImGuiKey_E;
      case(XK_f): 
      case(XK_F): return ImGuiKey_F;
      case(XK_g): 
      case(XK_G): return ImGuiKey_G;
      case(XK_h): 
      case(XK_H): return ImGuiKey_H;
      case(XK_i): 
      case(XK_I): return ImGuiKey_I;
      case(XK_j): 
      case(XK_J): return ImGuiKey_J;
      case(XK_k): 
      case(XK_K): return ImGuiKey_K;
      case(XK_l): 
      case(XK_L): return ImGuiKey_L;
      case(XK_m): 
      case(XK_M): return ImGuiKey_M;
      case(XK_n): 
      case(XK_N): return ImGuiKey_N;
      case(XK_o): 
      case(XK_O): return ImGuiKey_O;
      case(XK_p): 
      case(XK_P): return ImGuiKey_P;
      case(XK_q): 
      case(XK_Q): return ImGuiKey_Q;
      case(XK_r): 
      case(XK_R): return ImGuiKey_R;
      case(XK_s): 
      case(XK_S): return ImGuiKey_S;
      case(XK_t): 
      case(XK_T): return ImGuiKey_T;
      case(XK_u): 
      case(XK_U): return ImGuiKey_U;
      case(XK_v): 
      case(XK_V): return ImGuiKey_V;
      case(XK_w): 
      case(XK_W): return ImGuiKey_W;
      case(XK_x): 
      case(XK_X): return ImGuiKey_X;
      case(XK_y): 
      case(XK_Y): return ImGuiKey_Y;
      case(XK_z): 
      case(XK_Z): return ImGuiKey_Z;
      case(XK_F1): return ImGuiKey_F1;
      case(XK_F2): return ImGuiKey_F2;
      case(XK_F3): return ImGuiKey_F3;
      case(XK_F4): return ImGuiKey_F4;
      case(XK_F5): return ImGuiKey_F5;
      case(XK_F6): return ImGuiKey_F6;
      case(XK_F7): return ImGuiKey_F7;
      case(XK_F8): return ImGuiKey_F8;
      case(XK_F9): return ImGuiKey_F9;
      case(XK_F10): return ImGuiKey_F10;
      case(XK_F11): return ImGuiKey_F11;
      case(XK_F12): return ImGuiKey_F12;
      case(XK_F13): return ImGuiKey_F13;
      case(XK_F14): return ImGuiKey_F14;
      case(XK_F15): return ImGuiKey_F15;
      case(XK_F16): return ImGuiKey_F16;
      case(XK_F17): return ImGuiKey_F17;
      case(XK_F18): return ImGuiKey_F18;
      case(XK_F19): return ImGuiKey_F19;
      case(XK_F20): return ImGuiKey_F20;
      case(XK_F21): return ImGuiKey_F21;
      case(XK_F22): return ImGuiKey_F22;
      case(XK_F23): return ImGuiKey_F23;
      case(XK_F24): return ImGuiKey_F24;
      case(XK_apostrophe): return ImGuiKey_Apostrophe;
      case(XK_comma): return ImGuiKey_Comma;
      case(XK_minus): return ImGuiKey_Minus;
      case(XK_period): return ImGuiKey_Period;
      case(XK_slash): return ImGuiKey_Slash;
      case(XK_semicolon): return ImGuiKey_Semicolon;
      case(XK_equal): return ImGuiKey_Equal;
      case(XK_bracketleft): return ImGuiKey_LeftBracket;
      case(XK_backslash): return ImGuiKey_Backslash;
      case(XK_bracketright): return ImGuiKey_RightBracket;
      case(XK_grave): return ImGuiKey_GraveAccent;
      case(XK_Caps_Lock): return ImGuiKey_CapsLock;
      case(XK_Scroll_Lock): return ImGuiKey_ScrollLock;
      case(XK_Num_Lock): return ImGuiKey_NumLock;
      case(XK_Print): return ImGuiKey_PrintScreen; // TODO: in keysymdef.h there is XK_3270_PrintScreen. 
                                                   // not sure if this is the one we want or not
      case(XK_Pause): return ImGuiKey_Pause;
      case(XK_KP_0): return ImGuiKey_Keypad0;
      case(XK_KP_1): return ImGuiKey_Keypad1;
      case(XK_KP_2): return ImGuiKey_Keypad2;
      case(XK_KP_3): return ImGuiKey_Keypad3;
      case(XK_KP_4): return ImGuiKey_Keypad4;
      case(XK_KP_5): return ImGuiKey_Keypad5;
      case(XK_KP_6): return ImGuiKey_Keypad6;
      case(XK_KP_7): return ImGuiKey_Keypad7;
      case(XK_KP_8): return ImGuiKey_Keypad8;
      case(XK_KP_9): return ImGuiKey_Keypad9;
      case(XK_KP_Decimal): return ImGuiKey_KeypadDecimal;
      case(XK_KP_Divide): return ImGuiKey_KeypadDivide;
      case(XK_KP_Multiply): return ImGuiKey_KeypadMultiply;
      case(XK_KP_Subtract): return ImGuiKey_KeypadSubtract;
      case(XK_KP_Add): return ImGuiKey_KeypadAdd;
      case(XK_KP_Enter): return ImGuiKey_KeypadEnter;
      case(XK_KP_Equal): return ImGuiKey_KeypadEqual;
   }

   return ImGuiKey_None;
}

bool ImGui_ImplX11_InitForOpenGL(Display* display, Window* window){
   return ImGui_ImplX11_Init(display, window);
}


void ImGui_ImplX11_Shutdown(){

}

void ImGui_ImplX11_ProcessEvent(XEvent* event){
   ImGui_ImplX11_Data* bd = ImGui_ImplX11_GetBackendData();
   IM_ASSERT(bd != nullptr && "This backend (or ImGui context) has not been initialized yet");
   ImGuiIO& io = ImGui::GetIO();

   switch(event->type){
      case(EnterNotify):
      case(LeaveNotify):
         bd->inWindow = event->type == EnterNotify;
         // TODO: Do we want to add mouse pos event here?
         break;
      case(MotionNotify): // TODO: All key and button events contain information about the pointer's location. Should we process this?
         bd->inWindow ? io.AddMousePosEvent(event->xmotion.x, event->xmotion.y) : io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
         // TODO: Will likely need to add support for when pointer leaves window (there is an XEvent for this)
         // TODO: Queue a mouse position update. Use -FLT_MAX,-FLT_MAX to signify no mouse (e.g. app not focused and not hovered)
         break;
      case(ButtonPress):
      case(ButtonRelease):
         // TODO: Not sure how to add support for scroll from X but io.AddMouseWheelEvent does that?
         //IM_ASSERT((event->xbutton.button == 1 || event->xbutton.button == 2) 
                        //&& "Button released not equal to button 1 or 2");
         // TODO: Should we call ImGui_ImplX11_SetKeyModifiers here?
         std::cout << "Button passed: " << static_cast<int>(event->xbutton.button - 1) << '\n';
         io.AddMouseButtonEvent(static_cast<int>(event->xbutton.button - 1), event->type == ButtonPress);
         break;
      case(KeyPress): 
      case(KeyRelease): 
         ImGui_ImplX11_SetKeyModifiers(event->xkey.state); // TODO: Is this needed if we get events from X about keys pressed including mods?
                                          // There might be a good comment discussion in imgui.h around line 1656
                                          // Do we get events from X about mods being pressed ?
         ImGuiKey key = ImGui_ImplX11_TranslateKeyPressed(&event->xkey);
         IM_ASSERT(key != ImGuiKey_None && "button pressed corresponds to an unhandled key"); // TODO: if it returns none is this an issue or does it not matter ?
                                                                                              // If its fine that that key isn't found do we still pass it back to io.AddKeyEvent()?
         io.AddKeyEvent(key, event->type == KeyPress);
         break;
   }

   /*
   Extra features to maybe implement: 
   Mouses: 
      Use io.AddMouseSourceEvent() if you are able to distinguish Mouse from TouchScreen from Pen inputs. TouchScreen and Pen inputs requires different logic for some Dear ImGui features.
      Use io.AddMouseViewportEvent() to specify which viewport/OS window is being hovered by the mouse. Read instructions carefully as this is not as simple as it seems! (multi-viewports only)
   Keyboard:
      Use io.AddInputCharacter() to pass input (I believe for like a text box?)
   Gamepad Support
      Use io.AddKeyEvent() and io.AddKeyAnalogEvent() to pass gamepad events, using ImGuiKey_GamepadXXX values.
   Miscellaneous
      Clipboard Support: setup Platform_GetClipboardTextFn(), Platform_SetClipboardTextFn() handlers in ImGuiPlatformIO.
      Open in Shell support: setup Platform_OpenInShellFn() handler in ImGuiPlatformIO.
      IME Support: setup Platform_SetImeDataFn() handler in ImGuiPlatformIO.
      Use io.AddFocusEvent() to notify when application window gets focused/unfocused.
      Multi-viewport Support
      Update monitor list if supported.
      Setup all required handlers in ImGuiPlatformIO to create/destroy/move/resize/title/focus/etc. windows.
      */

}

void ImGui_ImplX11_NewFrame(){
   /* 
   In your ImGui_ImplXXX_NewFrame() function:
   Update mouse cursor shape is supported.
   */

   ImGui_ImplX11_Data* bd = ImGui_ImplX11_GetBackendData();
   IM_ASSERT(bd != nullptr && "This backend (or ImGui context) has not been initialized yet");
   ImGuiIO& io = ImGui::GetIO();

   //Set io.DeltaTime to the time elapsed (in seconds) since last frame.
   SystemTime currentTime = ImGui_ImplX11_GetCurrentTime();
   float diff = currentTime.count() - bd->Time.count();
   IM_ASSERT((diff > 0) && "current time is older or the same as the previous time stored in backend");
   io.DeltaTime = diff;
   bd->Time = currentTime;
   // TODO - If we need to get the X server specific time (and not use the chrono lib) we can possibly use 
   // XChangeProperty() to force a property change event which will contain a timestamp from X Server. 
   // Things to think about: How to call this routine without actually changing anything? Does this take a long time? Can we even do it with our current event loop setup ?

   //Set io.DisplaySize to your window size.
   io.DisplaySize = ImGui_ImplX11_GetDisplaySize(bd->XDisplay, bd->XWindow);
   //Set io.DisplayFrameBufferSize to your window pixel density (macOS/iOS only).
}
