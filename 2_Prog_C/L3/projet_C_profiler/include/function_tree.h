#ifndef __FUNCTION_TREE__
#define __FUNCTION_TREE__

/**
 * @file
 * @brief Provides an interface to create a tree of calls from a file.
 */

#include <stdio.h>

/**
 * @brief Longest function name + 1
 */
#define MAX_FUNC_NAME_LENGTH	80

/**
 * @brief A structure describing a node in a tree of function calls.
 *
 * The tree of itself is a "left child, right sibling" tree, in which the lower-level nodes store the more deeply nested calls.
 * Sibling nodes store functions which where called in sequence. They can be iterated as a regular linked list.
 */
typedef struct node {
	char   funct_name[MAX_FUNC_NAME_LENGTH];	/*!< The name of the function */
	double start;	/*!< Absolute starting time of the call */
	double end;	/*!< Absolute ending time of the call */
	double funct_duration;	/*!< Total duration of the call */
	struct node *Left_son;	/*!< Pointer to a node of nested calls. */
	struct node *Right_bro;	/*!< Pointer to a node of sequential calls. */
} Node, *Tree;


/**
 * @brief Allocates a new node whose function has the given properties.
 * @param name The name of the function
 * @param start Absolute starting time of the call.
 * @return A pointer to the newly allocated node, or NULL upon error.
 */
Node* allocate_node(const char name[], double start);

/**
 * @brief deallocates a whole tree and its content.
 * @param T a pointer to a tree.
 */
void erase_Tree(Tree *T);

/**
 * @brief Returns the number of siblings a node has in linear time.
 * @param root A pointer to a node.
 * @return The number of brothers the node has, or 0 if it has none.
 */
int get_brothers_count(Tree root);



#endif
