#pragma once 
#include "../Fig.h"

// Platform is intended to be an abstraction for OS specific stuff
// Inheriting classes should setup things such as the window, ImGui, OpenGL, event passing etc.
// TODO: do we want an init function to better pass errors? Or are we fine with just throwing in 
// constructor if something goes wrong during initialization


class Platform{
   public: 
      virtual ~Platform() = default;

      // TODO: currently returns a vector of events. Would it be better to do c-style array?
      // for this probably not? We only call this once per frame and it should only return a couple
      // and we don't have to worry about memory management
      virtual Fig::EventQueue PollEvents() = 0; // returns vector of Fig::Eventst to be handled by application
      virtual void SwapBuffers() = 0;
      virtual void NewImGuiFrame() = 0;
      virtual void RenderImGuiDrawData() = 0;
      virtual int GetWindowHeight() = 0;
      virtual int GetWindowWidth() = 0;
};
