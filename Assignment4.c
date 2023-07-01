#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// data structure tree
struct node {
  struct node *left_;
  int value_;
  struct node *right_;
};
typedef struct node node_t;

struct tree {
  node_t *root;
};
typedef struct tree tree_t;

// tree functions
void Init(tree_t *ptrTree) {
  ptrTree->root = NULL;
}

node_t *CreateNode(int value) {
  node_t *temp = (node_t *)malloc(sizeof(node_t));
  temp->value_ = value;
  temp->left_ = NULL;
  temp->right_ = NULL;
  return temp;
}

int Insert(tree_t *tree, int value) {
  int inserted;
  node_t *temp = CreateNode(value);
  if (tree->root == NULL) {
    tree->root = temp;
    inserted = value;
    return inserted;
  }

  node_t *travel = tree->root;
  node_t *pointer;
  while (travel) {
    pointer = travel;
    if (value <= travel->value_)
      travel = travel->left_;
    else
      travel = travel->right_;
  }
  if (value <= pointer->value_) {
    pointer->left_ = temp;
    inserted = value;
  } else {
    pointer->right_ = temp;
    inserted = value;
  }
  return inserted;
}

void PrintInsert(tree_t *tree, int value, FILE *fp) {
  Insert(tree, value);
  fprintf(fp, "%d inserted\n", value);
}

void Inorder(node_t *root, FILE *fp) {
  if (root) {
    Inorder(root->left_, fp);
    fprintf(fp, "%d ", root->value_);
    Inorder(root->right_, fp);
  }
}

void Preorder(node_t *root, FILE *fp) {
  if (root) {
    fprintf(fp, "%d ", root->value_);
    Preorder(root->left_, fp);
    Preorder(root->right_, fp);
  }
}

void Postorder(node_t *root, FILE *fp) {
  if (root) {
    Postorder(root->left_, fp);
    Postorder(root->right_, fp);
    fprintf(fp, "%d ", root->value_);
  }
}

int *Maximum(node_t *root) {
  node_t *travel = root;
  int *max = NULL;
  if (travel) {
    while (travel->right_) {
      travel = travel->right_;
    }
    max = (int *)malloc(sizeof(int));
    *max = travel->value_;
  }
  return max;
}

int *Minimum(node_t *root) {
  node_t *travel = root;
  int *min = NULL;

  if (travel) {
    while (travel->left_) {
      travel = travel->left_;
    }
    min = (int *)malloc(sizeof(int));
    *min = travel->value_;
  }
  return min;
}

void PrintMax(tree_t *tree, FILE *fp) {
  int *max = Maximum(tree->root);
  if (max)
    fprintf(fp, "%d", *max);
  fprintf(fp, "\n");
}

void PrintMin(tree_t *tree, FILE *fp) {
  int *min = Minimum(tree->root);
  if (min)
    fprintf(fp, "%d", *min);
  fprintf(fp, "\n");
}

int *Search(tree_t *tree, int value) {
  int *s = NULL;
  node_t *travel = tree->root;

  while (travel) {
    if (travel->value_ == value) {
      s = (int *)malloc(sizeof(int));
      *s = value;
      break;
    }
    if (value <= travel->value_) {
      travel = travel->left_;
    } else {
      travel = travel->right_;
    }
  }
  return s;
}

void PrintSearch(tree_t *tree, int value, FILE *fp) {
  int *s = Search(tree, value);
  if (s)
    fprintf(fp, "%d found\n", *s);
  else
    fprintf(fp, "%d not found\n", value);
}

void Successor(tree_t *tree, int key, FILE *fp) {
  node_t *travel = tree->root;
  if (!travel) {
    fprintf(fp, "Tree is empty\n");
    return;
  }
  node_t *present = NULL;
  node_t *successor = NULL;
  node_t *ancestor = tree->root;
  while (travel) {
    if (key < travel->value_) {
      travel = travel->left_;
    } else if (key > travel->value_) {
      travel = travel->right_;
    } else {
      present = travel;
      break;
    }
  }

  if (present == NULL) {
    fprintf(fp, "%d does not exist\n", key);
    return;
  }

  if (present->right_) {
    int *min = Minimum(present->right_);
    fprintf(fp, "%d\n", *min);
    return;
  } else {
    while (ancestor != present) {
      if (present->value_ < ancestor->value_) {
        successor = ancestor;
        ancestor = ancestor->left_;
      } else {
        ancestor = ancestor->right_;
      }
    }
  }

  if (successor == NULL) {
    fprintf(fp, "successor of %d does not exist\n", key);
  } else {
    fprintf(fp, "%d\n", successor->value_);
  }
}

void Predecessor(tree_t *tree, int key, FILE *fp) {
  node_t *travel = tree->root;
  if (!travel) {
    fprintf(fp, "Tree is empty\n");
    return;
  }
  node_t *present = NULL;
  node_t *predecessor = NULL;
  node_t *ancestor = tree->root;
  while (travel) {
    if (key < travel->value_) {
      travel = travel->left_;
    } else if (key > travel->value_) {
      travel = travel->right_;
    } else {
      present = travel;
      break;
    }
  }

  if (present == NULL) {
    fprintf(fp, "%d does not exist\n", key);
    return;
  }

  if (present->left_) {
    int *max = Maximum(present->left_);
    fprintf(fp, "%d\n", *max);
    return;
  } else {
    while (ancestor != present) {
      if (present->value_ < ancestor->value_) {
        ancestor = ancestor->left_;
      } else {
        predecessor = ancestor;
        ancestor = ancestor->right_;
      }
    }
  }

  if (predecessor == NULL) {
    fprintf(fp, "predecessor of %d does not exist\n", key);
  } else {
    fprintf(fp, "%d\n", predecessor->value_);
  }
}

int main(int argc, char *argv[]) {
  if (argc > 2) {
    printf("NOT VALID. ENTER ONLY ONE FILE NAME");
    exit(1);
  }

  if (argc < 2) {
    printf("NOT VALID. PLEASE ENTER THE FILE NAME");
    exit(1);
  }

  FILE *fp_r;
  FILE *fp_w;
  fp_r = fopen(argv[1], "r");

  if (!fp_r) {
    printf("FILE NOT FOUND");
    exit(2);
  }

  fp_w = fopen("output.txt", "w");

  int temp;
  char operation[100];
  tree_t *tree = malloc(sizeof(tree_t)); // Allocate memory for the tree
  Init(tree);
  int value;
  while (fscanf(fp_r, "%s %d\n", operation, &value) != EOF) {
    //printf("%s\n", operation);
    if (!strcmp(operation, "insert")) {
      PrintInsert(tree, value, fp_w);
    } else if (!strcmp(operation, "inorder")) {
      Inorder(tree->root, fp_w);
      fprintf(fp_w, "\n");
    } else if (!strcmp(operation, "preorder")) {
      Preorder(tree->root, fp_w);
      fprintf(fp_w, "\n");
    } else if (!strcmp(operation, "postorder")) {
      Postorder(tree->root, fp_w);
      fprintf(fp_w, "\n");
    } else if (!strcmp(operation, "minimum")) {
      PrintMin(tree, fp_w);
    } else if (!strcmp(operation, "maximum")) {
      PrintMax(tree, fp_w);
    } else if (!strcmp(operation, "successor")) {
      Successor(tree, value, fp_w);
    } else if (!strcmp(operation, "predecessor")) {
      Predecessor(tree, value, fp_w);
    } else if (!strcmp(operation, "search")) {
      PrintSearch(tree, value, fp_w);
    } else {
      printf("Invalid Operation\n");
    }
  }

  fclose(fp_r);
  fclose(fp_w);

  return 0;
}
