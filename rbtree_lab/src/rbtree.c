#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *t = malloc(sizeof(rbtree));
  if (t == NULL) return NULL;

  t->nil = malloc(sizeof(node_t));
  if (t->nil == NULL) {
    free(t);
    return NULL;
  }
  t->root = t->nil;
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
  if (new_node == NULL) return NULL;

  new_node->key = key;
  new_node->color = color;
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->parent = t->nil;

  return new_node;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  if (t == NULL) return NULL;

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

  // 새로운 노드 생성 (RED로 생성)
  node_t *new_node = make_node(t, key, RBTREE_RED);
  if (new_node == NULL) return NULL;

  // 부모에 연결
  new_node->parent = parent;

  if (parent == t->nil) {
    t->root = new_node;
  } else if (key < parent->key) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }

  // 레드블랙 규칙 복원
  rbtree_insert_fixup(t, new_node);

  return new_node;
}

// x를 기준으로 좌회전하는 함수
void left_rotate(rbtree *t, node_t *x) {
  if (t == NULL || x == NULL) return;

  node_t * y = x->right; // y는 x의 오른쪽 자식

  // 1. x의 오른쪽 자식을 y의 왼쪽 자식으로 변경
  x->right = y->left;
  
  if (y->left != t->nil) {
    y->left->parent = x;
  }

  // 2. y의 부모를 x의 부모로 설정
  y->parent = x->parent;

  // 3. x가 루트였는지 여부에 따라 루트 또는 부모의 자식 포인터 갱신
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  }

  // 4. y의 왼쪽 자식으로 x 연결, x의 부모를 y로 설정
  y->left = x;
  x->parent = y;
}

// x를 기준으로 우회전하는 함수
void right_rotate(rbtree *t, node_t *x) {
  if (t == NULL || x == NULL) return;

  node_t *y = x->left; // y는 x의 왼쪽 자식

  // 1. x의 왼쪽 자식을 y의 오른쪽 자식으로 변경
  x->left = y->right;

  if (y->right != t->nil) {
    y->right->parent = x;
  }

  // 2. y의 부모를 x의 부모로 설정
  y->parent = x->parent;

  // 3. x가 루트였는지 여부에 따라 루트 또는 부모의 자식 포인터 갱신
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  }

  // 4. y의 오른쪽 자식으로 x 연결, x의 부모를 y로 설정
  y->right = x;
  x->parent = y;
}

// 삽입 후 레드블랙트리 규칙 복원
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED) {
    node_t *gp = z->parent->parent;

    // 부모가 할아버지의 왼쪽 자식인 경우
    if (z->parent == gp->left) {
      node_t *uncle = gp->right;

      if (uncle->color == RBTREE_RED) {
        // Case 1: 삼촌도 RED -> 색상 변경만 하고 z를 위로 올림
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        gp->color = RBTREE_RED;
        z = gp;
      } else {
        // Case 2: z가 오른쪽 자식이면 -> 왼쪽 회전해서 Case 3 모양으로 바꿈
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        }
        // Case 3: z가 왼쪽 자식이면 -> 색상 변경 후 오른쪽 회전
        z->parent->color = RBTREE_BLACK;
        gp->color = RBTREE_RED;
        right_rotate(t, gp);
      }

    // 부모가 할아버지의 오른쪽 자식인 경우 (대칭)
    } else {
      node_t *uncle = gp->left;

      if (uncle->color == RBTREE_RED) {
        // Case 1 (대칭)
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        gp->color = RBTREE_RED;
        z = gp;
      } else {
        // Case 2 (대칭): z가 왼쪽 자식이면 -> 오른쪽 회전
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        // Case 3 (대칭): 색상 바꾸고 왼쪽 회전
        z->parent->color = RBTREE_BLACK;
        gp->color = RBTREE_RED;
        left_rotate(t, gp);
      }
    }
  }
  // 루트는 항상 BLACK이어야 함
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur = t->root;
  
  while (cur != t->nil) {
    if (key == cur->key) {
      return cur;
    } else if (key < cur->key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }

  return NULL;
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
