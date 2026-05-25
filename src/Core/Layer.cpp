#include "Layer.h"
#include "Application.h"

namespace Core{
   Layer::Layer()
      : m_window { Application::get().getWindow() }
   { }
   void Layer::queueTransition(std::unique_ptr<Layer> layer){
      // TODO: use lambda to queue a transition for end of loop in Application
   }

} // namespace Core
