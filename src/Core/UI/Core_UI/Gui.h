#pragma once
#include <memory>
#include "Core_Utils/Flags.h"
#include "Core_Utils/Linear/Vector.h"

namespace Core{
   class GuiImpl;

   class Gui{
      public: 
         // TODO: Poor Flags decision means prefix needed for flags
         
         // TODO: Add WindowFlags
         enum WindowFlags_{
            WindowFlags_None = 0, 
         };

         // TODO: Add TextFlags
         enum TextFlags_{
            TextFlags_None = 0,
         };

         // TODO: Add WidgetFlags
         enum WidgetFlags_{
            WidgetFlags_None = 0, 
         };

         typedef Flags<WindowFlags_> WindowFlags;
         typedef Flags<TextFlags_> TextFlags;
         typedef Flags<WidgetFlags_> WidgetFlags;

         class Group;
         class Widgets{
            public:
               Group group(std::string_view label, bool beginExpanded = false);

               // 'Layout' options
               void separator(std::size_t count = 1);
               void text(const char* text, bool sameLine = false);

               // Value manipulators
               
               // return true if the value changed
               bool button(const char* label, bool sameLine = false); // TODO: set size? I think ImGui Auto sets to size of label if size is not specified
               bool rgbSelector(const char* label, Linear::fvec3 color, bool sameLine = false);

            protected:
               Widgets() = default;
               Gui* m_gui = nullptr;
         };

         class Group : public Widgets{
            // TODO: Not really sure here what good a default constructor is, but Falcor had it, 
            // so we can play around with it later
            Group() = default;
            Group(Gui* gui, std::string_view label, bool beginExpanded = false);
            /* Falcor also had this constructor, but I am not sure if/why we would ever need it 
             * obv would need to add .gui() func
             * Group(const Widgets& w, rest of above...) : Group(w.gui(), label, beginExpanded) { }
             */

            // Return true if this group is open
            bool open() const;
            operator bool() const { return open(); }

            ~Group();

            // End Group block
            void release();
         };

         class Window : public Widgets{
            public:
            Window(Gui* gui, 
                  const char* name, 
                  bool& open,
                  Linear::ivec2 size, 
                  Linear::ivec2 pos = {0, 0}, 
                  WindowFlags flags = WindowFlags_None);
            ~Window();

            /*
             * End Window 
             */
            void release();

            // TODO: columns within 
            // TODO: Sizing 
         };

         Gui();
         ~Gui(); 

         /*
          * Begins the UI frame. Must be called at the start of each frame 
          */
         void beginFrame();

         /*
          * Renders the UI, and ends the ImGui frame 
          */
         void render();

      private: 
         std::unique_ptr<GuiImpl> m_impl;
   };

} // namespace Core
