#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct node {
  int key;
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef node *tree;

void insert(tree *t, int k);
void removeValue(tree *t, int k);
int isBST1(tree t);
int isBST(tree t);
int isSummationTree(tree t);
void queryRange(tree t, int a, int b);
tree lowestCommonAncestor(tree t, int x, int y);

int main(int argc, char *argv[]) { return EXIT_SUCCESS; }

void insertHelper(tree *t, int k, tree parent) {
  if (*t == NULL) {
    *t = (tree)malloc(sizeof(node));
    (*t)->key = k;
    (*t)->left = NULL;
    (*t)->right = NULL;
    (*t)->parent = parent;
    return;
  }

  if (k < (*t)->key)
    return insertHelper(&(*t)->left, k, *t);
  else if (k > (*t)->key)
    return insertHelper(&(*t)->right, k, *t);
}

void insert(tree *t, int k) { return insertHelper(t, k, NULL); }

tree removeNoChildren(tree t, int k) {
  free(t);
  return NULL;
}

tree removeOneChild(tree t, int k) {
  tree newSubRoot = t->right;
  if (newSubRoot == NULL)
    newSubRoot = t->left;
  newSubRoot->parent = t->parent;
  free(t);
  return newSubRoot;
}

tree minNode(tree t) {
  if (t->left == NULL)
    return t;
  return minNode(t->left);
}

tree successor(tree t) { return minNode(t->right); }

void unlink(tree parent, tree child) {
  if (parent->left == child) {
    parent->left = NULL;
  } else if (parent->right == child) {
    parent->right = NULL;
  }
  child->parent = NULL;
}

void linkLeft(tree parent, tree child) {
  parent->left = child;
  child->parent = parent;
}

void linkRight(tree parent, tree child) {
  parent->right = child;
  child->parent = parent;
}

tree removeTwoChildren(tree t, int k) {
  tree succ = successor(t);
  unlink(succ->parent, succ);
  succ->parent = t->parent;
  linkLeft(succ, t->left);
  linkRight(succ, t->right);
  free(t);
  return succ;
}

tree removeValueHelper(tree t, int k) {
  if (t->key == k) {
    if (t->left == NULL && t->right == NULL) {
      return removeNoChildren(t, k);
    }
    if (t->left != NULL && t->right != NULL) {
      return removeTwoChildren(t, k);
    }
    return removeTwoChildren(t, k);
  }

  if (k < t->key) {
    t->left = removeValueHelper(t->left, k);
    return t;
  } else {
    t->right = removeValueHelper(t->right, k);
    return t;
  }
}

void removeValue(tree *t, int k) { *t = removeValueHelper(*t, k); }

int isBSTHelper(tree t, int min, int max) {
  if (t == NULL)
    return 1;
  if (t->key <= min || t->key >= max)
    return 0;
  return isBSTHelper(t->left, min, t->key) &&
         isBSTHelper(t->right, t->key, max);
}

int isBST(tree t) {
  return t == NULL || isBSTHelper(t->left, INT_MIN, t->key) &&
                          isBSTHelper(t->right, t->key, INT_MAX);
}

typedef struct summationInfo {
  int sum;
  int isSummationTree;
} summationInfo;

summationInfo isSummationTreeHelper(tree t) {
  if (t == NULL) {
    summationInfo res;
    res.isSummationTree = 1;
    res.sum = 0;
    return res;
  }
  summationInfo leftSum = isSummationTreeHelper(t->left);
  summationInfo rightSum = isSummationTreeHelper(t->right);
  summationInfo res;
  res.sum = leftSum.sum + rightSum.sum;
  int shouldCheck = t->left != NULL || t->right != NULL;
  res.isSummationTree = leftSum.isSummationTree && rightSum.isSummationTree &&
                        (!shouldCheck || res.sum == t->key);
  res.sum += t->key;
  return res;
}

int isSummationTree(tree t) { return isSummationTreeHelper(t).isSummationTree; }

typedef struct lcaInfo {
  tree lca;
  int foundLeft;
  int foundRight;
} lcaInfo;

lcaInfo lcaHelper(tree t, int x, int y) {
  if (t == NULL) {
    lcaInfo res;
    res.lca = NULL;
    res.foundLeft = 0;
    res.foundRight = 0;
    return res;
  }
  lcaInfo leftInfo = lcaHelper(t->left, x, y);
  if (leftInfo.lca != NULL)
    return leftInfo;
  lcaInfo rightInfo = lcaHelper(t->right, x, y);
  if (rightInfo.lca != NULL)
    return rightInfo;
  lcaInfo res;
  res.foundLeft = leftInfo.foundLeft || rightInfo.foundLeft || t->key == x;
  res.foundRight = leftInfo.foundRight || rightInfo.foundRight || t->key == y;
  if (res.foundLeft && res.foundRight) {
    res.lca = t;
  }
  return res;
}

tree lowestCommonAncestor(tree t, int x, int y) {
  return lcaHelper(t, x, y).lca;
}

void queryRange(tree t, int a, int b) {
  if (t == NULL || t->key < a || t->key > b)
    return;

  queryRange(t->left, a, b);
  printf("%d ", t->key);
  queryRange(t->right, a, b);
}
