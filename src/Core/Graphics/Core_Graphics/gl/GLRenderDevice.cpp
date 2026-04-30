#include "GLRenderDevice.h"
#include "Core_Graphics/Camera.h"
#include "Core_Utils/Linear/MatrixTransform.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include <memory>

namespace Core{
   bool GLRenderDevice::registerModel(Model& model){
      // TODO: identify fail points

      GLuint VAO;
      m_openGL.glGenVertexArrays(1, &VAO);
      m_openGL.glBindVertexArray(VAO);

      GLuint VBOs[2];
      m_openGL.glGenBuffers(2, VBOs);

      // vertex position data
      m_openGL.glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
      m_openGL.glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * model.getVertexData().size()),
                           model.getVertexData().data(), GL_STATIC_DRAW);
      m_openGL.glEnableVertexAttribArray(0);
      m_openGL.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

      // color data
      m_openGL.glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
      m_openGL.glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * model.getColorData().size()),
                           model.getColorData().data(), GL_STATIC_DRAW);
      m_openGL.glEnableVertexAttribArray(1);
      m_openGL.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

      m_openGL.glBindVertexArray(0);
      m_modelVAOList.emplace_back(model, VAO);

      return true;
   }

   bool GLRenderDevice::registerEntity(EntityID id, Model& model, std::shared_ptr<ShaderProgram> shaderProgram){
      return false;
   }

   std::shared_ptr<Shader> GLRenderDevice::createShader(const char* filepath, ShaderType type) {
      return std::make_shared<GLShader>(filepath, type, m_openGL);
   }

   std::shared_ptr<ShaderProgram> GLRenderDevice::createShaderProgram() {
      return std::make_shared<GLShaderProgram>(m_openGL);
   }

   bool GLRenderDevice::isValidGraphicsComperand(const GraphicsComperand& cmp) const{
      if(!cmp.prog->isValid())
         return false;

      auto search = std::find_if(m_modelVAOList.begin(), m_modelVAOList.end(), 
            [&cmp](const std::pair<const Model&, GLuint>& v){
               return v.first == *cmp.model;
            });

      return !(search == m_modelVAOList.end());
   }

   void GLRenderDevice::drawRegisteredEntities() {
      // Get an iterator for ShaderProgram and Model
      // Use Program
      // For each ShaderProgram and model combo set the uniform (Assume it is MVP)

      // TODO: how do we pass the necessary values here? 
      // Example: for the MVP matrix we could pass the camera through the function here, 
      // (cuz we def don't want it to be owned by RenderDevice), but if different programs 
      // require different things, we probably need to inject something 
      // For testing just create a camera here I guess

      Camera cam {{10, 10, 10}, 45.f, 1080, 1920, {0, 0, 0}};
      auto V = cam.viewMatrix();
      auto P = cam.projectionMatrix();

      const auto& iterators = m_positionPool.separatorList();
      std::shared_ptr<ShaderProgram> currBoundProgram { nullptr };
      std::shared_ptr<Model> currBoundModel { nullptr };

      // TODO: depending on final form, look at hoisting out commonly looked at vars 
      // into a local var for better cache
      for(const auto& it : iterators){
         const GraphicsComperand& gComp = it.first;
         const auto& separator = it.second;
         // If ShaderProgram in iterator changes, rebind 

         if(gComp.prog != currBoundProgram){
            assert(gComp.prog->useProgram() && "Error using program");
            currBoundProgram = gComp.prog;
         }

         if(gComp.model != currBoundModel){
            auto search = std::find_if(m_modelVAOList.begin(), m_modelVAOList.end(), 
                  [&gComp](const std::pair<const Model&, GLuint>& v){
                     return v.first == *gComp.model;
                  });

            assert(search != m_modelVAOList.end() && "model in pool not found in RenderDevice");
            m_openGL.glBindVertexArray(search->second);

            currBoundModel = gComp.model;
         }

         //                     [first, second]
         // <= cuz separator is [lower, upper ] inclusive
         for(std::size_t i { separator.first }; i <= separator.second; ++i){
            // build MVP matrix here
            // MVP = P * V * M
            auto val = m_positionPool[i];

            // Example: 1 ShaderProgram has MVP uniform and one has MVP and COLOR uniform
            // MVP is mat4 in both and COLOR is vec3. How do we go about so that it knows 
            // which one, and how to use it 
            // an entity using program 1 would have an entry in position pool which could be used to calculate
            // MVP, while an entity using program 2 would have that AND an entry in colorPool 
            // with its color value
            // in this case every shaderProgram would have an associated set of pools with the 
            // data it needs and how to transform that data? 

            auto M = Linear::modelMatrix(val, Linear::Vector{1, 1, 1});
            auto MVP = P * V * M;
            auto MVP_T = MVP.transpose();
            assert(currBoundProgram->setUniform("MVP", &MVP_T, F_MAT4) && "Could not setUniform");

            m_openGL.glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // NOTE: look how hardcoded this is, must be a better way
         }
         
      }
   }

} // namespace Core
