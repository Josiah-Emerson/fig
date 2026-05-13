#include <catch2/catch_test_macros.hpp>
#include "Core_ECS/SortedComponentPool.h"
// TODO: ADD TESTS FOR FOLLOWING FUNCS: 
//    containsComperand()
//    separator()
//    Add tests to ensure that the pools stay synced for IDs 

struct testComperand{
   int outer;
   int inner;

   bool operator==(const testComperand& rhs) const{
      return outer == rhs.outer && inner == rhs.inner;
   }
};


struct Compare{
   bool operator()(const testComperand& a, const testComperand& b) const{
      // Outer first
      if(a.outer < b.outer){
         return true;
      }else if(a.outer > b.outer){
         return false;
      }
      
      // outers are same
      if(a.inner < b.inner)
         return true;

      return false;
   }
};

TEST_CASE("Ensure internal test struct and fucntion follow std::strict_weak_order requirements", "[internal]"){
   testComperand X { 0, 0 };
   testComperand a0 {0, 0};
   testComperand a1 {0, 1};
   testComperand a2 {0, 2};
   testComperand b0 {1, 0};
   testComperand c0 {2, 0};
   Compare compare;


   REQUIRE(!compare(X, X)); // irreflexive

   REQUIRE(compare(a0, b0)); 
   REQUIRE(compare(b0, c0));
   REQUIRE(compare(a0, c0)); // transitive outer

   REQUIRE(compare(a0, a1)); 
   REQUIRE(compare(a1, a2));
   REQUIRE(compare(a0, a2)); // transitive inner

   REQUIRE(compare(a0, a2));
   REQUIRE(compare(a2, c0));
   REQUIRE(compare(a0, c0)); // transitive acrosse
}

TEST_CASE("SortedComponentPools can be created and resized properly", "[SortedComponentPool]"){
   using namespace Core;
   constexpr std::size_t NUM { 10000 };
   SortedComponentPool<int, Compare, testComperand> lowCapacityPool{};
   SortedComponentPool<int, Compare, testComperand> largeCapacityPool{NUM};

   REQUIRE(lowCapacityPool.size() == 0);
   REQUIRE(largeCapacityPool.size() == 0);

   REQUIRE(lowCapacityPool.capacity() == 0);
   REQUIRE(largeCapacityPool.capacity() == NUM);

   lowCapacityPool.reserve(NUM);
   REQUIRE(lowCapacityPool.size() == 0);
   REQUIRE(lowCapacityPool.capacity() == NUM);
}

TEST_CASE("Insert function", "[SortedComponentPool]"){
   using namespace Core;
   constexpr std::size_t NUM { 100 };
   SortedComponentPool<EntityID, Compare, testComperand> pool{NUM}; // create a pool with NUM capacity

   EntityID currID = 0;

   testComperand aa {0, 0};
   testComperand ab {0, 1};
   testComperand ac {0, 2};
   testComperand ba {1, 0};
   testComperand bb {1, 1};
   testComperand bc {1, 2};
   testComperand ca {2, 0};
   testComperand cb {2, 1};
   testComperand cc {2, 2};
   testComperand da {3, 0};

   testComperand compArr[10] = {aa, ab, ac, ba, bb, bc, ca, cb, cc, da};

   SECTION("Insertion in order"){
      // insert 10 elements into the pool for each comperand 
      for(std::size_t i { 0 }; i < 10; ++i){
         for(int j { 0 }; j < 10; ++j){
            REQUIRE(pool.insert(currID, currID, compArr[i]));
            ++currID;
         }
      }

      REQUIRE(pool.size() == 100); // check size is correct

      for(std::size_t i { 0 }; i < currID; ++i){
         REQUIRE(pool[i] == static_cast<EntityID>(i)); // ensure elements 0-99 are in order 
      }
   }

   SECTION("Insertion in reverse order"){
      // insert elements 0-99 starting with a greater comperand 
      // should look like: 90-99 | 80-89 | 70-79 | ... | 0-9
      for(std::size_t i { 10 }; i > 0; --i){
         for(int j { 0 }; j < 10; ++j){
            REQUIRE(pool.insert(currID, currID, compArr[i-1]));
            ++currID;
         }
      }

      REQUIRE(pool.size() == 100);
      currID = 90;
      for(std::size_t i { 0 }; i < 100; ++i){
         REQUIRE(pool[i] == currID);
         ++currID;

         if((i + 1) % 10 == 0)
            currID-=20;
      }
   }

   SECTION("Insert in the middle somwhere"){
      // TODO: can make this a more robust and readable test case by moving all of these 
      // 10 insertions into their own thing and just switching up the order randomly
      
      // insert all comperands except for the Bs and ca first, then the Bs then Ca 
      // in such a way as to get 0-99 in order
      // As: 0-29
      // Bs: 30-59
      // Cs: 60-89
      // da: 90-99
      
      // insert As
      currID = 0;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[0]);
         ++currID;
      } 
      currID = 10;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[1]);
         ++currID;
      } 
      currID = 20;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[2]);
         ++currID;
      } 

      // insert cb and cc
      currID = 70;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[7]);
         ++currID;
      } 
      currID = 80;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[8]);
         ++currID;
      } 

      // insert da 
      currID = 90;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[9]);
         ++currID;
      } 

      REQUIRE(pool.size() == 60);

      // insert Bs and Ca
      currID = 30;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[3]);
         ++currID;
      } 
      currID = 40;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[4]);
         ++currID; } currID = 50;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[5]);
         ++currID;
      } 
      currID = 60;
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[6]);
         ++currID;
      } 

      REQUIRE(pool.size() == 100); // check size is correct

      for(std::size_t i { 0 }; i < currID; ++i){
         REQUIRE(pool[i] == static_cast<EntityID>(i)); // ensure elements 0-99 are in order 
      }

   }
}

TEST_CASE("Tests on a populated SortedComponentPool", "[SortedComponentPool]"){
   // pool will be populated with 100 elements 0-99 sorted from aa - da
   using namespace Core;
   constexpr std::size_t NUM { 100 };
   SortedComponentPool<EntityID, Compare, testComperand> pool{NUM}; // create a pool with NUM capacity
   EntityID currID = 0;

   testComperand aa {0, 0};
   testComperand ab {0, 1};
   testComperand ac {0, 2};
   testComperand ba {1, 0};
   testComperand bb {1, 1};
   testComperand bc {1, 2};
   testComperand ca {2, 0};
   testComperand cb {2, 1};
   testComperand cc {2, 2};
   testComperand da {3, 0};

   testComperand compArr[10] = {aa, ab, ac, ba, bb, bc, ca, cb, cc, da};
   for(std::size_t comp { 0 }; comp < 10; ++comp){
      for(int cnt { 0 }; cnt < 10; ++cnt){
         pool.insert(currID, currID, compArr[comp]);
         currID++;
      }
   }

   SECTION("contains"){
      REQUIRE(pool.contains(0));
      REQUIRE(pool.contains(47));
      REQUIRE(pool.contains(99));
      REQUIRE_FALSE(pool.contains(100));

      // check about an id not matching its value
      pool.insert(100, 800, da);
      REQUIRE(pool.contains(100));
      REQUIRE_FALSE(pool.contains(800));
   }

   SECTION("remove"){
      // remove beginning
      pool.remove(0);
      REQUIRE_FALSE(pool.contains(0));
      REQUIRE(pool.size() == 99);

      // remove middle
      pool.remove(47);
      REQUIRE_FALSE(pool.contains(47));
      REQUIRE(pool.size() == 98);

      // remove end
      pool.remove(99);
      REQUIRE_FALSE(pool.contains(99));
      REQUIRE(pool.size() == 97);

      // remove already removed
      pool.remove(99);
      REQUIRE_FALSE(pool.contains(99));
      REQUIRE(pool.size() == 97);

      // remove id never in pool
      REQUIRE_FALSE(pool.contains(1000));
      pool.remove(1000);
      REQUIRE_FALSE(pool.contains(1000));
      REQUIRE(pool.size() == 97);

      REQUIRE(pool.separatorList().size() == 10);
      // remove all of one type
      for(EntityID id { 0 }; id < 10; ++id){
         pool.remove(id);
      }
      REQUIRE(pool.separatorList().size() == 9);
   }

   SECTION("id"){
      // some random checks
      REQUIRE(pool.id(0) == 0);
      REQUIRE(pool.id(1) == 1);
      REQUIRE(pool.id(99) == 99);
      REQUIRE(pool.id(45) == 45);

      // access id of an invalid id
      REQUIRE_THROWS(pool.id(100));

      // insert a value thats not the same as its id
      pool.insert(100, 444, bb);
      REQUIRE(pool.id(100) == 444);

      // id still works if we update a component
      pool.updateComponent(34, 444);
      REQUIRE(pool.id(34) == 444);

      // id still works if we update a comperand
      pool.updateComperand(34, aa);
      REQUIRE(pool.id(34) == 444);

      // both in a row
      pool.updateComperand(3, ca);
      pool.updateComponent(3, 1200);
      REQUIRE(pool.id(3) == 1200);

      // reverse
      pool.updateComponent(64, 301);
      pool.updateComperand(64, ba);
      REQUIRE(pool.id(64) == 301);
   }

   SECTION("updateComponent"){
      // update beginning
      REQUIRE(pool.updateComponent(0, 444));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool.id(0) == 444);

      // update end
      REQUIRE(pool.updateComponent(99, 444));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool.id(99) == 444);

      // update middle
      REQUIRE(pool.updateComponent(30, 444));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool.id(30) == 444);

      // update invalid component
      REQUIRE_FALSE(pool.updateComponent(1000, 800));
      // ensure none were updated
      for(std::size_t i { 0 }; i < pool.size(); ++i){
         REQUIRE_FALSE(pool[i] == 800);
      }
   }

   SECTION("updateComperand"){
REQUIRE(pool.id(70) == 70); // TODO: Don't have the time right now but figure out if this was here for a reason or not?

      // update id 0 to have a comperand of da (i.e. back)
      REQUIRE(pool.updateComperand(0, da));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool[pool.size() - 11] == 0); // -11 because it should be sorted by EntityIDs now 

      // update id 99 to have a comperand aa (i.e. should go in back of front group or index 9 for now)
      REQUIRE(pool.updateComperand(99, aa));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool[9] == 99);

      // update id 40 to have its same comperand of bb (i.e. nothing happens)
      REQUIRE(pool.updateComperand(40, bb));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool[40] == 40);

      // update an ID which doesn't exist
      REQUIRE_FALSE(pool.updateComperand(100, aa));

      // update an id to have a comperand not yet seen at beginning
      REQUIRE(pool.updateComperand(50, {-1, -1}));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool[0] == 50);

      // update an id lower than 50 to have the same comperand (and it should jump before it)
      REQUIRE(pool.updateComperand(43, {-1, -1}));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool[0] == 43); 

      // update an id to have a comperand not yet seen at end
      REQUIRE(pool.updateComperand(70, {3, 3}));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool[99] == 70);

      // update an id lower than 70 to have the same comperand (and it should jump before it)
      REQUIRE(pool.updateComperand(67, {3, 3}));
      REQUIRE(pool.size() == 100);
      REQUIRE(pool[pool.size() - 2] == 67); // second to last should be 67 now

      // update to a new one in the middle (essentially bd)
      // should be at position 60
      REQUIRE(pool.updateComperand(75, {1, 3})); 
      REQUIRE(pool[60] == 75);
   }

   SECTION("separatorList"){
      std::size_t arrIndex = 0;
      const auto& list = pool.separatorList();
      for(const auto& e : list){
         auto u = e.first;
         auto sep = e.second;

         REQUIRE(u == compArr[arrIndex]);
         std::size_t low = arrIndex * 10;
         REQUIRE(sep.first == low);
         REQUIRE(sep.second == low+9);
         ++arrIndex;
      }

      REQUIRE(list.size() == 10);
   }


   SECTION("idFromIndex"){
      // random checks
      REQUIRE(pool.idFromIndex(0) == 0);
      REQUIRE(pool.idFromIndex(45) == 45);
      REQUIRE(pool.idFromIndex(99) == 99);

      // invalid access
      REQUIRE_THROWS(pool.idFromIndex(100));

      // insert a new element at the front
      pool.insert(100, 444, {-1, -1});
      REQUIRE(pool.idFromIndex(0) == 100);
      REQUIRE(pool.idFromIndex(1) == 0);
      REQUIRE(pool.idFromIndex(45) == 44);
      REQUIRE(pool.idFromIndex(100) == 99);

      // update first element to be have a comperand which places it at back
      pool.updateComperand(100, da);
      REQUIRE(pool.idFromIndex(0) == 0);
      REQUIRE(pool.idFromIndex(44) == 44);
      REQUIRE(pool.idFromIndex(100) == 100);
   }

}

