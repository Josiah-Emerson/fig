#include "Model.h"

namespace Core{
   bool Model::operator==(const Model& other) const{
      return (m_vertexData == other.m_vertexData) && (m_colorData == other.m_colorData);
   }
} // namespace Core

