#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
// make clean
//   make test_wordle
// make run_server
//
//
using namespace std;

template <typename T>
class BSTSet {
 private:
  struct BSTNode {
    // These two members should never change after initialization!
    const T value;
    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;

    // This is fancy constructor syntax
    // Don't worry too much about it -- it's just necesssary for the const
    // member.
    BSTNode(T value, BSTNode* parent)
        : value(value), parent(parent), left(nullptr), right(nullptr) {
    }
  };

  BSTNode* root;
  size_t sz;

  // Utility pointer for begin and next.
  BSTNode* curr;
  // TODO_STUDENT: add private helper function definitions here

  void insert_helper(BSTNode*& curr, BSTNode* parent, T value) {
    if (curr == nullptr) {
      curr = new BSTNode(value, parent);
      sz++;
      return;
    } else if (value > curr->value) {
      insert_helper(curr->right, curr, value);
    } else if (value < curr->value) {
      insert_helper(curr->left, curr, value);
    } else {
      if (curr->value == value) {
        return;
      }
    }
  }
  bool contains_helper(BSTNode* curr, T value) const {
    if (curr == nullptr) {
      return false;
    }
    if (curr->value == value) {
      return true;
    }
    if (value > curr->value) {
      return contains_helper(curr->right, value);
    } else {
      return contains_helper(curr->left, value);
    }
  }
  void destructor_helper(BSTNode*& curr) {
    if (curr == nullptr) {
      return;
    }
    destructor_helper(curr->left);
    destructor_helper(curr->right);
    delete curr;
    curr = nullptr;
  }
  void to_string_helper(BSTNode* curr, ostringstream& output,
                        bool& first) const {
    if (curr == nullptr) {
      return;
    }
    to_string_helper(curr->left, output, first);
    if (!first) {
      output << ", ";
    }
    output << curr->value;
    first = false;
    to_string_helper(curr->right, output, first);
  }
  BSTNode* copy_Costructor_helper(BSTNode* copyNode, BSTNode* parent) {
    if (copyNode == nullptr) {
      return nullptr;
    }

    BSTNode* newNode = new BSTNode(copyNode->value, parent);
    newNode->left = copy_Costructor_helper(copyNode->left, newNode);
    newNode->right = copy_Costructor_helper(copyNode->right, newNode);

    return newNode;
  }
  T remove_min_helper(BSTNode*& curr) {
    if (curr == nullptr) {
      throw(runtime_error("There is no Nodes in the tree!!!"));
    }
    if (curr->left == nullptr) {
      T value = curr->value;
      BSTNode* prevousNode = curr;
      curr = curr->right;
      if (curr != nullptr) {
        curr->parent = prevousNode->parent;
      }
      delete prevousNode;
      sz--;
      return value;
    }
    return remove_min_helper(curr->left);
  }

  T erase_helper(BSTNode*& curr, T targetValue) {
    if (curr == nullptr) {
      throw(out_of_range("There is no node with this value!"));
    }
    if (targetValue < curr->value) {
      return (erase_helper(curr->left, targetValue));
    }
    if (targetValue > curr->value) {
      return (erase_helper(curr->right, targetValue));
    }
    T erasedValue = curr->value;
    if (curr->left == nullptr && curr->right == nullptr) {
      delete curr;
      curr = nullptr;
      sz--;
      return erasedValue;
    }
    if (curr->left == nullptr || curr->right == nullptr) {
      BSTNode* child;
      if (curr->left != nullptr) {
        child = curr->left;
      } else {
        child = curr->right;
      }
      child->parent = curr->parent;
      BSTNode* tempNode = curr;
      curr = child;
      delete tempNode;
      sz--;
      return erasedValue;
    }
    BSTNode* oldNode = curr;
    BSTNode* newNode = curr->right;
    while (newNode->left != nullptr) {
      newNode = newNode->left;
    }
    if (newNode->parent != oldNode) {
      newNode->parent->left = newNode->right;
      if (newNode->right != nullptr) {
        newNode->right->parent = newNode->parent;
      }

      newNode->right = oldNode->right;
      newNode->right->parent = newNode;
    }

    newNode->left = oldNode->left;
    newNode->left->parent = newNode;
    newNode->parent = oldNode->parent;

    curr = newNode;

    delete oldNode;
    sz--;
    return erasedValue;
  }

 public:
  /**
   * Creates an empty `BSTSet`. Runs in O(1).
   */
  BSTSet() : root(nullptr), sz(0), curr(nullptr) {
    // Completed
  }

  /**
   * Checks if the `BSTSet` is empty. Runs in O(1).
   */
  bool empty() const {
    return sz == 0;
    // Completed
  }

  /**
   * Returns the number of elements in the `BSTSet`. Runs in O(1).
   */
  size_t size() const {
    return sz;
    // TODO_STUDENT
  }

  /**
   * Adds the `value` to the `BSTSet`. If the value already
   * exists in the set, does not do anything.
   *
   * Uses the `value` to determine the location in the underlying BST.
   * Creates exactly one new node.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  void insert(T value) {
    insert_helper(root, nullptr, value);
    // Completed
  }

  /**
   * Returns `true` if the `value` is present in the set,
   * and `false` otherwise.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  bool contains(const T& value) const {
    return contains_helper(root, value);
  }

  /**
   * Empties the `BSTSet`, freeing all memory it controls.
   *
   * Runs in O(N), where N is the number of elements in the set.
   */
  void clear() {
    destructor_helper(root);
    sz = 0;
    curr = nullptr;
    root = nullptr;
    // TODO_STUDENT
  }

  /**
   * Destructor, cleans up the `BSTSet`.
   *
   * Runs in O(N), where N is the number of elements in the set.
   */
  ~BSTSet() {
    clear();
    // TODO_STUDENT
  }

  /**
   * Converts the `BSTSet` to a string representation, with the values
   * in-order by value.
   *
   * Example:
   *
   * c++
   * BSTSet<string> names;
   * names.insert("Gwen");
   * names.insert("Jen");
   * names.insert("Ben");
   * names.insert("Sven");
   *
   * Calling `names.to_string()` would return the following
   * string: `{Ben, Gwen, Jen, Sven}`
   *
   * Runs in O(N), where N is the number of elements.
   */
  string to_string() const {
    ostringstream output;
    output << "{";
    bool first = true;
    to_string_helper(root, output, first);
    output << "}";
    return output.str();
  }

  /**
   * Copy constructor.
   *
   * Copies the elements from the provided `BSTSet`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N), where N is the number of elements in `other`.
   */
  BSTSet(const BSTSet& other) : root(nullptr), sz(0), curr(nullptr) {
    root = copy_Costructor_helper(other.root, nullptr);
    sz = other.sz;
    // TODO_STUDENTc
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this tree, and copies the elements from the provided `BSTSet`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N + O), where N is the number of elements in `this`, and O is
   * the number of elements in `other`.
   */
  BSTSet& operator=(const BSTSet& other) {
    if (this == &other) {
      return *this;
    }
    clear();
    root = copy_Costructor_helper(other.root, nullptr);
    sz = other.sz;
    curr = nullptr;
    return *this;
  }

  // =========================================================================

  /**
   * Locates the smallest element in the BSTSet, and removes it
   * from the `BSTSet`. If the `BSTSet` is empty, throws a
   * `runtime_error`.
   *
   * Creates no new nodes, and does not update any
   * existing nodes.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  T remove_min() {
    // TODO_STUDENT
    return remove_min_helper(root);
  }

  /**
   * Resets internal state for an iterative inorder traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr`.
   *
   * O(H), where H is the maximum height of the tree.
   */
  void begin() {
    curr = root;
    while (curr != nullptr && curr->left != nullptr) {
      curr = curr->left;
    }
    // TODO_STUDENT
  }

  /**
   * Uses the internal state to return the next in-order element
   * by reference, and advances the internal state. Returns `true`
   * if the reference was set, and `false` otherwise.
   *
   * Example usage:
   *
   * c++
   * BSTSet<string> bst;
   * bst.begin();
   * string value;
   * while (bst.next(val)) {
   *   cout << val << endl;
   * }
   * ```
   *
   * Modifies nothing except for `curr`.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  bool next(T& value) {
    // TODO_STUDENT
    if (curr == nullptr) {
      return false;
    }
    value = curr->value;
    if (curr->right != nullptr) {
      curr = curr->right;
      while (curr->left != nullptr) {
        curr = curr->left;
      }
    } else {
      BSTNode* childNode = curr;
      curr = curr->parent;
      while (curr != nullptr && childNode == curr->right) {
        childNode = curr;
        curr = curr->parent;
      }
    }
    return true;
  }

  // =========================================================================

  /**
   * Removes the given element from the `BSTSet`, and returns the
   * value.
   *
   * Throws `out_of_range` if the value is not present in the set.
   * Creates no new nodes, and does not update the value of any
   * existing nodes.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  T erase(const T& value) {
    // TODO_STUDENT
    return erase_helper(root, value);
  }

  // =========================================================================

  /**
   * Returns a pointer to the root node of the `BSTSet`. For autograder testing
   * purposes only.
   */
  void* getRoot() const {
    return this->root;
  }
};
