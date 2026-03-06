#pragma once
#include "Fig_Windows/IWindow.h"
#include <cstddef> // std::size_t
#include <memory> // std::unique_ptr
#include <vector> // std::vector

namespace Fig{
   // TODO: If this is slow see following vids: 
   // Avoiding Modern C++ | Anton Mikhailov 
   // Basically implements a tree with a data oriented mindset ^
   // CppCon 2014: Chandler Carruth "Efficiency with Algorithms, Performance with Data Structures"
   
   // NOTE: This tree implementation is meant for really just the window (and possibly something like simulation scenes) view of the application
   // Because of this i don't intend for the tree to be sorted at all 
   // NOTE: Because this is currently meant to be used for window the cache misses caused by storing a pointer shouldn't (hopefully) matter too much
   // must store pointer because IWindow is abstract

   // see avoiding modern C++ talk, or Casey's (i think) leaving OOP mindset, but why does this need to be a class
   class WindowTree{
      public:  
         WindowTree() = delete;
         WindowTree(std::unique_ptr<PlatformRootWindow> window); // TODO: Right now PlatformRootWindow will always be the root node of this tree
         // TODO: big 5?

         
      private: 
         struct WindowTreeNode{
            std::unique_ptr<IWindow> window;
            std::size_t parentIdx; // index of parent in tree
            std::vector<std::size_t> childrenIdx; // index of children in tree
         };

         std::vector<WindowTreeNode> Nodes_;
   };

   /*
    * Generic and more cache friendly version of tree not needed yet
   template <typename T>
   struct Node{
      T data;
      size_t parentIdx; // index of parent in Tree
      std::vector<std::size_t> childrenIdx;  // index of children in Tree
   };

   template <typename T>
   using Tree = std::vector<Node<T>>;  

   template <typename T>
   Tree<T> createTree(T rootNodeData);

   template <typename T>
   void addChild(std::size_t parentIdx, T data);

   template <typename T>
   void addChild(Node<T> parentNode, T data);
   */
}
