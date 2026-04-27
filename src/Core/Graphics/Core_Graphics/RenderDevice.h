#pragma once
#include "Shader.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Core_Utils/Types.h"

namespace Core{
   class RenderDevice{
      public: 
         RenderDevice() = default; // TBD: what goes in here
         virtual ~RenderDevice() = default;

         virtual bool registerModel(const Model& model) = 0;
         virtual bool registerEntity(EntityID id, const Model& model, std::shared_ptr<ShaderProgram> shaderProgram) = 0;

         virtual std::shared_ptr<Shader> createShader(const char* filepath, ShaderType type) = 0;
         virtual std::shared_ptr<ShaderProgram> createShaderProgram() = 0; // TBD: Args

         virtual void drawRegisteredEntities() = 0; // TBD: Args
                              
         static std::shared_ptr<RenderDevice> createRenderDevice(); // TBD: Any args

      private: 
   };
   
} // namespace Core
