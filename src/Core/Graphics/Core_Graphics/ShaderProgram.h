#pragma once
#include "Shader.h"
#include "Core_Utils/Flags.h"
#include <memory>
#include <string>
#include <vector>

namespace Core{
   // To be defined by the graphics specific implementation
   enum ShaderDataType : int;
   struct ShaderInputVariableInfo;


   class ShaderProgram{
      public:
         // Shader Program validity is tracked via stages. Each stage can have any number of fail reasons within it which have kept it from advanving to the next state
         enum ShaderProgramStage{
            NOT_CREATED, 
            CREATED,
            LINKED
         };

         // NOTE: The valid in does not mean that the fail bit cannot be used in other stages, just that 
         // those fail bits only come from an 'actual error' in those states as opposed to breaking the flow
         // Example: in NOT_CREATED stage, a fail bit of ATTACHMENT_NOT_ATTEMPTED means that a shader was attempted 
         // to be added to a program which was not yet created yet. Wheras ATTACHMENT_NOT_ATTEMPTED in CREATED or LINKED, 
         // indicates something was wrong with the shader itself which casued the program to decide not to even attempt to attach it
         enum StageFailFlags_{
            NONE                       = 0,      // Valid in: All stages
            CREATION_FAILURE           = 1 << 0, // Valid in: NOT_CREATED
            ATTACHMENT_NOT_ATTEMPTED   = 1 << 1, // Valid in: CREATED, LINKED (?)
            ATTACHMENT_FAILED          = 1 << 2, // Valid in: CREATED, LINKED (?)
            LINK_FAILED                = 1 << 3, // Valid in: CREATED, LINKED (?)
            STALE                      = 1 << 4, // Valid in: LINKED
            INVALID_FOR_USE            = 1 << 5, // Valid in: LINKED
         };
         typedef Flags<StageFailFlags_> StageFailFlags;


         struct ProgramState{
            ShaderProgramStage stage;
            StageFailFlags status;
         };

         ShaderProgram() : m_state { NOT_CREATED, NONE } { }
         virtual ~ShaderProgram() = default;

         virtual bool initializeProgram() = 0;

         // returns true if the shader already is attached
         virtual bool addShader(std::shared_ptr<Shader> shader) = 0;

         // get list of shaders
         const std::vector<std::shared_ptr<Shader>>& getAttachedShadersList() const { return m_shaders; }
         std::vector<std::shared_ptr<Shader>> getAttachedShadersList() { return m_shaders; }

         // links all added shaders together
         virtual bool link() = 0;

         // TODO: here or let render device do that? or return some type of ID?
         virtual bool useProgram() = 0;

         // This should only be used to retrieve an internal graphics implementation log
         // Returns an empty string if there is no log information
         virtual std::string getInfoLog() const = 0;

         // TODO: Do we need these?
         // Returns a list of ShaderInputVariableInfo structs which are graphics implementation specific and represent necessary info for Uniform variables
         virtual std::vector<ShaderInputVariableInfo> getUniformVariables() const = 0;
         // Returns a list of ShaderInputVariableInfo structs which are graphics implementation specific and represent necessary info for per-vertex variables
         virtual std::vector<ShaderInputVariableInfo> getAttributeVariables() const = 0;

         // TODO: possibly change attribute
         virtual bool setUniform(std::string_view name, void* value, ShaderDataType type) = 0;
         virtual bool setAttribute(std::string_view name, void* value, ShaderDataType type) = 0;

         bool isFailBitSet(const StageFailFlags bits, StageFailFlags_ flag) const;

      protected:
         void setState(ShaderProgramStage newStage, StageFailFlags fail = NONE);

         ProgramState m_state;
         std::vector<std::shared_ptr<Shader>> m_shaders;
   };

} // namespace Core

// NOTE: SEE glGetProgramiv function!!
