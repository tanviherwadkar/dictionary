/***
 * Tanvi Herwadkar
 * therwadk
 * 2023 Fall CSE101 PA7
 * Dictionary.cpp
 * Implements Dictionary ADT
***/ 

#include<iostream>
#include<string>
#include "Dictionary.h"

using namespace std;
#define NIL "\0"

// Node Constructors --------------------------------------------------------
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

// Helper Functions --------------------------------------------------------
void Dictionary::inOrderString(std::string& s, Node* R) const {
    if (R != nil) {
        inOrderString(s, R->left);
        s.append(R->key);
        s += " : ";
        s.append(std::to_string(R->val));
        s += "\n";
        inOrderString(s, R->right);
    }
}

void Dictionary::preOrderString(std::string& s, Node* R) const {
    if (R != nil) {
        s.append(R->key);
        s += "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

void Dictionary::preOrderCopy(Node* R, Node* N) {
    if (R != N) {
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

void Dictionary::postOrderDelete(Node* R) {
    if (R != nil) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        R->parent = nullptr;
        R->left = nullptr;
        R->right = nullptr;
        delete R;
        num_pairs--;
    }
}

Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    if (R == nil) {
        return R;
    }
    else if (k == R->key) {
        return R;
    }
    else if (k < R->key) {
        return search(R->left, k);
    }
    else { // if k > R->key
        return search(R->right, k);
    }
}

//pre: R!=nil
Dictionary::Node* Dictionary::findMin(Node* R) {
    if (R == nil) {
        throw logic_error("Dictionary: findMin(): root cannot equal nil");
    }
    while (R->left != nil) {
        R = R->left;
    }
    return R;
}

Dictionary::Node* Dictionary::findMax(Node* R) {
    if (R == nil) {
        throw logic_error("Dictionary: findMax(): root cannot equal nil");
    }
    while (R->right != nil) {
        R = R->right;
    }
    return R;
}

Dictionary::Node* Dictionary::findNext(Node* N) {
    if (N->right != nil) {
        return findMin(N->right);
    }
    Node* Y = N->parent;
    while (Y != nil && N == Y->right) {
        N = Y;
        Y = Y->parent;
    }
    return Y;
}

Dictionary::Node* Dictionary::findPrev(Node* N) {
    if (N->left != nil) {
        return findMax(N->left);
    }
    Node* Y = N->parent;
    while (Y != nil && N == Y->left) {
        N = Y;
        Y = Y->parent;
    }
    return Y;
}

//Dictionary Constructors and Destructors --------------------------------------------------------
Dictionary::Dictionary() {
    nil = new Node(NIL, -1);
    root = nil;
    current = nil;
    num_pairs  = 0;
}

Dictionary::Dictionary(const Dictionary& D) {
    nil = new Node(NIL, -1);
    root = nil;
    current = nil;
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary(){
    clear();
    delete nil;
    nil = nullptr;
    current = nullptr;
    root = nullptr;
}

// Access functions --------------------------------------------------------
int Dictionary::size() const {
    return this->num_pairs;
}

bool Dictionary::contains(keyType k) const {
    if (search(this->root, k) == nil) {
        return false;
    }
    else {
        return true;
    }
}

valType& Dictionary::getValue(keyType k) const {
    Node* N = search(this->root, k);
    if (N == nil) {
        throw logic_error("Dictionary: getValue(): key does not exist in dict");
    }
    else {
        return N->val;
    }
}

bool Dictionary::hasCurrent() const {
    if (this->current == nil) {
        return false;
    }
    else if (this->current == nullptr) {
        return false;
    }
    else {
        return true;
    }
}

keyType Dictionary::currentKey() const {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: currentKey(): current undef");
    }
    else {
        return current->key;
    }
}

valType& Dictionary::currentVal() const {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: currentKey(): current undef");
    }
    else {
        return current->val;
    }
}

// Manipulation procedures -------------------------------------------------
void Dictionary::clear() {
    postOrderDelete(root);
    root = nil;
    current = nil;
    num_pairs = 0;
}

void Dictionary::setValue(keyType k, valType v) {
    Node* Z = new Node(k, v);
    Z->left = nil;
    Z->right = nil;
    //TreeInsertAlg
    Node* Y = nil;
    Node* X = root;
    while (X != nil) {
        Y = X;
        if (Z->key < X->key) {
            X = X->left;
        }
        else if (Z->key == X->key) {
            X->val = v;
            delete Z;
            return;
        }
        else {
            X = X->right;
        }
    }
    Z->parent = Y;
    if (Y == nil) {
        num_pairs += 1;
        root = Z;
    }
    else if (Z->key < Y->key) {
        num_pairs += 1;
        Y->left = Z;
    }
    else if (Z->key > Y->key) {
        num_pairs += 1;
        Y->right = Z;
    }
}

void Dictionary::transplant(Dictionary* T, Dictionary::Node* U, Dictionary::Node* V) {
    if (U->parent == T->nil) {
        T->root = V;
    }
    else if (U == U->parent->left) {
        U->parent->left = V;
    }
    else {
        U->parent->right = V;
    }
    if (V != T->nil) {
        V->parent = U->parent;
    }
}

void Dictionary::remove(keyType k) {
    Node* Z = search(root, k);
    if (Z == nil) {
        throw logic_error("Dictionary: remove(): k DNE in dict");
    }
    
    if (Z == current) {
        current = nil;
    }
    if (Z->left == nil) {
        transplant(this, Z, Z->right);
        delete Z;
    }
    else if (Z->right == nil) {
        transplant(this, Z, Z->left);
        delete Z;
    }
    else {
        Node* Y = findMin(Z->right);
        if (Y->parent != Z) {
            transplant(this, Y, Y->right);
            Y->right = Z->right;
            Y->right->parent = Y;
        }
        transplant(this, Z, Y);
        Y->left = Z->left;
        Y->left->parent = Y;
        delete Z;
    }
    num_pairs -= 1;
}

void Dictionary::begin() {
    if (num_pairs > 0) {
        current = findMin(root);
    }
}

void Dictionary::end() {
    if (num_pairs > 0) {
        current = findMax(root);
    }
}

void Dictionary::next() {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: next(): cursor undef");
    }
    Node* F = findNext(current);
    if (F == nil) {
        current = nil;
    }
    else { //if node is found
        current = F;
    }
}

void Dictionary::prev() {
    if (!hasCurrent()) {
        throw logic_error("Dictionary: next(): cursor undef");
    }
    Node* F = findPrev(current);
    if (F == nil) {
        current = nil;
    }
    else { //if node is found
        current = F;
    }
}

// Other Functions ---------------------------------------------------------
std::string Dictionary::to_string() const {
    string s = "";
    inOrderString(s, this->root);
    return s;
}

std::string Dictionary::pre_string() const {
    string s = "";
    preOrderString(s, this->root);
    return s;
}

bool Dictionary::equals(const Dictionary& D) const {
    if (num_pairs != D.num_pairs) {
        return false;
    }
    string t = to_string();
    string d = D.to_string();
    if (t == d) {
        return true;
    }
    else {
        return false;
    }
}

// Overloaded Operators ----------------------------------------------------
std::ostream& operator<<( std::ostream& stream, Dictionary& D ) {
    return stream << D.Dictionary::to_string();
}

bool operator==( const Dictionary& A, const Dictionary& B ) {
    return A.Dictionary::equals(B);
}

Dictionary& Dictionary::operator=( const Dictionary& D ) {
    if (this != &D) {
        Dictionary temp = D;
        swap(nil, temp.nil);
        swap(root, temp.root);
        swap(current, temp.current);
        swap(num_pairs, temp.num_pairs);
    }
    return *this;
}