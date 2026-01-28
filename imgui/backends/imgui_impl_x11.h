#pragma once
#include <X11/Xlib.h>
#include <imgui.h>
#ifndef IMGUI_DISABLE 

typedef struct __GLXcontextRec *GLXContext;

IMGUI_IMPL_API bool ImGui_ImplX11_InitForOpenGL(Display* display, Window* window, GLXContext* x_gl_context);
IMGUI_IMPL_API void ImGui_ImplX11_NewFrame();
IMGUI_IMPL_API void ImGui_ImplX11_Shutdown();
IMGUI_IMPL_API void ImGui_ImplX11_ProcessEvent(const XEvent* event);

#endif
