#include "ImGuiContainer.h"
#include "Application.h"
#include "imgui.h"

namespace Core{
   ImGuiContainer::ImGuiContainer(int& red, int& green, int& blue)
      : m_nodes {}
   { 
      // NOTE: only here so that we can see the rootNode below in tree display
      Node actualRootNode;
      actualRootNode.name = "ROOT";
      actualRootNode.parentIdx = 0;
      m_nodes.push_back(actualRootNode);

      Node rootNode;
      rootNode.name = "Color";
      addChild(rootNode, m_nodes[0]);

      Node redNode;
      redNode.name = "red";
      redNode.dataType = ImGuiDataType_S32;
      redNode.data = &red;
      redNode.hasData = true;

      Node blueNode;
      blueNode.name = "blue";
      blueNode.dataType = ImGuiDataType_S32;
      blueNode.data = &blue;
      blueNode.hasData = true;

      Node greenNode;
      greenNode.name = "green";
      greenNode.dataType = ImGuiDataType_S32;
      greenNode.data = &green;
      greenNode.hasData = true;

      addChild(redNode, m_nodes[1]);
      addChild(blueNode, m_nodes[1]);
      addChild(greenNode, m_nodes[1]);
   }

   void ImGuiContainer::draw(){
      const int WINDOW_HEIGHT = Application::get().getWindow()->getHeight();
      const int WINDOW_WIDTH = Application::get().getWindow()->getWidth();
      const int HEIGHT { WINDOW_HEIGHT / 2 };
      const int WIDTH { 400 };

      ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
      ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - WIDTH, 0));
      if(!ImGui::Begin("Color Control", &m_open)){
         // TODO: If this stays then we need to actually handle this probably
         ImGui::End();
         return;
      }

      drawNodes();
      ImGui::End();
   }

   // TODO: Obviously this is very coupled to this specifc thing and we will need to abstract a bit
   // NOTE: Most of idea for drawing taken from imgui demo PropertyEditor
   void ImGuiContainer::drawNodes(){
      const Node& ROOT_NODE = m_nodes[0];

      // Left Side 
      if(ImGui::BeginChild("##Tree", ImVec2(100, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders)){
         if(ImGui::BeginTable("##bg", 1)){ // TODO: Add table flags

            // TODO: I don't think this will work (see TreeNodeEx() in drawNode fun)
            // I believe we need to just pass the root node in 
            // if true, rename funcs to be clearer? Or doesn't really matter since this is probably gonna be moved from container
            for(const std::size_t childIdx : m_nodes[0].childIdx)
               drawNode(childIdx);
                                          
            ImGui::EndTable(); // NOTE: For table, we only end if BeginTable returns true whereas child you always need matching end
         }
      }
      ImGui::EndChild();

      ImGui::SameLine();
      ImGui::BeginGroup();
      
      if(m_visibleNode){
         ImGui::Text("%s", m_visibleNode->name.c_str());
         std::string tempIDString = "##" + m_visibleNode->name;
         unsigned int ID = ImGui::GetID(tempIDString.c_str());
         ImGui::TextDisabled("ID: %u", ID);
         ImGui::Separator();

         if(ImGui::BeginTable("##properties", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable)){
            // TODO: demo says push object ID after table entry so that table is shared for all objects
            // Figure out why/what this means
            ImGui::PushID(tempIDString.c_str());
            if(m_visibleNode->hasData){
               ImGui::TableNextRow(); // NOTE: need this to enter into first cell of a new row
                                      
               // TODO: why push another ID here? In imgui demo it is a different field but still
               // Check what imgui demo data looks like. I think a leaf doesn't need one value but can be an array of values
               ImGui::PushID(tempIDString.c_str());
               ImGui::TableNextColumn();
               //ImGui::AlignTextToFramePadding();
               ImGui::TextUnformatted(m_visibleNode->name.c_str());
               ImGui::TableNextColumn();
               switch(m_visibleNode->dataType){
                  case(ImGuiDataType_S32): 
                  {
                     assert(m_visibleNode->data != nullptr && "data member of node is nullptr");
                     ImGui::DragInt(m_visibleNode->name.c_str(), (int*)m_visibleNode->data, 1.f, 0, 255);
                     break;
                  }
                  default:
                  {
                     assert(false && "Unhandled ImGuiDataType");
                  }
               }
               ImGui::PopID();
            }
            ImGui::PopID();
            ImGui::EndTable();
         }
      }

      ImGui::EndGroup();
   }

   void ImGuiContainer::drawNode(const std::size_t nodeIdx){
      const Node& node = m_nodes[nodeIdx];

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      const std::string ID = "##" + node.name; // TODO: obv temp string but ImGui shouldn't need it past this func
      ImGui::PushID(ID.c_str());

      ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
      flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;// Standard opening mode as we are likely to want to add selection afterwards
      flags |= ImGuiTreeNodeFlags_NavLeftJumpsToParent;  // Left arrow support
      flags |= ImGuiTreeNodeFlags_SpanFullWidth;         // Span full width for easier mouse reach 
      flags |= ImGuiTreeNodeFlags_DrawLinesToNodes;      // Always draw hierarchy outlines

      if(&node == m_visibleNode)
         flags |= ImGuiTreeNodeFlags_Selected;

      if(&node == m_visibleNode)
         flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;

      if(!node.hasData)
         ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);

      // TODO/NOTE: No idea what this does? TreeNodeEx checks if open and returns true. 
      // If true then we need to pop it from the tree. I think "" can be anything and just acts as an ID?
      // Chaning to "a" or some other string works fine
      bool nodeOpen = ImGui::TreeNodeEx("", flags, "%s", node.name.c_str());

      if(!node.hasData)
         ImGui::PopStyleColor();

      if(ImGui::IsItemFocused())
         m_visibleNode = &node;

      if(nodeOpen){
         for(const std::size_t childIdx : node.childIdx)
            drawNode(childIdx);

         ImGui::TreePop();
      }

      ImGui::PopID();
   }

   void ImGuiContainer::addChild(Node& childNode, const Node& parentNode){
      int parentIdx { -1 }; // TODO: int for now so that we can check if it is still -1 and assert 
      for(std::size_t i { 0 }; i < m_nodes.size(); ++i){
         if(parentNode == m_nodes[i]){
            parentIdx = i;
            break;
         }
      }

      assert(parentIdx != -1 && "parentNode not found in m_nodes");
      addChild(childNode, static_cast<std::size_t>(parentIdx));
   }

   void ImGuiContainer::addChild(Node& childNode, const std::size_t parentIdx){
      std::size_t childIdx = m_nodes.size(); 
      m_nodes[parentIdx].childIdx.push_back(childIdx);  

      childNode.parentIdx = parentIdx;
      m_nodes.push_back(childNode); // TODO: See important at top of header, but is push back fine or should we move?
   }

   bool ImGuiContainer::Node::operator==(const Node& rhs) const{
      return this == &rhs ? true : false;
   }

} // namespace Core
