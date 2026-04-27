#pragma once
#include <vector>

namespace Core{
   class Model{
      public: 
         Model(const std::vector<float>& vertexData, const std::vector<float>& colorData);
         Model(std::vector<float>&& vertexData, std::vector<float>&& colorData);

         const std::vector<float>& getVertexData() const { return m_vertexData; }
         const std::vector<float>& getColorData() const { return m_colorData; }

         std::vector<float>& getVertexData() { return m_vertexData; }
         std::vector<float>& getColorData() { return m_colorData; }

      private: 
         std::vector<float> m_vertexData;
         std::vector<float> m_colorData;
   };
} // namespace Core
