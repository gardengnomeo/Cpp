#ifndef TREE_H
#define TREE_H

#include <cstdlib>
#include <string>

template <typename T>
class Tree;

template <typename T>
std::ostream& operator<<(std::ostream& os, Tree<T>& rt);

template <typename T>
class Tree {
    friend class TTreeTest;
    friend std::ostream& operator<<<T>(std::ostream& os, Tree<T>& rt);

public:

    // constructor
    Tree(T data);

    // copy constructor
    Tree(const Tree &tree);

    // destructor
    ~Tree();

    // assignment operator
    Tree& operator=(const Tree &rhs);

    // operator to do the same thing as addChild
    Tree& operator+=(Tree &rt);

    // return true if two CTrees match node by node
    bool operator==(const Tree &rhs);

    // siblings and children must be unique
    // return true if added, false otherwise
    bool addChild(T data);

    // add tree root for better building, root should have null prev and sibs
    // returns false on any type of failure, including invalid root
    bool addChild(Tree<T> *t);

    // all characters in preorder separated by newlines, including at the end
    std::string toString();

private:

    // the value stored in the tree node
    T data;

    // children - pointer to first child of list, maintain order & uniqueness
    Tree * kids;

    // siblings - pointer to rest of children list, maintain order & uniqueness
    // should always be null if the object is the root of a tree
    Tree * sibs;

    // pointer to parent if this is a first child, or left sibling otherwise
    // should always be null if the object is the root of a tree
    Tree * prev;

    // these should only be called from addChild, and have the same restrictions
    // the root of a tree should never have any siblings
    // returns false on any type of failure, including invalid root
    bool addSibling(T data);
    bool addSibling(Tree<T> *t);

    // returns true if trees a and b are identical, false otherwise
    // (helper method to implement == operator)


    template <typename U>
    friend bool treeCompare(const Tree<U> *a, const Tree<U> *b);

    // returns pointer to a new tree that is a duplicate of *t
    // caller is responsible for updating result's root prev field (copy's
    // inner prevs will be set)
    // (helper method to implement copy constructor and assignment operator)

    template <typename U>
    friend Tree<U>* createDuplicateTree(const Tree<U> *t);

};


#include "Tree.inc"
#endif
