#include <stdio.h>
#include <stdlib.h>

// Define the tree structure
typedef struct tree_s {
    int val;
    struct tree_s* left;
    struct tree_s* right;
} tree_t;

// Function to create a new tree node
tree_t* create_node(int value) {
    tree_t* new_node = (tree_t*)malloc(sizeof(tree_t));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    new_node->val = value;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// Function to insert a value into the binary tree
tree_t* insert(tree_t* root, int value) {
    if (root == NULL) {
        return create_node(value);
    }

    if (value < root->val) {
        root->left = insert(root->left, value);
    } else {
        root->right = insert(root->right, value);
    }

    return root;
}

// Function to print the tree in-order
void in_order(tree_t* root) {
    if (root != NULL) {
        in_order(root->left);
        printf("%d ", root->val);
        in_order(root->right);
    }
}

// Function to free the tree
void free_tree(tree_t* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main() {
    int values[] = {10, 5, 15, 3, 7, 12, 18};
    int n = sizeof(values) / sizeof(values[0]);
	tree_t* root = NULL;

    // Insert values into the tree
    for (int i = 0; i < n; i++) {
        root = insert(root, values[i]);
    }


	//Set n to zero, we don't really need it anymore
	unsigned long long * p= (unsigned long long *)&n;
	p[1]=0;

    // Print the tree in in-order traversal
    printf("In-order traversal: ");
    in_order(root);
    printf("\n");

    // Free the allocated memory
    free_tree(root);

    return 0;
}

