#include "ShaderProgram.h"

namespace Core{
   bool ShaderProgram::isFailBitSet(const StageFailFlags bits, StageFailFlags_ flag) const{
      return bool(bits & flag);
   }

   // NOTE: fail is a default arg
   void ShaderProgram::setState(ShaderProgramStage newStage, StageFailFlags fail) { 
      m_state.stage = newStage;
      m_state.status = fail; 
   }
} // namespace Core
