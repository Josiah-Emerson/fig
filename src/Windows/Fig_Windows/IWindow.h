#pragma once
#include "Fig_Fig/Fig.h"
#include <memory> // std::shared_ptr

class PlatformWindow;

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
      // TODO: Decide better hierarchy (i.e. should LinuxPlatform be a type of window? Should it own a window?)
      std::shared_ptr<PlatformWindow> rootWindow_; // null if this window is the root 
      std::shared_ptr<IWindow> parentWindow_;
};

class PlatformRootWindow : public IWindow{
   public: 
      virtual ~PlatformRootWindow() = default;

      virtual Fig::EventQueue pollEvents() = 0;
      virtual void swapBuffers() = 0;
      virtual void newImGuiFrame() = 0;
      virtual void renderImGuiDrawData() = 0;
};
