#pragma once
#include "Core_Graphics/Shader.h"
#include "Core_Graphics/gl/OpenGL.h"
#include <GL/glext.h>

namespace Core{
   enum ShaderType : int {
      VERTEX_SHADER = GL_VERTEX_SHADER,
      FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
      COMPUTE_SHADER = GL_COMPUTE_SHADER
   };
   class GLShader : public Shader{
      public:
         GLShader(const char* filepath, ShaderType type, OpenGL& openGL);
         ~GLShader();

         // TODO: Big 5. Just delete for now
         GLShader(const GLShader&) = delete;
         GLShader(GLShader&&) = delete;
         GLShader& operator=(const GLShader&) = delete;
         GLShader& operator=(GLShader&&) = delete;

         bool compile() override;
         bool isCompiled() const override;
         bool reload() override;
         std::string getLogInfo() const override;
         bool operator==(const Shader&) const override;
         GLuint getShaderID() const { return m_ID; }

      private: 
         std::string getLogInfoFromOpenGL() const;

         std::string m_filepath;
         OpenGL& m_openGL;
         GLuint m_ID { 0 };
   };
} // namespace Core
