#pragma once
#include <cassert>

namespace Core{
   // aything ImGui should inherit from this and add its own functionality as needed
   class ImGuiContainer{
      public: 
         // TODO: How to do resizable ? 
         // TODO: How to do other flags and such
         struct Settings{
            int height        = 700;
            int width         = 400;
            bool leftJustify  = true;
            int x             = 0;
            int y             = 0;
         };

         // value of *p_open should be checked by caller. If false, do not call draw()
         ImGuiContainer(Settings settings, bool* const p_open)
            : m_open { p_open }
            , m_settings { settings }
         { }
         virtual ~ImGuiContainer() = default;

         void draw();
         // TODO: Not sure if this is needed since we can just pass getOpenBool() to the imgui func
         void windowShouldClose(){ 
            assert(m_open && "m_open is nullptr. This should never happen");
            *m_open = false;
         }
         bool shouldBeOpen(){ return *m_open; }

      protected: 
         bool* getOpenBool() { return m_open; }
         virtual void internalDraw() = 0;
      private: 
         bool* m_open;
         Settings m_settings;
   };
} // namespace Core
