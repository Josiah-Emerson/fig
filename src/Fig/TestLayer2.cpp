#include "TestLayer2.h"
#include "Application.h"
#include "Core_Graphics/GraphicsComponents.h"
#include "Core_Graphics/Shader.h"
#include "Core_Graphics/ShaderProgram.h"
#include "Core_ImGui/ImGuiPropertyEditor.h"
#include "Core_Utils/Linear/MatrixTransform.h"
#include "Events.h"
#include <cstring>
#include <memory>

TestLayer2::TestLayer2()
   : m_renderDevice { nullptr }
   , m_program { nullptr } 
   , m_camera {Core::Linear::fvec3{0,0,20}, 60.f, 
               static_cast<float>(Core::Application::get().getWindow().get()->getHeight()),
               static_cast<float>(Core::Application::get().getWindow().get()->getWidth()),
               Core::Linear::fvec3{0,0,0}}
   , m_registry { }
   , m_propertyEditor { nullptr }
{
   // build property editor stuff 
   Core::ImGuiPropertyEditor::DataNode rootNode;
   Core::ImGuiPropertyEditor::DataInfo r{
      "R", ImGuiDataType_Float, &m_color.val[0]
   };
   Core::ImGuiPropertyEditor::DataInfo g{
      "G", ImGuiDataType_Float, &m_color.val[1]
   };
   Core::ImGuiPropertyEditor::DataInfo b{
      "B", ImGuiDataType_Float, &m_color.val[2]
   };
   rootNode.name = "Cube Colors";
   rootNode.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(r));
   rootNode.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(g));
   rootNode.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(b));
   m_propertyEditor = std::make_unique<Core::ImGuiPropertyEditor>(&rootNode, &m_open);

   m_renderDevice = Core::RenderDevice::createRenderDevice(m_registry);
   m_registry.setComperandValidatorFunction(
         [rd = this->m_renderDevice](const Core::GraphicsComperand& cmp){
            return rd->isValidGraphicsComperand(cmp);
         });

   std::shared_ptr<Core::Shader> vertexShader = 
      m_renderDevice->createShader("Resources/Shaders/UnicolorVertex.vs", 
                                    Core::ShaderType::VERTEX_SHADER);
   std::shared_ptr<Core::Shader> fragmentShader = 
      m_renderDevice->createShader("Resources/Shaders/Fragment.fs", 
                                    Core::ShaderType::FRAGMENT_SHADER);

   vertexShader->compile();
   fragmentShader->compile();
   if(!vertexShader->isCompiled()){
      std::cout << "vertexShader log: " << vertexShader->getLogInfo() << '\n';
      std::exit(0);
   }
   if(!fragmentShader->isCompiled()){
      std::cout << "vertexShader log: " << fragmentShader->getLogInfo() << '\n';
      std::exit(0);
   }
m_program = m_renderDevice->createShaderProgram(); m_program->initializeProgram();
   m_program->addShader(vertexShader);
   m_program->addShader(fragmentShader);
   m_program->link();
   if(!m_program->isValid()){
      std::cout << "program not valid: " << m_program->getInfoLog() << '\n';
      std::exit(0);
   }
   using Arg = typename Core::CreateComponentFunction<Core::GraphicsComponentList>::ArgType;
   auto mvp = 
         [&cam = this->m_camera](void** data, Arg arg, std::size_t offset){
            using namespace Core;
            
            const PositionComponent* position = std::get<const PositionComponent*>(arg);
            const ScaleComponent* scale = std::get<const ScaleComponent*>(arg);
            assert(scale && position && "Scale or Position are nullptr");

            auto V = cam.viewMatrix();
            auto P = cam.projectionMatrix();
            auto M = Linear::modelMatrix(position[offset].val, scale[offset].val);
            auto MVP = P * V * M;

            memcpy(*data, &MVP, sizeof(MVP));
         };

   if(!m_program->setUniformCallback("MVP", mvp)){
      std::cout << "Could not set uniform callback. Logs: " << m_program->getInfoLog() << '\n';
   }
   if(!m_program->setUniformCallback("color", 
            [&color = this->m_color](void** data, Arg arg, std::size_t offset){
               using namespace Core;
               float tmp[3] { 
                  color.val[0] / 255.f,
                  color.val[1] / 255.f,
                  color.val[2] / 255.f 
               };
               memcpy(*data, tmp, sizeof(tmp));
            })){
      std::cout << "Could not set uniformCallback. Logs: " << m_program->getInfoLog() << '\n';
   }

   Core::Model model {g_posData, g_colorData};
   m_renderDevice->registerModel(model);

   // TODO: Create grid and stuff

   constexpr int NUM_X { 11 };
   constexpr int NUM_Y { 11 };
   constexpr int NUM_Z { 3 };

   constexpr int WIDTH { 2 };
   constexpr int HEIGHT { 2 };
   constexpr int DEPTH { 2 };

   constexpr int X_SPACE { 2 + WIDTH };
   constexpr int Y_SPACE { 2 + HEIGHT };
   constexpr int Z_SPACE { 5 + DEPTH };

   // start at top left and build across and then down 
   constexpr float X_INITIAL {((NUM_X - 1)/2.f) * X_SPACE * -1};
   constexpr float Y_INITIAL {((NUM_Y - 1)/2.f) * Y_SPACE};
   constexpr float Z_INITIAL { 0 };
   Core::PositionComponent position {{
      X_INITIAL,
      Y_INITIAL,
      Z_INITIAL
   }};

   std::shared_ptr<Core::Model> regModel = std::make_shared<Core::Model>(model);
   std::shared_ptr<Core::Model> rainbowModel = std::make_shared<Core::Model>(g_posData, g_posData);
   m_renderDevice->registerModel(*rainbowModel);

   std::shared_ptr<Core::ShaderProgram> m_regProg = m_renderDevice->createShaderProgram();
   std::shared_ptr<Core::Shader> m_regSh = m_renderDevice->createShader("Resources/Shaders/Vertex.vs", Core::ShaderType::VERTEX_SHADER);
   m_regSh->compile();
   m_regProg->initializeProgram();
   m_regProg->addShader(m_regSh);
   m_regProg->addShader(fragmentShader);
   m_regProg->link();
   m_regProg->setUniformCallback("MVP", mvp);

   Core::GraphicsComperand cmpArr[3] = {
      {m_program, regModel},
      {m_regProg, regModel},
      {m_regProg, rainbowModel}
   };

   for(std::size_t z { 0 }; z < NUM_Z; ++z){
      for(std::size_t y { 0 }; y < NUM_Y; ++y){
         for(std::size_t x { 0 }; x < NUM_X; ++x){
            m_registry.registerNewEntity(cmpArr[z],
                                         Core::PositionComponent{position.val},
                                         Core::ScaleComponent{{1,1,1}},
                                         Core::ColorComponent{{0, 1, 0}});
            position.val[0] += X_SPACE;
         }
         position.val[0] = X_INITIAL;
         position.val[1] -= Y_SPACE;
      }
      position.val[1] = Y_INITIAL;
      position.val[2] -= Z_SPACE;
   }
}

bool TestLayer2::onEvent(Core::Events::Event& event){
   if(event.type == Core::Events::Type::KEY_PRESS){
      m_move = true;
      return true;
   }
   return false;
}

void TestLayer2::onUpdate(float dt){
   if(m_move){
      m_camera.pos() = m_camera.pos() + Core::Linear::fvec3{{0, 0, 0.1f}};
   }

   if(m_open){
      m_propertyEditor->draw();
   }
}

void TestLayer2::onRender(){
   m_renderDevice->drawRegisteredEntities();
}

