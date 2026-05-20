#pragma once
#include <cstdint>

namespace Core{
   // NOTE: Code using EntityID assumes ID type is unsigned and overflow is defined as wrap around
   using EntityID = unsigned long; // NOTE: Though machine dependent, unsigned long is at least 32 bits (64 on my machine though) which means 4,294,967,295 possible entities. Should be enough

   struct Color{
      std::uint8_t R;
      std::uint8_t G;
      std::uint8_t B;
   };
} // namespace Core
