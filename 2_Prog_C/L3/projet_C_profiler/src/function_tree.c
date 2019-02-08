#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <function_tree.h>

#include <string.h>


/* #define PROFILER_ENABLED */
#include <macro_profiler.h>


Node* allocate_node(const char name[], double start_time) {
	PROFILE

	Node* new_node = (Node *) malloc(sizeof(Node));
	if(new_node == NULL) {
		fprintf(stderr, "Error malloc -> allocate_node().\n");
		exit(EXIT_FAILURE);
	}
	strcpy(new_node->funct_name, name);
	new_node->start = start_time;
	new_node->end = 0;
	new_node->funct_duration = 0;
	new_node->Left_son = NULL, new_node->Right_bro = NULL;
	return new_node;
}


void erase_Tree(Tree *T) {
	PROFILE
	if(*T == NULL)
		return;

	erase_Tree(&((*T)->Left_son));
	erase_Tree(&((*T)->Right_bro));
	free(*T);
	*T = NULL;

	return;
}


int get_brothers_count(Tree root)
{
	PROFILE
	if(root == NULL)
	{
		return 0;
	}
	else
	{
		int n = get_brothers_count(root->Right_bro);
		return n + 1;
	}
}
