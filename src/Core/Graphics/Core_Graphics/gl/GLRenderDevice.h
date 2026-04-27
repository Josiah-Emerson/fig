#pragma once
#include "Application.h"
#include "Core_Graphics/gl/OpenGL.h"
#include "Core_Graphics/RenderDevice.h"
#include <utility>

namespace Core{
   class GLRenderDevice : public RenderDevice {
      public: 
         GLRenderDevice()
            : m_openGL(*Application::get().getWindow())
            , m_modelVAOList {}
         { 
            std::cout << "New GL Render Device created\n";
         }

         bool registerModel(const Model& model) override;
         bool registerEntity(EntityID id, const Model& model, std::shared_ptr<ShaderProgram> shaderProgram) override;

         std::shared_ptr<Shader> createShader(const char* filepath, ShaderType type) override;
         std::shared_ptr<ShaderProgram> createShaderProgram() override;
         void drawRegisteredEntities() override { }

      private: 
         OpenGL m_openGL;
         std::vector<std::pair<const Model&, GLuint>> m_modelVAOList; // List of Model VAO object pairs
   };
} // namespace Core
