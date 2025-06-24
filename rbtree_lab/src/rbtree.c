#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *t = malloc(sizeof(rbtree));
  if (t == NULL) return;

  t->root = t->nil;

  t->nil = malloc(sizeof(node_t));
  t->nil->color = RBTREE_BLACK;
  t->nil->left = t->nil;
  t->nil->right = t->nil;
  t->nil->parent = t->nil;

  return t;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *make_node(rbtree *t, const key_t key, color_t color) {
  node_t *new_node = malloc(sizeof(node_t));
  if (new_node == NULL) return;

  new_node->key = key;
  new_node->color = color;
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->parent = t->nil;

  return new_node;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  if (t == NULL) return;

  node_t *parent = t->nil;
  node_t *cur = t->root;

  // RB Tree의 삽입 위치 탐색
  while (cur != t->nil) {
    parent = cur;
    if (key < cur->key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }

  // 노드 생성
  node_t *new_node = make_node(t, key, RBTREE_RED);
  if (new_node == NULL) return;

  // 부모에 연결
  new_node->parent = parent;

  if (parent == t->nil) {
    t->root = new_node;
  } else if (key < parent->key) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }

  return new_node;
}

// x를 기준으로 좌회전하는 함수
void left_rotate(rbtree *t, node_t *x) {
  if (t == NULL || x == NULL) return;

  node_t *y = x->right; // y는 x의 오른쪽 자식

  x->right = y->left; // y의 왼쪽 자식을 x의 오른쪽 자식으로 옮김

  if (y->left != t->nil) {
    y->left->parent = x; // y의 왼쪽 자식이 존재하면, 그 부모를 x로 설정
  }

  y->parent = x->parent; // y의 부모를 x의 부모로 설정

  // x가 루트였다면 y를 새로운 루트로 설정
  if (x == t->root) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y; // x가 부모의 왼쪽 자식이면, y를 왼쪽에 연결
  } else {
    x->parent->right = y; // x가 부모의 오른쪽 자식이면, y를 오른쪽에 연결
  }

  y->left = x; // x를 y의 왼쪽 자식으로 설정
  x->parent = y; // x의 부모를 y로 설정
}

// x를 기준으로 우회전하는 함수
right_rotate(rbtree *t, node_t *x) {

}

// 삽입 후 규칙 복원 (z는 삽입된 노드)
rbtree_insert_fixup(t, z) {

}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
