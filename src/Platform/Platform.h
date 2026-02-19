#pragma once 
#include "../Fig.h"

// Platform is intended to be an abstraction for OS specific stuff
// Inheriting classes should setup things such as the window, ImGui, OpenGL, event passing etc.
// TODO: do we want an init function to better pass errors? Or are we fine with just throwing in 
// constructor if something goes wrong during initialization


class Platform{
   public: 
      virtual ~Platform() = default;

      // TODO: better to return list of events we need to handle or just go one by one
      // TODO: How to tell caller there are no events (either some type of nullptr or null type or a list of size 0)
      virtual FigEvent PollEvents() = 0;
      virtual void HelloWorld() const = 0;
};
