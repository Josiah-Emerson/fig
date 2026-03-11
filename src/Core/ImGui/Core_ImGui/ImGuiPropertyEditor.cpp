#include "ImGuiPropertyEditor.h"
#include "Application.h"
#include "imgui.h"

namespace Core{

   // p_open has default arg
   ImGuiPropertyEditor::ImGuiPropertyEditor(const DataNode* const rootNode, bool* const p_open)
      : ImGuiContainer { p_open }
   {
      // construct tree
      assert(!rootNode->parent && "rootNode has a parent node");
      Node root = createNode(rootNode);
      buildTree(addNode(root), rootNode->firstChild);
   }

   ImGuiPropertyEditor::~ImGuiPropertyEditor(){
   }

   void ImGuiPropertyEditor::draw(){
      const int WINDOW_HEIGHT = Application::get().getWindow()->getHeight();
      const int WINDOW_WIDTH = Application::get().getWindow()->getWidth();
      const int HEIGHT { WINDOW_HEIGHT / 2 };
      const int WIDTH { 400 };

      ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
      ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH - WIDTH, 0));
      if(ImGui::Begin("Property Editor", getOpenBool()))
         drawTree();

      // NOTE: If we want to handle ImGui::Begin returning false, invert if statement, ensure we add 
      // ImGui::End() in if statement, handle this, and then early return in if block
      ImGui::End();
   }

   // PRIVATE METHODS
   ImGuiPropertyEditor::Node ImGuiPropertyEditor::createNode(const DataNode* const node){
      return {
         node->name, 
         node->parent ? -2 : -1, // if it has a parent -2 because we will need to add as child, if nullptr then it is root and can be -1
         {}, // empty childIdx vector
         //std::move( node->dataInfoArr )
         node->dataInfoArr
      };
   }

   std::size_t ImGuiPropertyEditor::addNode(const Node& node){
      m_nodes.push_back( std::move(node) );
      return m_nodes.size() - 1;
   }

   void ImGuiPropertyEditor::addChild(std::size_t parentIdx, std::size_t childIdx){
      m_nodes[parentIdx].childIdx.push_back(childIdx);
      // TODO: assert there is no parent yet?
      m_nodes[childIdx].parentIdx = parentIdx;
   }

   void ImGuiPropertyEditor::buildTree(std::size_t parentIdx, const DataNode* const child){
      assert(m_nodes.size() >= 1 && "trying to build tree on empty m_nodes vector");
      if(!child) // leaf
         return;

      Node node = createNode(child);
      std::size_t currIdx = addNode(node);
      addChild(parentIdx, currIdx);

      if(child->firstChild) // depth first
         buildTree(currIdx, child->firstChild);

      buildTree(parentIdx, child->nextSibling);
   }

   void ImGuiPropertyEditor::drawTree(){
      // Left Side
      const ImVec2 IMGUI_WINDOW_SIZE = ImGui::GetContentRegionAvail();
      if(ImGui::BeginChild("Tree", ImVec2(IMGUI_WINDOW_SIZE.x / 2, IMGUI_WINDOW_SIZE.y), 
               ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX )){
         if(ImGui::BeginTable("Tree", 1, 
                  ImGuiTableFlags_ScrollY | ImGuiTableFlags_HighlightHoveredColumn)){
            assert(m_nodes.size() > 0 && "Somehow m_nodes is empty"); // TODO: eventually change to early return but if this happens it would be good to know about for now
            drawTreeNode(0); // start with rootNode

            ImGui::EndTable();
         }
         ImGui::EndChild();
      }

      // Right Side
      ImGui::SameLine();
      ImGui::BeginGroup(); // lock horizontal position
      
      if(m_visibleNode){
         ImGui::TextUnformatted(m_visibleNode->name.c_str());
         // ImGui::Separator();
         if(ImGui::BeginTable("Properties", 2, ImGuiTableFlags_ScrollY)){
            std::string tempID = "##" + m_visibleNode->name;
            ImGui::PushID(tempID.c_str());
            if(!m_visibleNode->dataInfoArr.empty()){
               for(std::shared_ptr<DataInfo> dataInfo : m_visibleNode->dataInfoArr){
                  ImGui::TableNextRow();
                  tempID = "##" + dataInfo->propertyName; // TODO: When entering draw func, dataInfo->propertyName results in std::bad_alloc
                  ImGui::PushID(tempID.c_str()); // Push after row and before column so this ID is for the whole row ? Not sure
                  ImGui::TableNextColumn();
                  ImGui::Text("%s:", dataInfo->propertyName.c_str());
                  
                  ImGui::TableNextColumn();
                  switch(dataInfo->dataType){
                     case(ImGuiDataType_S32):
                     {
                        ImGui::DragInt(dataInfo->propertyName.c_str(), (int*)dataInfo->data, 1.f, 0, 255);
                        break;
                     }
                     default:
                        assert(false && "Data type not handled");
                  }
                  ImGui::PopID();
               }
            }
            ImGui::PopID();
            ImGui::EndTable();
         }
      }

      ImGui::EndGroup();
   }

   void ImGuiPropertyEditor::drawTreeNode(const std::size_t nodeIdx){
      const Node& node = m_nodes[nodeIdx];
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::PushID("##"); // TODO: Change if we end up caring about the ID
      ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                                 ImGuiTreeNodeFlags_SpanFullWidth /* | 
                                 ImGuiTreeNodeFlags_DrawLinesToNodes */;

      if(nodeIdx == 0)
         flags |= ImGuiTreeNodeFlags_DefaultOpen;
      if(&node == m_visibleNode)
         flags |= ImGuiTreeNodeFlags_Selected |
                  ImGuiTreeNodeFlags_Bullet |
                  ImGuiTreeNodeFlags_Leaf;

      if(node.dataInfoArr.empty())
         ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);

      // TreeNodeEx pushes this node to the tree and returns true if this node is open and we need to pop
      bool nodeOpen = ImGui::TreeNodeEx("", flags, "%s", node.name.c_str()); 
      
      if(node.dataInfoArr.empty())
         ImGui::PopStyleColor();

      if(ImGui::IsItemFocused())
         m_visibleNode = &node;

      if(nodeOpen){
         for(const std::size_t childIdx : node.childIdx)
            drawTreeNode(childIdx);

         ImGui::TreePop();
      }

      ImGui::PopID();
   }
   
} // namespace Core
