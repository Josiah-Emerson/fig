#pragma once

#include <cwchar>
#include <imgui.h>
#include <string>
#include <vector>
#include <iostream>

namespace Core{
   // TODO: IMPORTANT!!! When inserting into m_nodes, should we std::move it? are references fine?
   // should we create a copy? Does it matter? it being Node

   class ImGuiContainer{
      public: 
         ImGuiContainer(int& red, int& green, int& blue); // TODO: change after quick test
         ~ImGuiContainer() = default; // TODO: change after quick test
         // TODO: Big 5

         void draw();

      private: 
         struct Node{
            std::string name;
            std::size_t parentIdx;
            std::vector<std::size_t> childIdx;

            bool hasData { false }; // true if leaf and should have some way to 
            ImGuiDataType dataType { ImGuiDataType_COUNT };
            void* data = nullptr; // Address of value

            Node()
            {
               std::cout << "New Node Created\n";
            }

            bool operator==(const Node& rhs) const;
         };

         void addChild(Node& childNode, const Node& parentNode);
         void addChild(Node& childNode, const std::size_t parentIdx);
         void drawNodes();
         void drawNode(const std::size_t nodeIdx);

         // TODO: This might need to be vec of pointers (will need to see if performance gets bad)
         // Otherwise operator==() will not work well for Nodes because it needs to check address, 
         // and not just if all of the values are the same because we might have multiple of the same 'type' of Node
         // Also this might just take a long time to do all those comparisons
         std::vector<Node> m_nodes; // Root node should always be at index 0
         bool m_open { true };
         const Node* m_visibleNode = nullptr;
   };
} // namespace Core
