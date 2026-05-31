#include "Gui.h"
#include "imgui.h"
#include "../Application.h"

/*
 * OS and Graphics ifdefs 
 */
#ifdef LINUX 
#include <backends/imgui_impl_x11.h>
#include "Core_Window/LinuxWindow.h"
#endif 

#ifdef OPENGL
#include <backends/imgui_impl_opengl3.h>
#endif

namespace Core{
   class GuiImpl{
      public: 
         GuiImpl(); // TODO: Why public if Gui is whats gonna create it anyway? 
                    // Is it because of unique_ptr/make_unique?

      private: 
         friend class Gui;

         void addSeparator();
         void addText(const char* text, bool sameline);

         bool addButton(const char* label, bool sameLine);
         bool addRGBSelector(const char* label, Linear::fvec3 color, bool sameLine);

         /*
          * MEMBER VARS
          */
         ImGuiContext* m_imGuiContext;
   };

   GuiImpl::GuiImpl(){
      IMGUI_CHECKVERSION();
      m_imGuiContext = ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
      ImGui::StyleColorsDark();

      // Window system init
#ifdef LINUX
      // TODO: What if we mix X11 with not OpenGL
      LinuxWindow::LinuxImplementationDetails* winDetails = 
         (LinuxWindow::LinuxImplementationDetails*)
         Application::get().getWindow()->getImplementationSpecificDetails();

      ImGui_ImplX11_InitForOpenGL(winDetails->m_XDisplay, 
                                 &winDetails->m_XWindow);
#endif

      // Graphics init
#ifdef OPENGL
      ImGui_ImplOpenGL3_Init();
#endif 
   }

   void GuiImpl::addSeparator(){
      ImGui::Separator();
   }

   void GuiImpl::addText(const char* text, bool sameLine){
      if(sameLine)
         ImGui::SameLine();

      ImGui::TextUnformatted(text);
   }

   bool GuiImpl::addButton(const char* label, bool sameLine){
      if(sameLine)
         ImGui::SameLine();

      return ImGui::Button(label);
   }

   bool GuiImpl::addRGBSelector(const char* label, Linear::fvec3 color, bool sameLine){
      if(sameLine)
         ImGui::SameLine();

      // TODO: ColorEdit vs ColorPicker
      return ImGui::ColorEdit3(label, &color[0]);
   }

   Gui::Group Gui::Widgets::group(std::string_view label, bool beginExpanded){
      assert(false && "Not implemented yet");
   }

   void Gui::Widgets::separator(std::size_t count){
      if(!m_gui)
         return;

      for(std::size_t i { 0 }; i < count; ++i)
         m_gui->m_impl->addSeparator();
   }

   void Gui::Widgets::text(const char* text, bool sameLine){
      if(!m_gui)
         return;
      m_gui->m_impl->addText(text, sameLine);
   }

   bool Gui::Widgets::button(const char* label, bool sameLine){
      return m_gui && m_gui->m_impl->addButton(label, sameLine);
   }

   bool Gui::Widgets::rgbSelector(const char* label, Linear::fvec3 color, bool sameLine){
      return m_gui && m_gui->m_impl->addRGBSelector(label, color, sameLine);
   }

   Gui::Group::Group(Gui* gui, std::string_view label, bool beginExpanded){
      assert(false && "Not implemented yet");
   }

   bool Gui::Group::open() const{
      assert(false && "Not implemented yet");
      return false;
   }

   Gui::Group::~Group(){
      assert(false && "Not implemented yet");
   }

   void Gui::Group::release(){
      assert(false && "Not implemented yet");
   }

   Gui::Gui(){
      m_impl = std::make_unique<GuiImpl>();
   }

   // TODO
   Gui::~Gui() = default;
};
