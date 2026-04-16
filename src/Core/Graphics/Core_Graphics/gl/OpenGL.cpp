#include "Core_Graphics/gl/OpenGL.h"
#include <stdexcept>
// MACROS
// Load function
#define FETCH_GL_FUNC(FTYPE, FNAME) do{ \
   (FNAME) = (FTYPE) window.getProcAddress((char*)#FNAME); \
   if(!(FNAME)) \
      throw std::runtime_error("Could not load OpenGL function "#FNAME); \
}while(0)

namespace Core{
   OpenGL::OpenGL(Core::Window& window){
      FETCH_GL_FUNC(PFNGLATTACHSHADERPROC, glAttachShader);
      FETCH_GL_FUNC(PFNGLCOMPILESHADERPROC, glCompileShader);
      FETCH_GL_FUNC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
      FETCH_GL_FUNC(PFNGLCREATESHADERPROC, glCreateShader);
      FETCH_GL_FUNC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
      FETCH_GL_FUNC(PFNGLDELETESHADERPROC,  glDeleteShader);
      FETCH_GL_FUNC(PFNGLGETATTACHEDSHADERSPROC, glGetAttachedShaders);
      FETCH_GL_FUNC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
      FETCH_GL_FUNC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
      FETCH_GL_FUNC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
      FETCH_GL_FUNC(PFNGLGETSHADERIVPROC, glGetShaderiv);
      FETCH_GL_FUNC(PFNGLGETSTRINGPROC, glGetString);
      FETCH_GL_FUNC(PFNGLLINKPROGRAMPROC, glLinkProgram);
      FETCH_GL_FUNC(PFNGLSHADERSOURCEPROC, glShaderSource);
      FETCH_GL_FUNC(PFNGLUSEPROGRAMPROC, glUseProgram);
      FETCH_GL_FUNC(PFNGLVALIDATEPROGRAMPROC, glValidateProgram);
   }
} // namespace Core
