#include "imgui_impl_x11.h"
#include "imgui.h"
#include <X11/X.h>
#include <chrono>
#include <cstring> // for strcpy

// time in seconds and a type of float
typedef std::chrono::duration<float, std::ratio<1>> SystemTime;

struct ImGui_ImplX11_Data{
   Window* Window;
   Display* Display;
   SystemTime Time;
   // TODO: As mentioned with time before, it might be a good idea to include a member with at least the most recent 
   // time received in an event structure from X to verify that we disregard events that are out of order (not sure if that can happen)
   char BackendPlatformName[15];

   // Mouse stuff
   int x;
   int y;
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

   bd->Window = window;
   bd->Display = display;
   bd->Time = ImGui_ImplX11_GetCurrentTime();

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

bool ImGui_ImplX11_InitForOpenGL(Display* display, Window* window){
   return ImGui_ImplX11_Init(display, window);
}


IMGUI_IMPL_API void ImGui_ImplX11_Shutdown(){

}

IMGUI_IMPL_API void ImGui_ImplX11_ProcessEvent(const XEvent* event){

   /*
   In your ImGui_ImplXXX_NewFrame() function or event handlers:
   Keyboard Support
      Use io.AddKeyEvent() to pass key events.
      Use io.AddInputCharacter() to pass text/character events.
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
   ImGui_ImplX11_Data* bd = ImGui_ImplX11_GetBackendData();
   IM_ASSERT(bd != nullptr && "This backend (or ImGui context) has not been initialized yet");
   ImGuiIO& io = ImGui::GetIO();

   switch(event->type){
      case(MotionNotify): // TODO: All key and button events contain information about the pointer's location. Should we process this?
         io.AddMousePosEvent(event->xmotion.x, event->xmotion.y);
         // TODO: Will likely need to add support for when pointer leaves window (there is an XEvent for this)
         // TODO: Queue a mouse position update. Use -FLT_MAX,-FLT_MAX to signify no mouse (e.g. app not focused and not hovered)
         break;
      case(ButtonPress):
         // TODO: For now it'll just assert out, but X gives options for up to 5 (?) buttons to be pressed 
         // not a clue what those map to, but hopefully just left and right to 1 and 2
         // IMGUI has a button count which is set to 5 (but 0 indexed) at default
         IM_ASSERT((event->xbutton.button == 1 || event->xbutton.button == 2) 
                        && "Button pressed not equal to button 1 or 2");
         io.AddMouseButtonEvent(static_cast<int>(event->xbutton.button - 1), true);
         break;
      case(ButtonRelease):
         // TODO: Not sure how to add support for scroll from X but io.AddMouseWheelEvent does that?
         IM_ASSERT((event->xbutton.button == 1 || event->xbutton.button == 2) 
                        && "Button released not equal to button 1 or 2");
         io.AddMouseButtonEvent(static_cast<int>(event->xbutton.button - 1), false);
         break;
      case(KeyPress): 
         break;
      case(KeyRelease): 
         break;
   }

   // Extra features to maybe implement: 
   // Mouses: 
      // Use io.AddMouseSourceEvent() if you are able to distinguish Mouse from TouchScreen from Pen inputs. TouchScreen and Pen inputs requires different logic for some Dear ImGui features.
      // Use io.AddMouseViewportEvent() to specify which viewport/OS window is being hovered by the mouse. Read instructions carefully as this is not as simple as it seems! (multi-viewports only)
}

IMGUI_IMPL_API void ImGui_ImplX11_NewFrame(){
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
   IM_ASSERT(diff > 0 && "current time is older than previous time stored in backend");
   io.DeltaTime = diff;
   bd->Time = currentTime;
   // TODO - If we need to get the X server specific time (and not use the chrono lib) we can possibly use 
   // XChangeProperty() to force a property change event which will contain a timestamp from X Server. 
   // Things to think about: How to call this routine without actually changing anything? Does this take a long time? Can we even do it with our current event loop setup ?

   //Set io.DisplaySize to your window size.
   io.DisplaySize = ImGui_ImplX11_GetDisplaySize(bd->Display, bd->Window);
   //Set io.DisplayFrameBufferSize to your window pixel density (macOS/iOS only).
}
