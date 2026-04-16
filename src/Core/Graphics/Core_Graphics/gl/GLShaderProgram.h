#pragma once
#include "Core_Graphics/ShaderProgram.h"
#include "Core_Graphics/gl/OpenGL.h"
#include <GL/gl.h>
#include <GL/glext.h>

namespace Core{
   // OpenGL input types we have support for
   enum ShaderDataType : int{
   };

   struct ShaderInputVariableInfo{
      std::size_t size; // size of the input variable (NOTE: This is 1 for all non-array types for OpenGL)
      ShaderDataType type; // data type of input variable
      std::string name; // name of input variable
   };

   class GLShaderProgram : public ShaderProgram{
      public:
         GLShaderProgram(OpenGL& openGL);
         ~GLShaderProgram();

         bool initializeProgram() override;

         bool addShader(std::shared_ptr<Shader> shader) override;

         bool link() override;

         // NOTE: See TODO in ShaderProgram.h for this function
         bool useProgram() override;

         std::string getInfoLog() const override;

         std::vector<ShaderInputVariableInfo> getUniformVariables() const override;
         std::vector<ShaderInputVariableInfo> getAttributeVariables() const override;

         bool setUniform(std::string_view name, void* value, ShaderDataType type) override;
         bool setAttribute(std::string_view name, void* value, ShaderDataType type) override;

      private:
         OpenGL& m_openGL;
         GLuint m_ID;
   };
} // namespace Core

