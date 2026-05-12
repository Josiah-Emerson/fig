#include "GLRenderDevice.h"
#include "Core_Graphics/GraphicsComponents.h"
#include "Core_Graphics/RenderDevice.h"
#include "Core_Graphics/ShaderProgram.h"
#include "GLShader.h"
#include "GLShaderProgram.h"
#include <memory>
#include <tuple>

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
      std::vector<GraphicsComperand> gComps = m_registry.getAllComperands();
      std::shared_ptr<ShaderProgram> currBoundProgram { nullptr };
      std::shared_ptr<Model> currBoundModel { nullptr };
      for(const GraphicsComperand& u : gComps){
         if(u.prog != currBoundProgram){
            assert(u.prog->useProgram() && "Error using program");
            currBoundProgram = u.prog;
         }

         if(u.model != currBoundModel){
            auto search = std::find_if(m_modelVAOList.begin(), m_modelVAOList.end(), 
                  [&u](const std::pair<const Model&, GLuint>& v){
                     return v.first == *(u.model);
                  });
            assert(search != m_modelVAOList.end() && "Model in pool not found in RenderDevice list");
            m_openGL.glBindVertexArray(search->second);
            currBoundModel = u.model;
         }

         // TODO: This is temp for when we just have a position component
         // TODO: Make it more automated
         // Create a struct for unpacking the ComponentList
         // Track per component: ComponentPool, start index, bool for if it exists
         // Track: A pointer to first element here and then go for the offset 
         using ArgHelperType = typename CreateComponentFunction<GraphicsComponentList>::ArgType;
         ArgHelperType components {};
         
         // Gross for now but oh well
         // Loops through all component types and sets the pointer to either the first valid 
         // value or a nullptr if that pool does not contain the component
         // range becomes the number of entities for this comperand to draw
         std::size_t range { 0 };
         for(std::size_t i { 0 }; i < GraphicsComponentIndex::NUM_COMPONENTS; ++i){
            std::size_t tempRange { 0 };
            switch(i){
               case(GraphicsComponentIndex::POSITION):
               {
                  tempRange = getPtrAndRangeFromComponentTypeAndComperand(u, 
                                          std::get<POSITION>(components));
                  break;
               }
               case(GraphicsComponentIndex::DIRECTION):
               {
                  tempRange = getPtrAndRangeFromComponentTypeAndComperand(u, 
                                          std::get<DIRECTION>(components));
                  break;
               }
               case(GraphicsComponentIndex::SCALE):
               {
                  tempRange = getPtrAndRangeFromComponentTypeAndComperand(u, 
                                          std::get<SCALE>(components));
                  break;
               }
               case(GraphicsComponentIndex::COLOR):
               {
                  tempRange = getPtrAndRangeFromComponentTypeAndComperand(u, 
                                          std::get<COLOR>(components));
                  break;
               }
               case(NUM_COMPONENTS):
                  assert(false && "HIT NUM_COMPONENTS IN SWITCH THAT SHOULD NEVER HAPPEN");
               default: 
                  assert(false && "Hit default in switch on GraphicsComponentIndex. A change must have happened to this enum which was not handled");
            } // end switch 

            if(tempRange != 0 ){// contains components
               if(range == 0 ){ // first encounter of a component pool containing entries
                  range = tempRange;
               }
               // assert that the range is the same
               assert(range == tempRange && "Component pool has a different number of entries than the rest");
            }
         } // end of component 'builder' for loop

         // loop through and draw all entities with this comperand
         for(std::size_t i { 0 }; i < range; ++i){
            const auto& uniformVars = currBoundProgram->getUniformVariables();
            for(const auto& uVar : uniformVars){
               void* data = getDataPtr(uVar.type);
               uVar.callback(data, components, i);
               currBoundProgram->setUniform(uVar.name, data);
               deleteDataPtr(data, uVar.type);
            }

            // TODO: Make this depend on the model
            m_openGL.glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
         }
      }

      // Get an iterator for ShaderProgram and Model
      // Use Program
      // For each ShaderProgram and model combo set the uniform (Assume it is MVP)

      // TODO: how do we pass the necessary values here? 
      // Example: for the MVP matrix we could pass the camera through the function here, 
      // (cuz we def don't want it to be owned by RenderDevice), but if different programs 
      // require different things, we probably need to inject something 
      // For testing just create a camera here I guess


      /*
      auto V = camera.viewMatrix();
      auto P = camera.projectionMatrix();

      auto positionPool = m_registry.getPool<PositionComponent>();
      const auto& iterators = positionPool.separatorList();
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
            auto val = positionPool[i];

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
            // auto MVP_T = MVP.transpose();
            assert(currBoundProgram->setUniform("MVP", &MVP, F_MAT4) && "Could not setUniform");

            m_openGL.glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // NOTE: look how hardcoded this is, must be a better way
         }
         
      }
   */
   }

   void* GLRenderDevice::getDataPtr(ShaderDataType type){
      void* data = nullptr;
      switch(type){
         case(INT):
            {
               data = new int;
               break;
            }
         case(FLOAT): 
            {
               data = new float;
               break;
            }
         case(DOUBLE): 
            {
               data = new double;
               break;
            }
         case(F_VEC3): 
            {
               data = new float[3];
               break;
            }
         case(F_MAT4): 
            {
               data = new float[16];
               break;
            }
         case(UNKNOWN):
         default: 
            assert(false && "Unknown ShaderDataType encounted in getDataPtr func");
      }

      return data;
   }

   void GLRenderDevice::deleteDataPtr(void* ptr, ShaderDataType type){
      assert(ptr && "nullptr passed to GLRenderDevice::deleteDataPtr");
      switch(type){
         case(INT):
            {
               int* data = static_cast<int*>(ptr);
               delete data;
               break;
            }
         case(FLOAT): 
            {
               float* data = static_cast<float*>(ptr);
               delete data;
               break;
            }
         case(DOUBLE): 
            {
               double* data = static_cast<double*>(ptr);
               delete data;
               break;
            }
         case(F_VEC3): 
            {
               float* data = static_cast<float*>(ptr);
               delete[] data;
               break;
            }
         case(F_MAT4): 
            {
               float* data = static_cast<float*>(ptr);
               delete[] data;
               break;
            }
         case(UNKNOWN):
         default: 
            assert(false && "Unknown ShaderDataType encounted in deleteDataPtr func");
      }
   }


} // namespace Core
