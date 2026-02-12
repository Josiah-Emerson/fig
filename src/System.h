// First iteration (i.e. a bake settings approach): set settings, calc sim, play sim. Change settings, re-calc and play. 
// Application layer which shouldn't care about what system we are on. Should be fine to couple to things like imgui
//    controls the flow of the app like running the sym and changing variables and play back and all that
// System layer which handles all of the system specific stuff 
#pragma once
#include <GL/glx.h>
#include <X11/Xlib.h>

class System{
   // System-specific stuff we need to handle 
   //    Opening window and binding it to opengl context 
   //       opengl handles drawing for each system so that is not something we need to worry about
   //    setting up imgui according to what system we are on
   //       TBD: Do we need to handle updates of variables here or in the application class 
   //    getting input from system that imgui doesn't want
   //       probably best to translate to application understood stuff (i.e. calling functions of the app based on input) ?
   private: 
      // TODO: best way to handle pointers 
      Display* XDisplay_;
      Window XWindow_;
      // TODO: should we keep glx stuff as members here? Or pass to a different entity?
      GLXWindow glxWindow_;
      GLXContext glxContext_;
      bool running_ { true };

   public: 
      System();
      System(const System&) = delete;
      System(System&&) = delete;
      System operator=(const System&) = delete;
      System operator=(System&&) = delete;
      ~System();

      void run();

   private: 
      void processEvent(const XEvent& event);
};
