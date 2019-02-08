#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <function_table.h>

/* #define PROFILER_ENABLED */
#include <macro_profiler.h>



FTable *table_init(void)
{
	PROFILE
	FTable *t = malloc(sizeof(FTable));
	if(t != NULL)
	{
		if(NULL == (t->functions = malloc(DEFAULT_TABLE_SIZE * sizeof(FTable_entry))))
		{
			free(t);
		}
		else
		{
			t->size = 0;
			t->capacity = DEFAULT_TABLE_SIZE;
		}
	}
	return t;
}

void table_free(FTable *table)
{
	PROFILE
	assert(table != NULL);
	assert(table->functions != NULL);

	free(table->functions);
	free(table);
	return ;
}

int table_add(FTable *table, const char *name, double duration)
{
	PROFILE
	int i;

	assert(table != NULL);
	assert(table->functions != NULL);
	assert(name != NULL);
	assert(duration >= 0);

	/* Is the function already in the table? */
	for(i = 0; i < table->size; ++i)
	{
		if(!strcmp(table->functions[i].funct_name, name))
		{
			/* Match! Simply increase its duration and number of calls. */
			strncpy(table->functions[i].funct_name, name, MAX_FUNC_NAME_LENGTH);
			table->functions[i].total_duration += duration;
			table->functions[i].calls_count += 1;

			return 0;
		}
	}

	/* A new entry must be created. If the table has spare room, use it. Otherwise, reallocate the whole table with an increased capacity. */
	if(table->size >= table->capacity)
	{
		/* Reallocate the table. */
		FTable_entry *new_location;

		if(NULL == (new_location = realloc(table->functions, table->capacity * 2 * sizeof(FTable_entry))))
		{
			/* Reallocation failed, fall back to the old memory location, but notify the caller that the operation could not be carried out. */
			return 1;
		}
		else
		{
			table->functions = new_location;
			table->capacity *= 2;
		}
	}

	/* Create the new entry. */
	strncpy(table->functions[table->size].funct_name, name, MAX_FUNC_NAME_LENGTH);
	table->functions[table->size].total_duration = duration;
	table->functions[table->size].calls_count = 1;
	table->size++;

	return 0;
}

FTable_entry *table_get(FTable *table, const char *name)
{
	PROFILE
	int i;

	assert(table != NULL);
	assert(table->functions != NULL);
	assert(name != NULL);

	for(i = 0; i < table->size; ++i)
	{
		if(!strcmp(table->functions[i].funct_name, name))
		{
			return &(table->functions[i]);
		}
	}
	return NULL;
}


void construct_tree_logFile(Tree* T, FILE* log_file, FTable *table) {
	PROFILE
	char name[MAX_FUNC_NAME_LENGTH];
	double end_time;

	assert(NULL != T);
	assert(NULL != log_file);
	assert(NULL != table);

	while(fscanf(log_file, "%s -- time : %lfs\n", name, &end_time) == 2) {

		if(strcmp(name, "END") == 0) {
			(*T)->end = end_time;
			(*T)->funct_duration = (*T)->end - (*T)->start;

			if(table_add(table, (*T)->funct_name, (*T)->funct_duration))
			{
				/* Error */
				fprintf(stderr, "Tree allocation error\n");
				exit(EXIT_FAILURE);
			}
			break;
		}

		else {
			Tree new_tree = allocate_node(name, end_time);

			if(new_tree == NULL)
			{
				/* Error */
				fprintf(stderr, "Tree allocation error\n");
				exit(EXIT_FAILURE);
			}

			if(NULL != *T) {
				if(NULL == (*T)->Left_son) {
					(*T)->Left_son = new_tree;
					construct_tree_logFile(&((*T)->Left_son), log_file, table);
				}
				else {
					Node* tmp = (*T)->Left_son;
					while(NULL != tmp->Right_bro)	/* Add it to the rightmost brother */
						tmp = tmp->Right_bro;
					tmp->Right_bro = new_tree;
					construct_tree_logFile(&new_tree, log_file, table);
				}
			}
			else
			{
				*T = new_tree;
			}

		}
	}
	return ;
}

#undef PROFILER_ENABLED

int __sort_by_name_ascending(const void *_a, const void *_b)
{
	PROFILE
	FTable_entry *a = (FTable_entry *)_a;
	FTable_entry *b = (FTable_entry *)_b;

	int res = strcmp(a->funct_name, b->funct_name);
	return res;
}

int __sort_by_duration_ascending(const void *_a, const void *_b)
{
	PROFILE
	FTable_entry *a = (FTable_entry *)_a;
	FTable_entry *b = (FTable_entry *)_b;

	if(a->total_duration == b->total_duration)
	{
		return 0;
	}
	return (a->total_duration < b->total_duration)? -1 : 1;
}

int __sort_by_calls_ascending(const void *_a, const void *_b)
{
	PROFILE
	FTable_entry *a = (FTable_entry *)_a;
	FTable_entry *b = (FTable_entry *)_b;

	return a->calls_count - b->calls_count;
}


int __sort_by_name_descending(const void *_a, const void *_b)
{
	PROFILE
	int res = -__sort_by_name_ascending(_a, _b);
	return res;
}

int __sort_by_duration_descending(const void *_a, const void *_b)
{
	PROFILE
	int res = -__sort_by_duration_ascending(_a, _b);
	return res;
}

int __sort_by_calls_descending(const void *_a, const void *_b)
{
	PROFILE
	int res = -__sort_by_calls_ascending(_a, _b);
	return res;
}

#define PROFILER_ENABLED

void table_sort_by_name(FTable *table, enum Sort_direction dir)
{
	PROFILE
	assert(table != NULL);
	assert(table->functions != NULL);

	qsort(
		table->functions,
		table->size,
		sizeof(FTable_entry),
		(dir == SORT_ASCENDING)? __sort_by_name_ascending : __sort_by_name_descending
		);
	return ;
}

void table_sort_by_duration(FTable *table, enum Sort_direction dir)
{
	PROFILE
	assert(table != NULL);
	assert(table->functions != NULL);

	qsort(
		table->functions,
		table->size,
		sizeof(FTable_entry),
		(dir == SORT_ASCENDING)? __sort_by_duration_ascending : __sort_by_duration_descending
		);
	return ;
}

void table_sort_by_calls(FTable *table, enum Sort_direction dir)
{
	PROFILE
	assert(table != NULL);
	assert(table->functions != NULL);

	qsort(
		table->functions,
		table->size,
		sizeof(FTable_entry),
		(dir == SORT_ASCENDING)? __sort_by_calls_ascending : __sort_by_calls_descending
		);
	return ;
}
