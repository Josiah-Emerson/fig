#pragma once
#include "Core_Window/Window.h"
#include <GL/gl.h>

// Loads all needed functions for openGL. Is it fine to have them gloablly like this? 

namespace Core{
   // Typedefs 
   typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
   typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
   typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
   typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
   typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
   typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
   typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC) (GLenum shaderType);
   typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
   typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
   typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
   typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
   typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
   typedef void (APIENTRYP PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
   typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
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
   typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
   
   struct OpenGL{
      OpenGL(Core::Window&); // loads functions
      bool operator==(const OpenGL&) const;
                             
      PFNGLATTACHSHADERPROC glAttachShader;
      PFNGLBINDBUFFERPROC glBindBuffer;
      PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
      PFNGLBUFFERDATAPROC glBufferData;
      PFNGLCOMPILESHADERPROC glCompileShader;
      PFNGLCREATEPROGRAMPROC glCreateProgram;
      PFNGLCREATESHADERPROC glCreateShader;
      PFNGLDELETEPROGRAMPROC glDeleteProgram;
      PFNGLDELETESHADERPROC  glDeleteShader;
      PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
      PFNGLGENBUFFERSPROC glGenBuffers;
      PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
      PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
      PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
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
      PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
   };

   // Functions
} // namespace Core

