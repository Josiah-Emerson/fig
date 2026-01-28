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
   char BackendPlatformName[15];
};

static SystemTime ImGui_ImplX11_GetCurrentTime(){
   auto now = std::chrono::system_clock::now();
   return std::chrono::duration_cast<SystemTime>(now.time_since_epoch());
}

static ImGui_ImplX11_Data* ImGui_ImplX11_GetBackendData(){
   return ImGui::GetCurrentContext() ? (ImGui_ImplX11_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}

static bool ImGui_ImplX11_Init(Display* display, Window* window){
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

bool ImGui_ImplX11_InitForOpenGL(Display* display, Window* window){
   return ImGui_ImplX11_Init(display, window);
}


IMGUI_IMPL_API void ImGui_ImplX11_Shutdown();
IMGUI_IMPL_API void ImGui_ImplX11_ProcessEvent(const XEvent* event){
}

IMGUI_IMPL_API void ImGui_ImplX11_NewFrame(){
   // TODO - If we need to get the X server specific time (and not use the chrono lib) we can possibly use 
   // XChangeProperty() to force a property change event which will contain a timestamp from X Server. 
   // Things to think about: How to call this routine without actually changing anything? Does this take a long time? Can we even do it with our current event loop setup ?

   /* 
   In your ImGui_ImplXXX_NewFrame() function:
   Set io.DisplaySize to your window size.
   Set io.DisplayFrameBufferSize to your window pixel density (macOS/iOS only).
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
}
