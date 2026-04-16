#pragma once
#include <type_traits>

/* From: https://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c
 * Allows for something like: 
 * enum TFlags_{
 *    None = 0,
 *    Flag1 = 1 << 0;
 *    Flag2 = 1 << 1;
 *    etc...
 * };
 *
 * typedef Flags<TFlags_> TFlags; 
 * and then use from here ^
 */
namespace Core{
   template<typename E, typename U = std::underlying_type<E>::type>
   class Flags{
      public: 
         Flags()
            :  m_flags { 0 }
         { }

         Flags(E flag)
            : m_flags { flag }
         { }

         Flags(const Flags& original)
            : m_flags { original.m_flags }
         { }

         constexpr Flags& operator|=(E addValue) {
            m_flags |= addValue;
            return *this;
         }
         
         constexpr Flags operator|(E addValue) const{
            Flags ret { *this };
            return ret |= addValue;
         }

         constexpr Flags& operator&=(E maskValue){
            m_flags &= maskValue;
            return *this;
         }

         constexpr Flags& operator&=(const Flags& flag){
            m_flags &= flag.m_flags;
            return *this;
         }

         constexpr Flags operator&(E maskValue) const{
            Flags ret { *this };
            return ret &= maskValue;
         }

         constexpr Flags operator~() const{
            Flags ret { *this };
            ret.m_flags = ~ret.m_flags;
            return ret;
         }

         constexpr explicit operator bool() const{
            return m_flags != 0;
         }

      private: 
         U m_flags;
   };

   template<typename E>
   constexpr Flags<E> operator~(E flag){
      return ~Flags<E>(flag);
   }
   // allow for clearing a bit like:
   // flags &= ~FLAG1;
} // namespace Core
