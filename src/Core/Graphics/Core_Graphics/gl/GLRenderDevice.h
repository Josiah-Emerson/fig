#pragma once
#include "Application.h"
#include "Core_Graphics/gl/OpenGL.h"
#include "Core_Graphics/RenderDevice.h"

namespace Core{
   class GLRenderDevice : public RenderDevice {
      public: 
         GLRenderDevice()
            : m_openGL(*Application::get().getWindow())
         { 
            std::cout << "New GL Render Device created\n";
         }
         void registerDrawable() override { 
            const GLubyte* ptr;
            ptr = m_openGL.glGetString(GL_VENDOR);
            std::cout << "The Vendor is: " << ptr << '\n';

            ptr = m_openGL.glGetString(GL_RENDERER);
            std::cout << "The Renderer is: " << ptr << '\n';

            ptr = m_openGL.glGetString(GL_VERSION);
            std::cout << "The GL Version is: " << ptr << '\n';
         }

         void unregisterDrawable() override { }
      private: 
         OpenGL m_openGL;
   };
} // namespace Core
