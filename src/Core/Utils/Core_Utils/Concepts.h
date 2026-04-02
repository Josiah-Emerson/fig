#pragma once
#include <type_traits>

namespace Core{
   namespace Concepts{

      // NOTE: Numeric to me means numbers and not things such as char or bools
      // NOTE: for type T which std::is_arithmetic_v returns true for, it will return false for T* and T& meaning this concept resstricts T to not being a pointer or a reference
      template<typename T>
      concept numeric = std::is_arithmetic_v<T> &&
                        !std::is_same_v<T, bool> &&
                        !std::is_same_v<T, char> &&
                        !std::is_same_v<T, char const>;

   } // namespace Concepts
} // namespace Core
