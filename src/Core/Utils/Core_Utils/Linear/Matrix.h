#pragma once
#include "Core_Utils/Concepts.h"
#include "Core_Utils/Linear/Vector.h"
#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <iostream>

/*
 * Provides useful types and associated functions with Matrices
 * TODO: Might be interesting to explore a design where we store it as an array of vectors,
 * which would help with things like returning a row as a vector without having to construct one,
 * and possibly transposing, however it brings up challenges when we need a pointer to the data. For example,
 * opengl requires a simple pointer which we could just contstruct, but how do we ensure that the pointer stays valid?
 *
 * TODO: Add support for an r-value version of of constructing using vectors. For example we could create something like:
 * Matrix<float, 3, 3> { {0, 1, 2}, {3, 4, 5} , {6, 7, 8}};
 * Instead of having to construct vectors first
 * NOTE: After trying with initializer_list<const Vector<T, N>>& I found that it cannot store references,
 * and I also think it might not be able to do r-value? At least not in a 'move' type of way. Def need to look into it more
 *
 * TODO: Add more flexibility in creation of matricies (and possibly vectors too for that matter)
 * For example: Vecs of different types create a common matrix of some other type
 * For example: Matrix(fmat2, 1.f) creates a mat3 that looks like:
 *    x x 0  = fmat2 | 0
 *    x x 0  =   0   | 1.f
 *    0 0 1  =
 */

namespace Core{
   namespace Linear{

      // Creates an M x N matrix of type T where M is is the N is the number of rows and N is the number of columns, and T is numeric
      template<typename T, std::size_t M, std::size_t N>
      requires(Concepts::numeric<T>)
      class Matrix{
         public: 
            // creates an MxN matrix of type T with all elements initialized to 0
            Matrix(); 

            // creates an MxN matrix of type T with all elements initialized to val
            Matrix(const T val);

            // Creates an MxN matrix of type T from M (row) vectors of length N
            Matrix(std::initializer_list<Vector<T, N>> vals);

            // TODO: Probably not the best way to do this but works (?) for now
            // Creates an MxN matrix of type T from M (row) vectors of length N
            template<typename... Args>
            requires(sizeof...(Args) == M &&
                     (std::is_same_v<Args, const Vector<T, N>&> && ...))
            Matrix(Args... args);

            // TODO: Big 5 if needed. For now default
            Matrix(const Matrix& m) = default;
            Matrix(Matrix&& m) = default;
            Matrix& operator=(const Matrix& m) = default;
            Matrix& operator=(Matrix&& m) = default;
            ~Matrix() = default;

            // operators
            // TODO: For now these are fine. Three options: 
            // A) Keep public and add at function for accessing rows and columns
            // B) Move to private for use internally and add at function for accessing rows and columns
            // C) Compeletely remove in favor of at function. NOTE: Right now a couple external functions (namely operator<<) use [] and will need to be replaced
            T& operator[](const std::size_t i);
            const T& operator[](const std::size_t i) const;


            void fill(const T val);
         private:
            T m_data[M * N];
      };

      /*
       *
       * FREE METHOD DECLARATIONS
       *
       */
      template<typename T, std::size_t M, std::size_t N>
      requires(Concepts::numeric<T>)
      std::ostream& operator<<(std::ostream& out, const Matrix<T, M, N>& m);

      /*
       *
       * MATRIX CLASS IMPLEMENTATION
       *
       */

      /*
       *
       * PUBLIC METHODS
       *
       */


      /*
       *
       * CONSTRUCTORS, DESTRUCTORS and ASSIGNMENT
       *
       */

      template<typename T, std::size_t M, std::size_t N>
      requires(Concepts::numeric<T>)
      Matrix<T, M, N>::Matrix()
         : Matrix(0)
      { }

      template<typename T, std::size_t M, std::size_t N>
      requires(Concepts::numeric<T>)
      Matrix<T, M, N>::Matrix(const T val){
         fill(val);
      }

      template<typename T, std::size_t M, std::size_t N>
      requires(Concepts::numeric<T>)
      Matrix<T, M, N>::Matrix(std::initializer_list<Vector<T, N>> list){
         assert(list.size() == M && "Amount of Vectors supplied does not match the amount of rows needed");
         constexpr std::size_t ROW_SIZE = N * sizeof(T); 

         std::cout << "ROW_SIZE: " << ROW_SIZE << '\n';
         std::cout << "sizeof Vector: " << sizeof(Vector<T, N>) << '\n';
         std::size_t cnt { 0 };
         for(const Vector<T, N>& row : list){
            std::cout << "Addres: " << m_data + (cnt * N) << '\n';
            std::memcpy(m_data + (cnt * N), &row.data()[0], ROW_SIZE);
            ++cnt;
         }
      }

      /*
       *
       * OPERATORS
       *
       */

      template<typename T, std::size_t M, std::size_t N>
      requires(Concepts::numeric<T>)
      T& Matrix<T, M, N>::operator[](const std::size_t i){
         // TODO: Throw exception?
         assert(i <= M*N && "Index is out of bounds");
         return m_data[i];
      }

      template<typename T, std::size_t M, std::size_t N>
      requires(Concepts::numeric<T>)
      const T& Matrix<T, M, N>::operator[](const std::size_t i) const{
         // TODO: Throw exception?
         assert(i <= M*N && "Index is out of bounds");
         return m_data[i];
      }


      /*
       *
       * MEMBER METHODS
       * 
       */

      template<typename T, std::size_t M, std::size_t N>
      requires(Concepts::numeric<T>)
      void Matrix<T, M, N>::fill(const T val){
         for(std::size_t i { 0 }; i < M * N; ++i){
            m_data[i] = val;
         }
      }

      /*
       *
       * FREE METHOD IMPLEMENTATIONS
       *
       */

      template<typename T, std::size_t M, std::size_t N>
      requires(Concepts::numeric<T>)
      std::ostream& operator<<(std::ostream& out, const Matrix<T, M, N>& m){
         // TODO: If replace [] with at function, update this 
         out << '\n';
         for(std::size_t row { 0 }; row < M; ++row){
            out << "| ";
            for(std::size_t col { 0 }; col < N; ++col){
               out << m[(row * M) + col] << ' ';
            }
            out << "|\n";
         }

         out << '\n';
         return out;
      }


   } // namespace Linear
} // namespace Core
