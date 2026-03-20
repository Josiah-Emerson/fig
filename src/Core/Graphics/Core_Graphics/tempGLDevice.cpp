#pragma once

#include "Core_Window/Window.h"
#include "Application.h"
#include <GL/gl.h>
#include <GL/glx.h>

namespace Core{
   class GL{
      private: 
         // openGL function typedefs
         typedef void (APIENTRYP PFNGLGETINTEGERVPROC) (GLenum pname, GLint* data);

      public: 
         GL()
            : m_window { Application::get().getWindow() }
         {
            loadFuncs();
         }
         ~GL() = default;

         void printVersion(){
            int maj { -1 };
            int min { -1 };
            glGetIntegerv(GL_MAJOR_VERSION, &maj);
            glGetIntegerv(GL_MINOR_VERSION, &min);
            std::cout << "OpenGL Version: " << maj << '.' << min << '\n';
         }

      private: 
         std::shared_ptr<Window> m_window;

      private: 
         void loadFuncs(){
            // NOTE: may change getProcAddress func signature to just a string or something and then do the unsigned char* cast in the LinuxWindow specific func,
            // as this might be very couples to the glx get proc address func
            glGetIntegerv = (PFNGLGETINTEGERVPROC) m_window->getProcAddress((char*)"glGetIntegerv");
            if(!glGetIntegerv)
               exit(1); // TODO: change 
        }

         // OpenGL Funcs
         PFNGLGETINTEGERVPROC glGetIntegerv;

   };
} // namespace Core
