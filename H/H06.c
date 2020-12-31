/*

The length of the input string will be considered less than or equal to 256

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum { MAXLEN = 256 };

struct tree
{
    char c;
    int n;
    struct tree *left;
    struct tree *right;
    struct tree *parent;
};

void tree_add(struct tree **, char);
void tree_form(struct tree **, char);
void tree_delete(struct tree *);
void print_depth(struct tree **);


// Adding an element
void tree_add(struct tree **tr, char c) {
    (*tr) = (struct tree*)malloc(sizeof(struct tree));
    (*tr) -> left = NULL;
    (*tr) -> right = NULL;
    (*tr) -> parent = NULL;
    (*tr) -> c = c;
    (*tr) -> n = 1;
}

// Tree creation (recursively) with parents
void tree_form(struct tree **tr, char c) {
    if ((*tr) == NULL) {
        tree_add(tr, c);
    }

    if (((*tr) -> c == c) && ((*tr) -> parent != NULL)) {
        (*tr) -> n += 1;
    } else if (((unsigned char)((*tr) -> c) < (unsigned char)c) &&
                ((*tr) -> right == NULL)) {
        struct tree *new_tr = (struct tree*)malloc(sizeof(struct tree));
        (*tr) -> right = new_tr;
        new_tr -> left = NULL;
        new_tr -> right = NULL;
        new_tr -> parent = *tr;
        new_tr -> c = c;
        new_tr -> n = 1;
    } else if (((unsigned char)((*tr) -> c) > (unsigned char)c)
                && ((*tr) -> left == NULL)) {
        struct tree *new_tr = (struct tree*)malloc(sizeof(struct tree));
        (*tr) -> left = new_tr;
        new_tr -> left = NULL;
        new_tr -> right = NULL;
        new_tr -> parent = *tr;
        new_tr -> c = c;
        new_tr -> n = 1;
    }
    else if (((unsigned char)((*tr) -> c) > (unsigned char)c) &&
            ((*tr) -> left != NULL))
        tree_form(&((*tr) -> left), c);
    else if (((unsigned char)((*tr) -> c) < (unsigned char)c) &&
            ((*tr) -> right != NULL))
        tree_form(&((*tr) -> right), c);
}


// Tree deleting (non-recursively)
void tree_delete(struct tree *tr) {
    struct tree *node = tr;
    struct tree *parent;

    while(1) {
        while (!((node -> left == NULL) && (node -> right == NULL))) {
            if (node -> left != NULL) {
                node = node -> left;
            } else if (node -> right != NULL) {
                node = node -> right;
            }
        }

        parent = node -> parent;

        if (parent == NULL){
            free(node);
            break;
        }

        if ((parent -> right != NULL) && (parent -> right == node)) {
            parent -> right = NULL;
        } else if((parent -> left != NULL) && (parent -> left == node)) {
            parent -> left = NULL;
        }

        free(node);
        node = parent;
    }
}

// Print in depth
void print_depth(struct tree **tr) {
    if (tr != NULL) {
        if ((*tr) -> right != NULL) {
            print_depth(&((*tr) -> right));
        }
        printf("%c%c%d\n", (*tr) -> c, ' ', (*tr) -> n);
        if ((*tr) -> left != NULL) {
            print_depth(&((*tr)->left));
        }
    }
}

int main(void) {
    struct tree *my_tree = NULL;
    char str[MAXLEN];

    // String input
    fgets(str, MAXLEN, stdin);
    int len = strlen(str);
    if (str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }

    // Tree creating
    for(int i = 0; i < len; ++i) {
        tree_form(&my_tree, str[i]);
    }

    // Result
    printf("%s\n", str);
    print_depth(&my_tree);

    // Tree deleting
    tree_delete(my_tree);

    return 0;
}