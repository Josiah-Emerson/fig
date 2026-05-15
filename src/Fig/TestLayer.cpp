#include "TestLayer.h"
#include <cstring>
#include <memory>
#include "Application.h"
#include "Core_Graphics/GraphicsComponents.h"
#include "Core_Graphics/RenderDevice.h"
#include "Core_Utils/Linear/MatrixTransform.h"
#include "Core_Utils/Linear/Vector.h"

std::vector<float> g_posData{
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};
std::vector<float> g_colorData {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

TestLayer::TestLayer()
   : m_renderDevice {nullptr}
   , m_shaders { }
   , m_registry { }
   , m_camera { {-8, 7, 3}, 70.f, 
               static_cast<float>(Core::Application::get().getWindow()->getHeight()), 
               static_cast<float>(Core::Application::get().getWindow()->getWidth()), 
               {0, 0, 0}}
{
   m_renderDevice = Core::RenderDevice::createRenderDevice(m_registry);

   // NOTE: rd becomes a shared_ptr and thus should be up to date with any state changed in the render device 
   m_registry.setComperandValidatorFunction(
         [rd = this->m_renderDevice](const Core::GraphicsComperand& cmp){
            return rd->isValidGraphicsComperand(cmp);
         });


   m_shaders[0] = m_renderDevice->createShader("./Resources/Shaders/Vertex.vs", 
                                                Core::ShaderType::VERTEX_SHADER);
   m_shaders[1] = m_renderDevice->createShader("./Resources/Shaders/Fragment.fs", 
                                                Core::ShaderType::FRAGMENT_SHADER);
   m_shaders[2] = m_renderDevice->createShader("./Resources/Shaders/UnicolorVertex.vs",
                                                Core::ShaderType::VERTEX_SHADER);

   Core::Model cubeModel {g_posData, g_colorData};

   assert(m_renderDevice->registerModel(cubeModel) && "Could not register model");

   // Compile shaders

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

   if(!m_shaders[2]->compile()){
      std::cout << "UnicolorVertex.vs did not compile.\n";
      std::cout << "Log info: " << m_shaders[2]->getLogInfo() << '\n';
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
   if(!m_shaders[2]->isCompiled()){
      std::cout << "UnicolorVertex.vs compile() returned true, but isCompiled() returned false\n";
      std::cout << "Log info: " << m_shaders[2]->getLogInfo() << '\n';
      std::exit(0);
   }


   m_shaderProgram = m_renderDevice->createShaderProgram();
   m_shaderProgram->initializeProgram();

   m_unicolorShaderProgram = m_renderDevice->createShaderProgram();
   m_unicolorShaderProgram->initializeProgram();

   // Add shaders and link
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

   // UNICOLOR
   if(!m_unicolorShaderProgram->addShader(m_shaders[2])){
      std::cout << "Error adding UnicolorVertex.vs to the shader program\n";
      std::cout << "Shader program log: " << m_unicolorShaderProgram->getInfoLog() << '\n';
      std::exit(0);
   }
   if(!m_unicolorShaderProgram->addShader(m_shaders[1])){
      std::cout << "Error adding Fragment.fs to the shader program\n";
      std::cout << "Shader program log: " << m_unicolorShaderProgram->getInfoLog() << '\n';
      std::exit(0);
   }
   if(!m_unicolorShaderProgram->link()){
      std::cout << "Error linking program: " << m_unicolorShaderProgram->getInfoLog() << '\n';
      std::exit(0);
   }


   // set m_shaderProgram's uniform var callbacks
   // TODO: is capuring the camera lik this fine?
   // TODO: How can we make it so that we can define the function params better? member function?
   using ArgType = typename Core::CreateComponentFunction<Core::GraphicsComponentList>::ArgType;
   auto mvpCallback = [&cam = this->m_camera](void** data, ArgType arg, std::size_t offset){
            using namespace Core;

            const PositionComponent* position = std::get<const PositionComponent*>(arg);
            const ScaleComponent* scale = std::get<const ScaleComponent*>(arg);

            assert(position && "Position component is nullptr");
            assert(scale && "Scale component is nullptr");

            auto V = cam.viewMatrix();
            auto P = cam.projectionMatrix();
            auto M = Linear::modelMatrix(
                  static_cast<PositionComponent::Type>(position[offset]), 
                  static_cast<ScaleComponent::Type>(scale[offset]));
            auto MVP = P * V * M;
            memcpy(*data, &MVP, sizeof(MVP));
   };

   m_shaderProgram->setUniformCallback("MVP", mvpCallback);
   m_unicolorShaderProgram->setUniformCallback("MVP", mvpCallback);
   m_unicolorShaderProgram->setUniformCallback("color", 
         [](void** data, ArgType arg, std::size_t offset){
            using namespace Core;
            const ColorComponent* color = std::get<const ColorComponent*>(arg);
            assert(color && "Color component is nullptr");
            *data = (void*)color;
         });

   // now that shaderProgram is linked, and we have a registered Model with renderDevice, we can register with the registry
   // NOTE: doing std::make_shared<Model>(model) creates a new shared ptr to the model each time I believe. Look into what is the 
   // semantically correct way to do this
   Core::Model rainbowModel { g_posData, g_posData };
   m_renderDevice->registerModel(rainbowModel);
   Core::GraphicsComperand comp {m_shaderProgram, std::make_shared<Core::Model>(cubeModel)};

   m_registry.registerNewEntity(comp, Core::PositionComponent{{0, 0, 0}},
                                      Core::ScaleComponent{{1, 1, 1}});
   m_registry.registerNewEntity(Core::GraphicsComperand{m_shaderProgram, std::make_shared<Core::Model>(rainbowModel)}, 
                                Core::PositionComponent{{-2, 2, -4.5}},
                                Core::ScaleComponent{{1, 1, 1}});
   m_registry.registerNewEntity(Core::GraphicsComperand{m_unicolorShaderProgram, std::make_shared<Core::Model>(cubeModel)},
                                Core::PositionComponent{{2, 2, 3}},
                                Core::ScaleComponent{{1, 1, 1}},
                                Core::ColorComponent{{1, 0, 0}}); 
}


bool TestLayer::onEvent(Core::Events::Event& event) { return false; }
void TestLayer::onUpdate() { 
   m_camera.pos() = m_camera.pos() + Core::Linear::fvec3{0, 0, 0.1};
}
void TestLayer::onRender() { 
   m_renderDevice->drawRegisteredEntities();
}
