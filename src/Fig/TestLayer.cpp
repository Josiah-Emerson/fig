#include "TestLayer.h"
#include "Core_ECS/SortedComponentPool.h"
#include "Core_Graphics/ShaderProgram.h"
#include "Core_Utils/Linear/Vector.h"

TestLayer::TestLayer()
   : m_renderDevice {Core::RenderDevice::createRenderDevice()}
{
   m_shaders[0] = m_renderDevice->createShader("./Resources/Shaders/Vertex.vs", 
                                                Core::ShaderType::VERTEX_SHADER);
   m_shaders[1] = m_renderDevice->createShader("./Resources/Shaders/Fragment.fs", 
                                                Core::ShaderType::FRAGMENT_SHADER);

   if(!m_shaders[0]->compile()){
      std::cout << "Vertex.vs is not compiled.\n";
      std::cout << "Log info: " << m_shaders[0]->getLogInfo() << '\n';
      std::exit(0);
   }

   if(!m_shaders[1]->compile()){
      std::cout << "Fragment.fs is not compiled.\n";
      std::cout << "Log info: " << m_shaders[1]->getLogInfo() << '\n';
      std::exit(0);
   }

   if(!m_shaders[0]->isCompiled()){
      std::cout << "Vertex.vs compile() returned true, but isCompiled() returns false\n";
      std::cout << "Log info: " << m_shaders[0]->getLogInfo() << '\n';
      std::exit(0);
   }
   if(!m_shaders[1]->isCompiled()){
      std::cout << "Fragment.fs compile() returned true, but isCompiled() returns false\n";
      std::cout << "Log info: " << m_shaders[1]->getLogInfo() << '\n';
      std::exit(0);
   }

   m_shaderProgram = m_renderDevice->createShaderProgram();
   if(!m_shaderProgram->initializeProgram()){
      std::cout << "Error initializing program\n";
      std::exit(0);
   }

   if(!m_shaderProgram->addShader(m_shaders[0])){
      std::cout << "Error adding Vertex.vs to the shader program\n";
      std::cout << "Shader program log: " << m_shaderProgram->getInfoLog() << '\n';
      std::exit(0);
   }
   if(!m_shaderProgram->addShader(m_shaders[1])){
      std::cout << "Error adding Fragment.fs to the shader program\n";
      std::cout << "Shader program log: " << m_shaderProgram->getInfoLog() << '\n';
      std::exit(0);
   }

   if(!m_shaderProgram->link()){
      std::cout << "Error linking program: " << m_shaderProgram->getInfoLog() << '\n';
      std::exit(0);
   }

   std::cout << "SUCCESS\n";
   auto attr = m_shaderProgram->getAttributeVariables();
   std::cout << "Attribute variables: \n";
   for(const auto& e : attr){
      std::cout << "Name: " << e.name << '\n';
      std::cout << "Is this of type F_VEC3? ";
      e.type == Core::F_VEC3 ? std::cout << "yes\n" : std::cout << "no\n";
      std::cout << '\n';
   }
   
   auto uni = m_shaderProgram->getUniformVariables();
   std::cout << "Uniform variables: \n";
   for(const auto& e : uni){
      std::cout << "Name: " << e.name << '\n';
      std::cout << "Is this of type F_MAT4? ";
      e.type == Core::F_MAT4 ? std::cout << "yes\n" : std::cout << "no\n";
      std::cout << '\n';
   }

   Core::SortedComponentPool<Core::Linear::ivec3, std::less<int>, int> pool;
   int comp = 0;
   Core::EntityID id = 0;
   std::cout << "Inserting\n";
   for(int i { 0 }; i < 1000; ++i){
      std::cout << i << '\n';
      assert(pool.insert(id, Core::Linear::ivec3{i, i, i}, comp) && "Returned false");
      ++id;
      if(i % 100 == 0)
         ++comp;
   }
   std::cout << "Insertion complete\n";
   assert(pool.insert(40404, {0, 0, 0}, 1) && "Returned false");

   pool.remove(0);
   const auto& sep = pool.separatorList();
   const auto& data = pool.contiguousData();
   for(const auto& e : sep){
      for(std::size_t i{e.second.first}; i <= e.second.second; ++i){
         std::cout << "class " << e.first << ": " << data[i] << " (ID: " << pool.idFromIndex(i) << ")\n";
      }
   }

}

bool TestLayer::onEvent(Core::Events::Event& event) { return false; }
void TestLayer::onUpdate() { }
void TestLayer::onRender() { }

