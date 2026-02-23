#pragma once
#include "Platform/Platform.h"
// TODO: Very temporary, not sure how I want to go about doing this yet 
// Some very different approaches exist (see ehre vs tracy for imgui


class Ui{
   private: 
      Platform& platform_;

   public: 
      Ui(Platform& platform)
         : platform_ { platform }
      { }

      void updateGui(int* R, int* G, int* B);
};
