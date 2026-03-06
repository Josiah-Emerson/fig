#pragma once
#include "Fig_Fig/Events.h"
#include "Fig_Fig/Primitives.h"
#include <memory> // std::shared_ptr

class PlatformWindow;

// TODO: probably a good idea to have some internal functionality for when the size is updated to let the parent window know
// For example: update a window and it needs to ask/let its parent know its new pos/size and then the parent checks to make sure it can 
class IWindow {
   public: 
      virtual ~IWindow() = default;
      virtual bool isVisible() = 0;
      virtual bool updatePos(Fig::Vec2i pos) = 0;
      virtual bool updateSize(Fig::Vec2i pos) = 0;
      virtual Fig::Vec2i getPos() { return pos_; } // TODO: change this to not virtual probably and following funcs
      virtual Fig::Vec2i getSize() { return size_; }
      virtual int getWidth() { return size_.x; }
      virtual int getHeight() { return size_.y; }
      std::shared_ptr<PlatformWindow> getRootWindow() { return rootWindow_; }
      std::shared_ptr<IWindow> getParentWindow() { return parentWindow_; }

   protected: 
      Fig::Vec2i pos_; // relative to parent 
      Fig::Vec2i size_;

   private: 
      // TODO: How to ensure invariant here with the parent and root window pointer 
      // being the same as what is held in the tree. Should we just remove this and 
      // define getRoot/ParentWindow funcs differently?
      // TODO: Decide better hierarchy (i.e. should LinuxPlatform be a type of window? Should it own a window?)
      std::shared_ptr<PlatformWindow> rootWindow_; // null if this window is the root 
      std::shared_ptr<IWindow> parentWindow_;
};

// PlatformRootWindow is intended to be an abstraction for OS specific stuff
// Inheriting classes should setup things such as the window, ImGui, OpenGL, event passing etc.
// TODO: do we want an init function to better pass errors? Or are we fine with just throwing in 
// constructor if something goes wrong during initialization

class PlatformRootWindow : public IWindow{
   public: 
      virtual ~PlatformRootWindow() = default;

      // TODO: Should we provide a way to overrule certain things?
      // Example: (at least at one point to test stuff) hitting escape was meant to close the program 
      // but if the cursor was focused in an imgui window then imgui would want that key press and never notify the application
      virtual Fig::EventQueue pollEvents() = 0;  // returns vector of Fig::Events to be handled by application (i.e. isn't wanted by imgui)
      virtual void swapBuffers() = 0;
      virtual void newImGuiFrame() = 0;
      virtual void renderImGuiDrawData() = 0;
};
