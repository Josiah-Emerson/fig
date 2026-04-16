#pragma once
#include "Core_Window/Window.h"
#include <GL/gl.h>

// Loads all needed functions for openGL. Is it fine to have them gloablly like this? 

namespace Core{
   // Typedefs 
   typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
   typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
   typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
   typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC) (GLenum shaderType);
   typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
   typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
   typedef void (APIENTRYP PFNGLGETATTACHEDSHADERSPROC) (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
   typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
   typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
   typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
   typedef void (APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
   typedef const GLubyte* (APIENTRYP PFNGLGETSTRINGPROC) (GLenum name);
   typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
   typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
   typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
   typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC) (GLuint program);
   
   struct OpenGL{
      OpenGL(Core::Window&); // loads functions
                             
      PFNGLATTACHSHADERPROC glAttachShader;
      PFNGLCOMPILESHADERPROC glCompileShader;
      PFNGLCREATEPROGRAMPROC glCreateProgram;
      PFNGLCREATESHADERPROC glCreateShader;
      PFNGLDELETEPROGRAMPROC glDeleteProgram;
      PFNGLDELETESHADERPROC  glDeleteShader;
      PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
      PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
      PFNGLGETPROGRAMIVPROC glGetProgramiv;
      PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
      PFNGLGETSHADERIVPROC glGetShaderiv;
      PFNGLGETSTRINGPROC glGetString;
      PFNGLLINKPROGRAMPROC glLinkProgram;
      PFNGLSHADERSOURCEPROC glShaderSource;
      PFNGLUSEPROGRAMPROC glUseProgram;
      PFNGLVALIDATEPROGRAMPROC glValidateProgram;
   };

   // Functions
} // namespace Core

