#pragma once
#include <X11/Xlib.h>
#include "imgui.h"
#ifndef IMGUI_DISABLE 

// TODO - Do we need this ?
//typedef struct __GLXcontextRec *GLXContext;

IMGUI_IMPL_API bool ImGui_ImplX11_InitForOpenGL(Display* display, Window* window);
IMGUI_IMPL_API void ImGui_ImplX11_NewFrame();
IMGUI_IMPL_API void ImGui_ImplX11_Shutdown();
IMGUI_IMPL_API void ImGui_ImplX11_ProcessEvent(XEvent* event);

#endif
