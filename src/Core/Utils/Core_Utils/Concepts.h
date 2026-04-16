#pragma once
#include <type_traits>

namespace Core{
   namespace Concepts{

      // NOTE: Numeric to me means numbers and not things such as char or bools
      // NOTE: for type T which std::is_arithmetic_v returns true for, it will return false for T* and T& meaning this concept resstricts T to not being a pointer or a reference
      /*
      template<typename T>
      concept numeric = std::is_arithmetic_v<T> &&
                        !std::is_same_v<T, bool> &&
                        !std::is_same_v<T, char> &&
                        !std::is_same_v<T, char const>;
                        */
      template<typename... Args>
      concept numeric = (std::is_arithmetic_v<Args> && ...) &&
                        (!std::is_same_v<Args, bool> && ...) &&
                        (!std::is_same_v<Args, char> && ...) &&
                        (!std::is_same_v<Args, char const> && ...);

      // I guess not necessarily a concept but oh well
      // shortcut for type promotion for example: float_promotion_t<int, int> returns float
      // Useful for functions which can take two integral operands (but they don't have to be integral) 
      // but will need to return floating point value to be useful
      template<typename... Args>
      using float_promotion_t = std::common_type_t<float, Args...>;


      template<typename T>
      concept is_component = std::is_trivial_v<T> && std::is_standard_layout_v<T>;

   } // namespace Concepts
} // namespace Core
