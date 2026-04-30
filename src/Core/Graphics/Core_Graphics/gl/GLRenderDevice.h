#pragma once
#include "Application.h"
#include "Core_Graphics/gl/OpenGL.h"
#include "Core_Graphics/RenderDevice.h"
#include <utility>

namespace Core{
   class GLRenderDevice : public RenderDevice {
      public: 
         GLRenderDevice(const PositionPool& positionPool)
            : m_openGL(*Application::get().getWindow())
            , m_modelVAOList {}
            , RenderDevice(positionPool)
         { 
            std::cout << "New GL Render Device created\n";
         }

         bool registerModel(Model& model) override;
         bool registerEntity(EntityID id, Model& model, std::shared_ptr<ShaderProgram> shaderProgram) override;

         std::shared_ptr<Shader> createShader(const char* filepath, ShaderType type) override;
         std::shared_ptr<ShaderProgram> createShaderProgram() override;

         bool isValidGraphicsComperand(const GraphicsComperand& cmp) const override;
          
         void drawRegisteredEntities() override;

      private: 
         OpenGL m_openGL;
         // Model by value not reference for now. Could switch to std::shared_ptr if we need to
         std::vector<std::pair<const Model, GLuint>> m_modelVAOList; // List of Model VAO object pairs
   };
} // namespace Core
