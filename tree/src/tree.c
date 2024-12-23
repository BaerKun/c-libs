#include <tree.h>
#include <stdlib.h>

inline TreeNodePtr treeNewNode(const DataType data) {
    const TreeNodePtr node = malloc(sizeof(TreeNode));
    node->next = NULL;
#ifdef  TREE_BINARY
    node->left = node->right = NULL;
#else
    node->firstChild = NULL;
#endif
    node->data = data;
    return node;
}

inline void treeInsertNode(TreeNodePtr *const node2child, const TreeNodePtr node) {
    node->next = *node2child;
    *node2child = node;
}

inline void treeInsertData(TreeNodePtr *const node2child, const DataType data) {
    const TreeNodePtr node = malloc(sizeof(TreeNode));
    node->data = data;
#ifdef  TREE_BINARY
    node->left = node->right = NULL;
#else
    node->firstChild = NULL;
#endif
    treeInsertNode(node2child, node);
}

inline TreeNodePtr treeUnlink(TreeNodePtr *const node2child) {
    const TreeNodePtr node = *node2child;
    *node2child = node->next;
    node->next = NULL;
    return node;
}

void treeDestroy(TreeNodePtr root) {
    while (root) {
#ifdef  TREE_BINARY
        treeDestroy(root->left);
        treeDestroy(root->right);
#else
        treeDestroy(root->firstChild);
#endif
        const TreeNodePtr node = root;
        root = root->next;
        free(node);
    }
}
