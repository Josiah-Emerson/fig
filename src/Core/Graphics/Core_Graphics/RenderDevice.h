#pragma once
#include "Core_Utils/Linear/Vector.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Core_Utils/Types.h"
#include "Core_ECS/SortedComponentPool.h"

// TODO: Right now we jsut have PositionPool, but how can we make it so that it is easy to add 
// components needed to render? Just settle on an agreed upon set of components? probably best for our case

// TODO: I think there is a TODO somewhere in here or .cpp file, but not sure if we should be 
// keeping the models as shared_ptr or by value? the Models can be very large so perhaps by ptr?
// really not sure, but we do both here which def is not the right approach

namespace Core{
   // GraphicsComperand and GraphicsCompare represent a way to sort a SortedRegistry and SortedComponentPool 
   // using relevant things like associated ShaderPrograms and Models to lessen switching in graphics layer (most likely contexts inside GPU)
   // For now it is sorted based on the ShaderProgram first and Model second, but can be changed later
   // This is what works with OpenGL (i think) best, and possibly can be changed if it is different across platforms

   struct GraphicsComperand{
      std::shared_ptr<ShaderProgram> prog;
      std::shared_ptr<Model> model;

      bool operator==(const GraphicsComperand& other) const;
   };

   struct GraphicsCompare{
      bool operator()(const GraphicsComperand& lhs, const GraphicsComperand& rhs) const;
   };

   using PositionComponent  = Linear::fvec3;

   using PositionPool = SortedComponentPool<PositionComponent, GraphicsCompare, GraphicsComperand>;

   class RenderDevice{
      public: 
         RenderDevice() = delete; // TBD: what goes in here
         RenderDevice(const PositionPool& positionPool);
         virtual ~RenderDevice() = default;

         virtual bool registerModel(Model& model) = 0;

         // TODO: I think with new design we remove this and just have users register in their controlling registry
         virtual bool registerEntity(EntityID id, Model& model, std::shared_ptr<ShaderProgram> shaderProgram) = 0;

         virtual std::shared_ptr<Shader> createShader(const char* filepath, ShaderType type) = 0;
         virtual std::shared_ptr<ShaderProgram> createShaderProgram() = 0; // TBD: Args

         // checks if GraphicsComperand is valid (generally if its contained ShaderProgram is valid and its model is registered here)
         virtual bool isValidGraphicsComperand(const GraphicsComperand& cmp) const = 0;
                                                                           
         virtual void drawRegisteredEntities() = 0; // TBD: Args
                              
         static std::shared_ptr<RenderDevice> createRenderDevice(const PositionPool& positionPool); // TBD: Any args

      protected: 
         const PositionPool& m_positionPool;
   };
   
} // namespace Core
