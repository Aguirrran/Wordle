#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>
#include <set>
#include <vector>

#include "bstset.h"

using namespace std;
using namespace testing;

namespace {

class Random {
 private:
  static mt19937 rng;

 public:
  static void seed(int s) {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max) {
    // Even though mt19937 is standardized, STL distributions aren't!
    // So, unfortunately, even though this is biased, we're forced to
    // do something like this. Technically uint32_fast_t isn't...
    // totally consistent across platforms? But within reason it works.
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;

TEST(BSTSetCore, isContructorEmpty) {
  BSTSet<int> BSTSetTest;
  EXPECT_THAT(BSTSetTest.empty(), true);
}
TEST(BSTSetCore, isSizeTheSame) {
  BSTSet<int> BSTSetTest;
  EXPECT_THAT(BSTSetTest.size(), 0);
  EXPECT_THAT(BSTSetTest.size(), BSTSetTest.size());
}
TEST(BSTSetCore, empty_true) {
  BSTSet<int> BSTSet;
  BSTSet.insert(20);
  EXPECT_THAT(BSTSet.empty(), false);
}
TEST(BSTSetCore, size_fixed) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(20);
  EXPECT_THAT(BSTSetTest.size(), 1);
}
TEST(BSTSetCore, contains_falseAndtrue) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(20);
  BSTSetTest.insert(30);
  EXPECT_THAT(BSTSetTest.contains(30), true);
  EXPECT_THAT(BSTSetTest.contains(40), false);
}
TEST(BSTSetCore, clear_no_reset_size) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(20);
  BSTSetTest.insert(30);
  BSTSetTest.clear();
  EXPECT_THAT(BSTSetTest.size(), 0);
}
TEST(BSTSetCore, copy_of_empty) {
  BSTSet<int> BSTSetTest;
  BSTSet<int> copyBSTSet2(BSTSetTest);
  EXPECT_THAT(copyBSTSet2.empty(), true);
}
TEST(BSTSetCore, assign_wrong_size) {
  BSTSet<int> BSTSetTest;
  BSTSet<int> copyBSTSet;
  BSTSetTest.insert(5);
  BSTSetTest.insert(10);
  BSTSetTest.insert(15);
  BSTSetTest.insert(20);
  copyBSTSet.insert(1);
  copyBSTSet.insert(2);
  BSTSetTest = copyBSTSet;
  EXPECT_FALSE(copyBSTSet.empty());
  EXPECT_THAT(copyBSTSet.size(), BSTSetTest.size());
  EXPECT_THAT(copyBSTSet.contains(0), BSTSetTest.contains(0));
}
TEST(BSTSetCore, isOperatorSelfAssignmentProtected) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(5);
  BSTSetTest.insert(10);
  BSTSet<int>& copyBSTSet = BSTSetTest;
  BSTSetTest = copyBSTSet;
  EXPECT_THAT(BSTSetTest.size(), 2);
  EXPECT_THAT(BSTSetTest.contains(0), false);
  EXPECT_THAT(BSTSetTest.contains(1), false);
}
TEST(BSTSetCore, assigmentEmptyCheck) {
  BSTSet<int> BSTSetTest;
  BSTSet<int> copyBSTSet;
  copyBSTSet.insert(1);
  copyBSTSet.insert(2);
  copyBSTSet = BSTSetTest;
  EXPECT_TRUE(copyBSTSet.empty());
  EXPECT_THAT(copyBSTSet.size(), 0);
}
TEST(BSTSetCore, assign_creates_empty) {
  BSTSet<int> BSTSetTest;
  BSTSet<int> copyBSTSet;
  copyBSTSet = BSTSetTest;
  EXPECT_TRUE(copyBSTSet.empty());
  EXPECT_THAT(copyBSTSet.size(), 0);
}
TEST(BSTSetCore, copy_always_empty) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(5);
  BSTSetTest.insert(10);
  BSTSetTest.insert(15);
  BSTSetTest.insert(20);
  BSTSet<int> copyBSTSet(BSTSetTest);
  EXPECT_THAT(copyBSTSet.size(), 4);
  EXPECT_THAT(copyBSTSet.contains(5), true);
  EXPECT_THAT(copyBSTSet.contains(10), true);
  EXPECT_THAT(copyBSTSet.contains(15), true);
  EXPECT_THAT(copyBSTSet.contains(20), true);
}
TEST(BSTSetCore, ssign_creates_empty) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(5);
  BSTSetTest.insert(10);
  BSTSetTest.insert(15);
  BSTSetTest.insert(20);
  BSTSet<int> copyBSTSet;
  copyBSTSet = BSTSetTest;
  EXPECT_THAT(copyBSTSet.size(), 4);
  EXPECT_THAT(copyBSTSet.contains(5), true);
  EXPECT_THAT(copyBSTSet.contains(10), true);
  EXPECT_THAT(copyBSTSet.contains(15), true);
  EXPECT_THAT(copyBSTSet.contains(20), true);
}
TEST(BSTSetAugmented, remove_min_empty) {
  BSTSet<int> BSTSetTest;
  EXPECT_THROW(BSTSetTest.remove_min(), runtime_error);
}
TEST(BSTSetAugmented, remove_min_no_size) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(5);
  BSTSetTest.insert(10);
  BSTSetTest.insert(15);
  BSTSetTest.insert(20);
  EXPECT_THAT(BSTSetTest.size(), 4);
  BSTSetTest.remove_min();
  EXPECT_THAT(BSTSetTest.size(), 3);
}
TEST(BSTSetAugmented, remove_wrong_return) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(5);
  BSTSetTest.insert(10);
  BSTSetTest.insert(15);
  BSTSetTest.insert(20);
  EXPECT_THAT(BSTSetTest.remove_min(), 5);
}
TEST(BSTSetAugmented, remove_min_loses_children) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(10);
  BSTSetTest.insert(5);
  BSTSetTest.insert(20);
  BSTSetTest.insert(10);
  BSTSetTest.insert(4);
  BSTSetTest.insert(22);
  BSTSetTest.insert(30);
  BSTSetTest.remove_min();
  EXPECT_THAT(BSTSetTest.remove_min(), 5);
}
TEST(BSTSetAugmented, begin_next_empty) {
  BSTSet<int> BSTSetTest;
  int value;
  BSTSetTest.begin();
  EXPECT_FALSE(BSTSetTest.next(value));
}
TEST(BSTSetAugmented, begin_next_wrong_start) {
  BSTSet<int> BSTSetTest;
  int value;
  BSTSetTest.insert(10);
  BSTSetTest.insert(5);
  BSTSetTest.insert(15);
  BSTSetTest.insert(4);
  BSTSetTest.insert(9);
  BSTSetTest.begin();
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 4);
}
TEST(BSTSetAugmented, next_revisit_nodes) {
  BSTSet<int> BSTSetTest;
  int value;
  BSTSetTest.insert(5);
  BSTSetTest.insert(2);
  BSTSetTest.insert(15);
  BSTSetTest.begin();
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 2);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 5);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 15);
  EXPECT_FALSE(BSTSetTest.next(value));
}
TEST(BSTSetAugmented, next_skip_nodes) {
  BSTSet<int> BSTSetTest;
  int value;
  BSTSetTest.insert(5);
  BSTSetTest.insert(10);
  BSTSetTest.insert(15);
  BSTSetTest.insert(13);
  BSTSetTest.insert(20);
  BSTSetTest.insert(11);
  BSTSetTest.insert(39);
  BSTSetTest.insert(21);
  BSTSetTest.insert(27);
  BSTSetTest.begin();
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 5);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 10);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 11);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 13);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 15);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 20);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 21);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 27);
  ASSERT_TRUE(BSTSetTest.next(value));
  EXPECT_THAT(value, 39);
  EXPECT_FALSE(BSTSetTest.next(value));
}
TEST(BSTSetErase, erase_no_throw) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(5);
  BSTSetTest.insert(10);
  BSTSetTest.insert(15);
  EXPECT_THROW(BSTSetTest.erase(20), out_of_range);
}
TEST(BSTSetErase, erase_loses_descendants) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(5);
  BSTSetTest.insert(10);
  BSTSetTest.insert(15);
  BSTSetTest.erase(5);
  EXPECT_THAT(BSTSetTest.size(), 2);
  EXPECT_THAT(BSTSetTest.contains(10), true);
  EXPECT_THAT(BSTSetTest.contains(15), true);
  EXPECT_THAT(BSTSetTest.contains(5), false);
}
TEST(BSTSetErase, erase_root_no_children) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(5);
  BSTSetTest.erase(5);
  EXPECT_THAT(BSTSetTest.size(), 0);
  EXPECT_THAT(BSTSetTest.contains(5), false);
  EXPECT_THROW(BSTSetTest.erase(5), out_of_range);
}
TEST(BSTSetErase, erase_successor_with_children) {
  BSTSet<int> BSTSetTest;
  BSTSetTest.insert(10);
  BSTSetTest.insert(5);
  BSTSetTest.insert(20);
  BSTSetTest.insert(15);
  BSTSetTest.insert(17);
  BSTSetTest.erase(10);
  EXPECT_THAT(BSTSetTest.size(), 4);
  EXPECT_THAT(BSTSetTest.contains(10), false);
  EXPECT_THAT(BSTSetTest.contains(5), true);
  EXPECT_THAT(BSTSetTest.contains(20), true);
  EXPECT_THAT(BSTSetTest.contains(15), true);
  EXPECT_THAT(BSTSetTest.contains(17), true);
}

}  // namespace
