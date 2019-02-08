#ifndef _H_STACK_
#define _H_STACK_

/**
 * @file
 * @brief Provides an interface to manipulate a stack of functions and areas.
 */

#include <function_tree.h>

/** 
 * @brief A linked list of Tree roots and their graphical area.
 *
 * This recursive structure works as a stack. Each token holds a pointer to the root of a tree and four integers describing a rectangle.
 * Simply put, this structure associates an area of the screen to a function in the tree of calls. Using which_token(), one can query
 * the stack for the function whose area was clicked in. 
 */
typedef struct token
{
	struct token *next;	/*!< The next token of the stack, or NULL if there is none. */
	Tree function;	/*!< A pointer to a function node in a tree of calls. */
	int x;	/*!< The abscissa of the right-left corner of the area */
	int y;	/*!< The ordinate of the right-left corner of the area */
	int w;	/*!< The width of the area */
	int h;	/*!< The height of the area */
}Token, *Stack;

/** 
 * @brief Allocates a new token.
 * @param function a function in a tree of calls.
 * @param x The abscissa of the right-left corner of the area
 * @param y The ordinate of the right-left corner of the area
 * @param w The width of the area
 * @param h The height of the area
 * @return A pointer to the newly allocated token, or NULL upon error.
 */
Token *new_token(Tree function, int x, int y, int w, int h);

/**
 * @brief Pushes a token to the top of a stack. The stack's content are modified.
 * @param stack A pointer to a stack of tokens
 * @param The token to place at the top of the stack.
 */
void push_token(Token **stack, Token *t);

/**
 * @brief Pop a token from a given stack and return it, or NULL if the stack is empty. The stack's content are modified.
 * @param stack A pointer to a stack of tokens
 * @return The token at the top of the stack.
 */
Token *pop_token(Token **stack);

/** 
 * @brief Returns the first token in which the x and y coordinates belong, or NULL if none do. Does not modify the stack.
 *
 * This function checks every token T of the stack. If T.x <= x <= T.x + T.w and T.x <= x <= T.x + T.w, then the point is within the
 * bounds of the token's rectangle, and thus T is returned. Since the stack can only be iterated one way, priority is given to the first token that
 * satisfies this condition. If no token do or if the stack is empty, NULL is returned.
 * @param stack A stack of tokens
 * @param x The abscissa of a point
 * @param y The ordinate of a point
 * @return The first token in which the x and y coordinates belong, or NULL if none do.
 */
Token *which_token(Token *stack, int x, int y);

/** 
 * @brief Deallocates a whole stack of tokens.
 * @param stack A pointer to a stack of tokens
 */
void free_stack(Token **stack);

#endif