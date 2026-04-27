#include "GLRenderDevice.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include <memory>

namespace Core{
   bool GLRenderDevice::registerModel(const Model& model){
      GLuint VAO;
      m_openGL.glGenVertexArrays(1, &VAO);
      m_openGL.glBindVertexArray(VAO);

      GLuint VBOs[2];
      m_openGL.glGenBuffers(2, VBOs);

      // vertex position data
      m_openGL.glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
      m_openGL.glBufferData(GL_ARRAY_BUFFER, sizeof(model.getVertexData().data()),
                           model.getVertexData().data(), GL_STATIC_DRAW);
      m_openGL.glEnableVertexAttribArray(0);
      m_openGL.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

      // color data
      m_openGL.glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
      m_openGL.glBufferData(GL_ARRAY_BUFFER, sizeof(model.getColorData().data()),
                           model.getVertexData().data(), GL_STATIC_DRAW);
      m_openGL.glEnableVertexAttribArray(0);
      m_openGL.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

      m_openGL.glBindVertexArray(0);
      m_modelVAOList.emplace_back(model, VAO);

      return true;
   }

   bool GLRenderDevice::registerEntity(EntityID id, const Model& model, std::shared_ptr<ShaderProgram> shaderProgram){
      return false;
   }

   std::shared_ptr<Shader> GLRenderDevice::createShader(const char* filepath, ShaderType type) {
      return std::make_shared<GLShader>(filepath, type, m_openGL);
   }

   std::shared_ptr<ShaderProgram> GLRenderDevice::createShaderProgram() {
      return std::make_shared<GLShaderProgram>(m_openGL);
   }

} // namespace Core
