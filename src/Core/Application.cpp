#include "Application.h"

namespace Core{

   static Application* s_application = nullptr; // Want one single instance of application

   // NOTE: appSpec has a default initialization if nothing is passed in
   Application::Application(const ApplicationSpec& appSpec)
      : m_specification { appSpec }
   {
      s_application = this;
      m_specification.windowSpec.eventCallback = [this](Core::Events::Event& event) { raiseEvent(event); };
      m_window = Core::Window::getWindow(appSpec.windowSpec);
   }

   Application::~Application(){
      // TODO: anything else?
      s_application = nullptr;
   }

   void Application::run(){
      m_running = true;

      while(m_running){
         m_window->pollEvents();

         // TODO: if we should have a reason to close from the window 
         // if(m_window->shouldClose()){
         // stop();
         // break;
         // }

         // NOTE: update and render are separated for a couple reasons:
         // 1) if rendering is done on separate thread and we need to make sure we render after layer is updated
         // 2) if things from one layer will depend on updates from other layers before rendering
         // TODO: if none of the first 2 are true, could profile to see how much quicker it is doing updates and render at same time
         // TODO: if 2 is true, and it is sufficiently quicker to update and render at same time,
         // could implement a way where if a layer does depend on a not yet updated layer for rendering, that layer's rendering only is delayed? but idk
         for(const std::unique_ptr<Layer>& layer: m_layerStack){
            layer->onUpdate();
         }

         for(const std::unique_ptr<Layer>& layer: m_layerStack){
            layer->onRender();
         }

         m_window->update();
      }
   }

   void Application::stop(){
      m_running = false;
   }

   void Application::raiseEvent(Core::Events::Event& event){
      // this is where we distribute events and propogate to different layers 
      for(auto& layer : m_layerStack){
         if(layer->onEvent(event)) // true if layer handles it
            return;
      }
   }

} // namespace Core
