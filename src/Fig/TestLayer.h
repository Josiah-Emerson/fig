#pragma once
#include "Core_ECS/SortedRegistry.h"
#include "Layer.h"
#include <array>
#include <memory>
#include "Core_Graphics/Shader.h"
#include "Core_Graphics/RenderDevice.h"

class TestLayer : public Core::Layer {
   public: 
      TestLayer();
      bool onEvent(Core::Events::Event& event) override;
      void onUpdate() override;
      void onRender() override;

   private: 
      using ShaderList = std::array<std::shared_ptr<Core::Shader>, 2>;
      using Registry = Core::SortedRegistry<Core::GraphicsComperand, Core::GraphicsCompare, 
                                            Core::PositionComponent>;


      std::shared_ptr<Core::RenderDevice> m_renderDevice;
      ShaderList m_shaders;
      std::shared_ptr<Core::ShaderProgram> m_shaderProgram;
      Registry m_registry;
   };
