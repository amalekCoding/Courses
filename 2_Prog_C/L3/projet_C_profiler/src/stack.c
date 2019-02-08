#include <assert.h>
#include <stdlib.h>

#include <stack.h>

/* #define PROFILER_ENABLED */
#include <macro_profiler.h>

Token *new_token(Tree function, int x, int y, int w, int h)
{
	PROFILE
	Token *t = malloc(sizeof(Token));
	if(t != NULL)
	{
		t->next = NULL;
		t->function = function;
		t->x = x;
		t->y = y;
		t->w = w;
		t->h = h;
	}
	return t;
}

void push_token(Token **stack, Token *t)
{
	PROFILE
	if(*stack == NULL)
		*stack = t;
	else
	{
		t->next = *stack;
		*stack = t;
	}
	return ;
}


Token *pop_token(Token **stack)
{
	PROFILE
	if(*stack != NULL)
	{
		Token *top = *stack;
		*stack = (*stack)->next;
		return top;
	}
	/* empty stack */
	return NULL;
}

void free_stack(Stack *stack)
{
	PROFILE
	assert(stack != NULL);

	if(*stack != NULL)
	{
		free_stack(&((*stack)->next));
		free(*stack);
	}
	return ;
}

Token *which_token(Token *stack, int x, int y)
{
	PROFILE
	if(stack != NULL)
	{
		if(x >= stack->x && y >= stack->y && x <= (stack->x + stack->w) && y <= (stack->y + stack->h))
		{
			return stack;
		}
		else
		{
			Token *res = which_token(stack->next, x, y);
			return res;
		}
	}
	return NULL;
}
