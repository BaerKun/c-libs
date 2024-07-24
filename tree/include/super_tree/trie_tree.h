#ifndef TRIE_TREE_H
#define TRIE_TREE_H

#include "binary_tree.h"

typedef BinaryTreeNode TrieTreeNode, *TrieTreeNodePtr;

typedef BinaryTree TrieTree, *TrieTreePtr;

#define NO_DATA (-1)
TrieTreePtr HuffmanCode(DataType *data, const int *cost, int number);

#endif //TRIE_TREE_H
