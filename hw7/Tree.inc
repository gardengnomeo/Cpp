#include <string>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <ostream>


template<typename T>
std::string Tree<T>::toString() {

    // traverse root
    std::ostringstream os;
    os << data << '\n';
    std::string result(os.str());

    // traverse children
    if (kids != nullptr) {
        result += kids->toString();
    }

    // traverse siblings
    if (sibs != nullptr) {
        result += sibs->toString();
    }

    return result;

}


//constructs the head pointer
//initialize head node data to char and other info to nullptr
template <typename T>
Tree<T>::Tree(T data)
    : data(data), kids (nullptr), sibs(nullptr), prev(nullptr) {}



//duplicates a tree using recursion
template<typename T>
Tree<T>* createDuplicateTree(const Tree<T> *t){

    //check if the node referenced exists as using recursion so need base case
    if(t == nullptr){
        return nullptr;
    }

    //create a copy of the data at origional tree
    Tree<T>* copy = new Tree<T>(t->data);

    //copy the kids and sibs by using recursion
    if (t->kids != nullptr) {
        copy->kids = createDuplicateTree(t->kids);
        copy->kids->prev = copy;
    }
    if (t->sibs != nullptr) {
        copy->sibs = createDuplicateTree(t->sibs);
        copy->sibs->prev = copy;
    }

    return copy;
}


//copy constructor
template<typename T>
Tree<T>::Tree(const Tree<T> &tree){

    //create a temp copy of the input tree so that a deep copy can be made
    Tree<T>* temp = createDuplicateTree(&tree);

    //copy the nodes from the temp tree to this tree
    this->data = temp->data;
    this->sibs = temp->sibs;
    this->kids = temp->kids;
    this->prev = temp->prev;

    //delete the dynamically allocated memory in temp
    temp->kids = nullptr;
    temp->sibs = nullptr;
    delete temp;
}


//destructor
template<typename T>
Tree<T>::~Tree(){
    /*
    //recursively delete all children and siblings
    if (kids != nullptr) {
        delete kids;
        kids = nullptr;
    }
    if (sibs != nullptr) {
        delete sibs;
        sibs = nullptr;
    }
    */
    delete kids;
    delete sibs;
}


//creates a child node and send to the overloaded addChild funciton to add
template<typename T>
bool Tree<T>::addChild(T data){

    //create new child node
    Tree* newChild = new Tree<T>(data);

    //send the node to the overloaded addChild and check if succeded
    bool success = addChild(newChild);

    //if failed delete the node
    if (!success) {
        delete newChild;
    }

    return success;
}


//adds a child to the node and inserts it in the right location
template<typename T>
bool Tree<T>::addChild(Tree *t){

    //check to ensure t is root node
    if(t == nullptr || t->prev != nullptr || t->sibs != nullptr){
        return false;
    }

    //variables to see where to insert node
    Tree* current = kids;
    Tree* prev = this;

    //loop through the children of the parent node to see where to insert
    while(current != nullptr && current->data <= t->data){
        //check if there is already a child with same char
        if(current->data == t->data){
            return false;
        }
        prev = current;
        current = current->sibs;
    }

    //insert the new child if first child or other
    if(prev == this){
        t->sibs = kids;
        kids = t;
    } else {
        prev->sibs = t;
        t->sibs = current;
    }

    //set prev and current to relink the chain correctly
    t->prev = prev;
    if(current != nullptr){
        current->prev = t;
    }

    return true;
}


//creates a sibling node and send to the overloaded addSibling funciton 
template<typename T>
bool Tree<T>::addSibling(T data){

    //create new sibling node
    Tree* newSibling = new Tree<T>(data);

    //send the node to the overloaded addsibling and check if succeded
    bool success = addSibling(newSibling);

    //if failed delete the node
    if (!success) {
        delete newSibling;
    }

    return success;
}


//adds a sibling to the right of the node and inserts it in the right location
template<typename T>
bool Tree<T>::addSibling(Tree *t){

    //check to ensure t is root node, also checks that not adding sibling
    //to a root node and that t should be placed to the right
    if(t == nullptr || t->prev != nullptr || t->sibs != nullptr ||
    this->prev == nullptr || this->data > t->data){
        return false;
    }

    //variables to see where to insert node
    Tree* current = this->sibs;
    Tree* prev = this;

    //loop through the children of the parent node to see where to insert
    while(current != nullptr && current->data < t->data){
        //check if there is already a child with same char
        if(current->data == t->data){
            return false;
        }
        prev = current;
        current = current->sibs;
    }

    //insert the new sibling
    t->sibs = current;
    t->prev = prev;
    prev->sibs = t;

    if(current != nullptr){
        current->prev = t;
    }

    return true; 
}


//checks to see if two trees are the same using recursion to go through the nodes
template<typename T>
bool treeCompare(const Tree<T> *a, const Tree<T> *b){

    //check if both nodes are nullptr
    if(a == nullptr && b == nullptr){
        return true;
    }

    //return false if only one is nullptr (wont both be as check above)
    if(a == nullptr || b == nullptr){
        return false;
    }

    //return false if data different
    if(a->data != b->data){
        return false;
    }

    //recursively go through the nodes to check
    return treeCompare(a->sibs, b->sibs) && treeCompare(a->kids, b->kids);
}


//copies the ths into the current pointer using recurision
template<typename T>
Tree<T>& Tree<T>::operator=(const Tree<T> &rhs){
    
    //ensure not copying something to itself
    if(this == &rhs){
        return *this;
    }

    //delete existing tree
    if(kids != nullptr) {
        delete kids;
        kids = nullptr;
    }
    if(sibs != nullptr) {
        delete sibs;
        sibs = nullptr;
    }

    //assign root node to be the same
    this->data = rhs.data;

    //copy the kids:
    if(rhs.kids){
        this->kids = createDuplicateTree(rhs.kids);
    } else {
        this->kids = nullptr;
    }

    //copy the sibs
    if(rhs.sibs){
        this->sibs = createDuplicateTree(rhs.sibs);
    } else {
        this->sibs = nullptr;
    }

    return *this;
}


//adds a tree to a node as a child calls addChild
template<typename T>
Tree<T>& Tree<T>::operator+=(Tree<T> &rt){

    //call addChild to add rt as a child
    this->addChild(&rt);

    //return the tree 
    return *this;
}


//Overload the == operator and use tree compare to compare trees
template<typename T>
bool Tree<T>::operator==(const Tree<T> &rhs){

    //check to see if they are the same using treeCompare
    return treeCompare(this,&rhs);
}


//output for if a tree is getting outputted
template<typename T>
std::ostream& operator<<(std::ostream& os, Tree<T>& rt){

    //call to string so that it is able to print a string
    os << rt.toString();
    return os;
}
