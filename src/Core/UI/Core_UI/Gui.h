#pragma once
#include <memory>

namespace Core{
   class GuiImpl;

   class Gui{
      public: 

      private: 
         std::unique_ptr<GuiImpl> m_impl;
   };
} // namespace Core
