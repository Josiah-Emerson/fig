#include "CameraRotateLayer.h"
#include "Application.h"
#include "Core_Graphics/GraphicsComponents.h"
#include "Core_Graphics/RenderDevice.h"
#include "Core_ImGui/ImGuiPropertyEditor.h"
#include "Core_Utils/Linear/MatrixTransform.h"
#include "Events.h"
#include "imgui.h"
#include <cstring>

CameraRotateLayer::CameraRotateLayer()
   : m_camera { {0, 0, 10}, 45.f, 
               static_cast<float>(Core::Application::get().getWindow()->getHeight()),
               static_cast<float>(Core::Application::get().getWindow()->getWidth()),
               {0, 0, 0} }
   , m_graphicsRegistry { }
   , m_renderDevice { Core::RenderDevice::createRenderDevice(m_graphicsRegistry) }
   , m_graphicsComperands { }
   , m_imGuiPropertyEditor { nullptr }
{
   m_graphicsRegistry.setComperandValidatorFunction(
         [&rd = this->m_renderDevice](const Core::GraphicsComperand& cmp){
            return rd->isValidGraphicsComperand(cmp);
         });
   
   // set up graphics stuff 
   std::shared_ptr<Core::Model> regularModel = std::make_shared<Core::Model>(g_posData, g_colorData);
   std::shared_ptr<Core::Model> rainbowModel = std::make_shared<Core::Model>(g_posData, g_posData);
   m_renderDevice->registerModel(*regularModel);
   m_renderDevice->registerModel(*rainbowModel);

   std::shared_ptr<Core::Shader> fragmentShader = createShader("Resources/Shaders/Fragment.fs", Core::ShaderType::FRAGMENT_SHADER);
   std::shared_ptr<Core::Shader> unicolorVertex = createShader("Resources/Shaders/UnicolorVertex.vs", Core::ShaderType::VERTEX_SHADER);
   std::shared_ptr<Core::Shader> regularVertex = createShader("Resources/Shaders/Vertex.vs", Core::ShaderType::VERTEX_SHADER);

   std::shared_ptr<Core::ShaderProgram> unicolorProgram = createShaderProgram(unicolorVertex, fragmentShader);
   std::shared_ptr<Core::ShaderProgram> regularProgram = createShaderProgram(regularVertex, fragmentShader);

   using Arg = typename Core::CreateComponentFunction<Core::GraphicsComponentList>::ArgType;
   auto mvp = [&cam = this->m_camera](void** data, Arg arg, std::size_t offset){
      using namespace Core;

      const PositionComponent* position = std::get<const PositionComponent*>(arg);
      const ScaleComponent* scale = std::get<const ScaleComponent*>(arg);
      assert(position && scale && "Position or Scale nullptr in MVP function callback");

      // TODO: This gets calculated for each call of this function, so we should probably 
      // change how we pass this value here 
      auto V = cam.viewMatrix();
      auto P = cam.projectionMatrix();
      auto M = Linear::modelMatrix(position[offset].val, scale[offset].val);
      auto MVP = P * V * M;

      memcpy(*data, &MVP, sizeof(MVP));
   };

   auto color = [](void** data, Arg arg, std::size_t offset){
      using namespace Core;

      const ColorComponent* color = std::get<const ColorComponent*>(arg);
      // TODO: This works if we assume that every graphics device is RGB on a scale of 0 - 1.f
      // Need to figure out a way to make it so that it literally just points to our agnostic Color 
      // type and then the implementation figures out how to manipulate that value
      float tmp[3] = 
      {
         static_cast<float>(color->val.R) / 255.f,
         static_cast<float>(color->val.G) / 255.f,
         static_cast<float>(color->val.B) / 255.f,
      };

      memcpy(*data, tmp, sizeof(tmp));
   };

   regularProgram->setUniformCallback("MVP", mvp);
   unicolorProgram->setUniformCallback("MVP", mvp);
   unicolorProgram->setUniformCallback("color", color);

   m_graphicsComperands[0] = { unicolorProgram, regularModel };
   m_graphicsComperands[1] = { regularProgram, rainbowModel };
   m_graphicsComperands[2] = { regularProgram, regularModel };

   Core::EntityID id = m_graphicsRegistry.registerNewEntity(m_graphicsComperands[1], 
                                        Core::PositionComponent{{0, 0, 0}}, 
                                        Core::ScaleComponent{{1, 1, 1}}, 
                                        Core::ColorComponent{{128, 128, 255}});

   // set up ImGui
   Core::ImGuiPropertyEditor::DataNode rootNode;
   rootNode.name = "CameraRotateLayer Settings";

   Core::ImGuiPropertyEditor::DataNode comperandNode, movementNode, cameraNode;
   rootNode.firstChild = &comperandNode;
   comperandNode.nextSibling = &movementNode;
   movementNode.nextSibling = &cameraNode;

   // Comperand Settings: 
   comperandNode.name = "Comperand Settings";
   Core::ImGuiPropertyEditor::DataInfo comperand {
      "Unfinished", ImGuiDataType_S32, &tmp };
   comperandNode.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(comperand));

   // Movement Settings: 
   movementNode.name = "Cube Spatial Settings";
   Core::ImGuiPropertyEditor::DataNode pos, scale, rot;
   movementNode.firstChild = &pos;
   pos.nextSibling = &scale;
   scale.nextSibling = &rot;

   pos.name = "Position";
   auto& positionPool = m_graphicsRegistry.getPool<Core::PositionComponent>();

   pos.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "X", ImGuiDataType_Float, &positionPool.id(id).val[0]));
   pos.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "Y", ImGuiDataType_Float, &positionPool.id(id).val[1]));
   pos.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "Z", ImGuiDataType_Float, &positionPool.id(id).val[2]));

   scale.name = "Scale";
   auto& scalePool = m_graphicsRegistry.getPool<Core::ScaleComponent>();
   scale.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "X", ImGuiDataType_Float, &scalePool.id(id).val[0]));
   scale.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "Y", ImGuiDataType_Float, &scalePool.id(id).val[1]));
   scale.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "Z", ImGuiDataType_Float, &scalePool.id(id).val[2]));

   // TODO: Rotation

   // Camera Settings: 
   cameraNode.name = "Camera Settings";
   cameraNode.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "FOV", ImGuiDataType_Float, &m_camera.fov()));

   Core::ImGuiPropertyEditor::DataNode directionPos;
   directionPos.name = "Look At";
   cameraNode.firstChild = &directionPos;
   directionPos.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "X", ImGuiDataType_Float, &m_camera.look()[0]));
   directionPos.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "Y", ImGuiDataType_Float, &m_camera.look()[1]));
   directionPos.dataInfoArr.push_back(std::make_shared<Core::ImGuiPropertyEditor::DataInfo>(
            "Z", ImGuiDataType_Float, &m_camera.look()[2]));

   m_imGuiPropertyEditor = std::make_unique<Core::ImGuiPropertyEditor>(&rootNode);
}

bool CameraRotateLayer::onEvent(Core::Events::Event& event) {
   if(event.type == Core::Events::Type::KEY_PRESS){
      constexpr float SPEED { 0.5f };
      switch(event.keyEvent.key){
         case(Core::Events::Key::W):
         {
            m_camera.pos()[2] -= SPEED;
            return true;
         }
         case(Core::Events::Key::A):
         {
            m_camera.pos()[0] -= SPEED;
            return true;
         }
         case(Core::Events::Key::S):
         {
            m_camera.pos()[2] += SPEED;
            return true;
         }
         case(Core::Events::Key::D):
         {
            m_camera.pos()[0] += SPEED;
            return true;
         }
         default:
            break;
      }
   }
   return false;
}

void CameraRotateLayer::onUpdate(){
   auto posPool = m_graphicsRegistry.getPool<Core::PositionComponent>();

   m_imGuiPropertyEditor->draw();
}

void CameraRotateLayer::onRender(){
   m_renderDevice->drawRegisteredEntities();
}

/*
 *
 * PRIVATE METHOD IMPL 
 *
 */

std::shared_ptr<Core::Shader> CameraRotateLayer::createShader(const char* path, Core::ShaderType type){
   std::shared_ptr<Core::Shader> shader = m_renderDevice->createShader(path, type);
   shader->compile();
   return shader;
}

std::shared_ptr<Core::ShaderProgram> CameraRotateLayer::createShaderProgram(std::shared_ptr<Core::Shader> vertex, std::shared_ptr<Core::Shader> fragment){
   std::shared_ptr<Core::ShaderProgram> program = m_renderDevice->createShaderProgram();
   program->initializeProgram();
   program->addShader(vertex);
   program->addShader(fragment);
   program->link();

   return program;
}
