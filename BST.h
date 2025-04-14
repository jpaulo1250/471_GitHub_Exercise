// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.
// 05/15/2023 modification by Prof Sipantzi
// This file includes all of the pieces of the BST implementation

// Include the node implementation
#include "BSTNode.h"

// Include the dictionary ADT
#include "dictionary.h"

#pragma once

using namespace std;

// Binary Search Tree implementation for the Dictionary ADT
template <class Key, class E>
class BST : public Dictionary<Key,E> {
private:
  BSTNode<Key,E>* root;   // Root of the BST
  int nodecount;          // Number of nodes in the BST

  // Private "helper" functions
  void clearhelp(BSTNode<Key, E>*);
  BSTNode<Key,E>* inserthelp(BSTNode<Key, E>*, const Key&, const E&);
  BSTNode<Key,E>* deletemin(BSTNode<Key, E>*);
  BSTNode<Key,E>* getmin(BSTNode<Key, E>*);
  BSTNode<Key,E>* removehelp(BSTNode<Key, E>*, const Key&);
  E* findhelp(BSTNode<Key, E>*, const Key&) const;
  void printhelp(BSTNode<Key, E>*, int) const;
  void visit(BSTNode<Key, E>*) const;

  // New helper functions for pre-order and post-order
  void printPreOrder(BSTNode<Key, E>*) const;
  void printPostOrder(BSTNode<Key, E>*) const;

public:
  BST() { root = NULL; nodecount = 0; }  // Constructor

  // Destructor not used in this assignment
  //~BST() { clearhelp(root); }

  void clear() {
    clearhelp(root);
    root = NULL;
    nodecount = 0;
  }

  void insert(const Key& k, const E& e) {
    root = inserthelp(root, k, e);
    nodecount++;
  }

  E* remove(const Key& k) {
    E* temp = findhelp(root, k);
    if (temp != NULL) {
      root = removehelp(root, k);
      nodecount--;
    }
    return temp;
  }

  E* removeAny() {
    if (root != NULL) {
      E* temp = new E;
      *temp = root->element();
      root = removehelp(root, root->key());
      nodecount--;
      return temp;
    } else return NULL;
  }

  E* find(const Key& k) const {
    return findhelp(root, k);
  }

  int size() {
    return nodecount;
  }

  void print() const {
    if (root == NULL) {
      cout << "The BST is empty.\n";
      return;
    }

    cout << "\nIn-Order Traversal:\n";
    printhelp(root, 0);

    cout << "\nPre-Order Traversal:\n";
    printPreOrder(root);

    cout << "\nPost-Order Traversal:\n";
    printPostOrder(root);
  }
};

// Visit -- prints out root
template <typename Key, typename E>
void BST<Key, E>::visit(BSTNode<Key,E>* r) const {
  cout << "Node - " << r->key() << ' ' << r->element() << endl;
}

// Clean up BST
template <typename Key, typename E>
void BST<Key, E>::clearhelp(BSTNode<Key, E>* root) {
  if (root == NULL) return;
  clearhelp(root->left());
  clearhelp(root->right());
  delete root;
}

// Insert a node
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::inserthelp(
    BSTNode<Key, E>* root, const Key& k, const E& it) {
  if (root == NULL)
    return new BSTNode<Key, E>(k, it, NULL, NULL);
  if (k < root->key())
    root->setLeft(inserthelp(root->left(), k, it));
  else
    root->setRight(inserthelp(root->right(), k, it));
  return root;
}

// Get minimum
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::getmin(BSTNode<Key, E>* rt) {
  if (rt->left() == NULL) return rt;
  else return getmin(rt->left());
}

template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::deletemin(BSTNode<Key, E>* rt) {
  if (rt->left() == NULL)
    return rt->right();
  else {
    rt->setLeft(deletemin(rt->left()));
    return rt;
  }
}

// Remove node
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::removehelp(BSTNode<Key, E>* rt, const Key& k) {
  if (rt == NULL) return NULL;
  else if (k < rt->key())
    rt->setLeft(removehelp(rt->left(), k));
  else if (k > rt->key())
    rt->setRight(removehelp(rt->right(), k));
  else {
    BSTNode<Key, E>* temp = rt;
    if (rt->left() == NULL) {
      rt = rt->right();
      delete temp;
    } else if (rt->right() == NULL) {
      rt = rt->left();
      delete temp;
    } else {
      BSTNode<Key, E>* temp = getmin(rt->right());
      rt->setElement(temp->element());
      rt->setKey(temp->key());
      rt->setRight(deletemin(rt->right()));
      delete temp;
    }
  }
  return rt;
}

// Find a node
template <typename Key, typename E>
E* BST<Key, E>::findhelp(BSTNode<Key, E>* root, const Key& k) const {
  if (root == NULL) return NULL;
  if (k < root->key())
    return findhelp(root->left(), k);
  else if (k > root->key())
    return findhelp(root->right(), k);
  else {
    E* temp = new E;
    *temp = root->element();
    return temp;
  }
}

// In-order print
template <typename Key, typename E>
void BST<Key, E>::printhelp(BSTNode<Key, E>* root, int level) const {
  if (root == NULL) return;
  printhelp(root->left(), level + 1);
  visit(root);
  printhelp(root->right(), level + 1);
}

// Pre-order print
template <typename Key, typename E>
void BST<Key, E>::printPreOrder(BSTNode<Key, E>* root) const {
  if (root == NULL) return;
  visit(root);
  printPreOrder(root->left());
  printPreOrder(root->right());
}

// Post-order print
template <typename Key, typename E>
void BST<Key, E>::printPostOrder(BSTNode<Key, E>* root) const {
  if (root == NULL) return;
  printPostOrder(root->left());
  printPostOrder(root->right());
  visit(root);
}

void BST<Key, E>::
printPreOrder(BSTNode<Key, E>* root, int level) const {
  if (root == NULL) return;           // Empty tree
  visit(root);						  // Print node value
  printPreOrder(root->left(), level+1);   // Do left subtree
  printPreOrder(root->right(), level+1);  // Do right subtree
}

template <typename Key, typename E>
void BST <Key, E>::
printPostOrder(BSTNode<Key, E>* root, int level) const {
  if (root == NULL) return;           // Empty tree
  printPostOrder(root->left(), level+1);   // Do left subtree
  printPostOrder(root->right(), level+1);  // Do right subtree
  visit(root);						  // Print node value
}
