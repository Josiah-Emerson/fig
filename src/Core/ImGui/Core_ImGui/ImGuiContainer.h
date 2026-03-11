#pragma once
#include <cassert>

namespace Core{
   // aything ImGui should inherit from this and add its own functionality as needed
   class ImGuiContainer{
      public: 
         // value of *p_open should be checked by caller. If false, do not call draw()
         ImGuiContainer(bool* const p_open)
            : m_open { p_open }
         { }
         virtual ~ImGuiContainer() = default;

         virtual void draw() { }
         void windowShouldClose(){ 
            assert(m_open && "m_open is nullptr. This should never happen");
            *m_open = false;
         }
         bool shouldBeOpen(){ return *m_open; }

      protected: 
         bool* getOpenBool() { return m_open; }
      private: 
         bool* m_open;
   };
} // namespace Core
