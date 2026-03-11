#pragma once
#include "ImGuiContainer.h"
#include <imgui.h>
#include <memory>
#include <string>
#include <vector>

namespace Core{
   // builds and displays a property editor based on the tree defined by rootNode in constructor
   // TODO: Add functionality for passing a window size (possibly through the ImGuiContainer constructor)
   // TODO: Add functionality for passing window flags or table flags (possible through ImGuiContainer or ImGuiPropertyEditor)
   class ImGuiPropertyEditor : public ImGuiContainer {
      public: 
         struct DataNode;
         ImGuiPropertyEditor(const DataNode* const rootNode, bool* const p_open = nullptr);
         ~ImGuiPropertyEditor();
         // TODO: Big 5

         // TODO: If this tree is slow see following vids: 
         // Avoiding Modern C++ | Anton Mikhailov 
         // Basically implements a tree with a data oriented mindset ^
         // CppCon 2014: Chandler Carruth "Efficiency with Algorithms, Performance with Data Structures"
         // TODO: IMPORTANT: If external structure changes, how can we ensure we stay up to date in imgui?
         struct DataInfo;
         struct DataNode{
            std::string name;
            DataNode* parent { nullptr }; // nullptr if root node
            DataNode* firstChild { nullptr }; // nullptr if leaf 
            DataNode* nextSibling { nullptr }; // nullptr if last sibling 
            std::vector< std::shared_ptr<DataInfo>> dataInfoArr;
         };

         struct DataInfo{
            std::string propertyName;
            ImGuiDataType dataType { ImGuiDataType_COUNT };
            void* const data { nullptr };
         };

      private: 
         struct Node{
            std::string name;
            int parentIdx; // -1 is parent -2 is no parent and not root (i.e. something went wrong, probably never added as child)
            std::vector<std::size_t> childIdx;
            std::vector< std::shared_ptr<DataInfo> > dataInfoArr;

            bool operator==(const Node& rhs) const;
         };

         void internalDraw() override;
         // create Node from DataNode.
         // NOTE: If not root node, we still need to add as child
         Node createNode(const DataNode* const node);
         // Adds node to m_nodes via std::move and returns index in m_nodes
         std::size_t addNode(const Node& node);
         void addChild(std::size_t parentIdx, std::size_t childIdx);
         void buildTree(std::size_t parentIdx, const DataNode* const firstChild);
         void drawTreeNode(const std::size_t nodeIdx);

         std::vector<Node> m_nodes;
         const Node* m_visibleNode = nullptr;
   };
} // namespace Core
