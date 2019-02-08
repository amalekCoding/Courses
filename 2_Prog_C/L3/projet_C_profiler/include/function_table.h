#ifndef _H_FUNCTION_TABLE_
#define _H_FUNCTION_TABLE_

/**
 * @file
 * @brief Provides an interface to store and manipulate a list of functions and their associated informations.
 */

#include <function_tree.h>

/**
 * @brief Default number of entries in a new table.
 */
#define DEFAULT_TABLE_SIZE	10

/**
 * @brief A structure packing general informations about a function, rather than a call to a function.
 */
typedef struct
{
	char   funct_name[MAX_FUNC_NAME_LENGTH];	/*!< The name of the function */
	double total_duration;	/*!< The cumulated durations of all calls to the function */
	int    calls_count;	/*!< The number of calls to the function */
}FTable_entry;

/**
 * @brief A structure holding global informations about every profiled function. It can be queried with table_get() and augmented with table_add().
 *
 * This structure behaves like a dynamic array of function entries. Through table_get() and table_add(), it is accessed as a hash table, mapping a function name to a number of calls and a total duration.
 */
typedef struct
{
	FTable_entry *functions;	/*!< A dynamic array of entries */
	int size;	/*!< The current number of valid entries */
	int capacity;	/*!< The total number of entries that can be used before requiring reallocation. */
}FTable;

/**
 * @brief Create an empty function table.
 * @return An FTable structure allocated by malloc, or NULL on error.
 */
FTable *table_init(void);
/**
 * Free a whole function table.
 * @param table An FTable allocated by table_init().
 */
void table_free(FTable *table);
/**
 * @brief Add a function to an existing FTable structure. If the function was already present within the table, its associated values are updated. Otherwise, a new entry is created.
 * @param table An FTable allocated by table_init().
 * @param name The name of the new function.
 * @param duration The duration of the new function. If the function was already present within the table, it is increased by this amount.
 * @return 0 upon error, 1 otherwise.
 */
int table_add(FTable *table, const char *name, double duration);

/**
 * @brief Look for a function in the given table. Return its associated entry if present, NULL otherwise.
 * @param table An FTable allocated by table_init().
 * @param name The name of the queried function.
 * @return The FTable_entry describing the function if it is found within the table. NULL if the function is not in the table.
 */
FTable_entry *table_get(FTable *table, const char *name);

/**
 * @brief Builds a tree of calls from the contents of a file, maintaining a table of functions at the same time.
 *
 * Parses the content of a log file and constructs a table of functions and a tree of calls.
 * @param T a pointer to the tree which will be created, IE a pointer to a pointer of nodes.
 * @param log An open file / stream containing the log of a profiled program.
 * @param table A pointer to the table of functions which will be created. 
 */
void construct_tree_logFile(Tree* T, FILE* log, FTable *table);

/**
 * @brief The direction in which a sorting function should sort the entries.
 */ 
enum Sort_direction
{
	SORT_ASCENDING,	/*!< Entries are sorted by increasing attribute value. */
	SORT_DESCENDING	/*!< Entries are sorted by decreasing attribute value. */
};

/**
 * @brief Sort the content of the table by function name. The table is modified directly.
 * @param table A table of functions allocated by table_init().
 * @param dir The direction in which the sorting should happen.
 */
void table_sort_by_name(FTable *table, enum Sort_direction dir);

/**
 * @brief Sort the content of the table by duration. The table is modified directly.
 * @param table A table of functions allocated by table_init().
 * @param dir The direction in which the sorting should happen.
 */
void table_sort_by_duration(FTable *table, enum Sort_direction dir);

/**
 * @brief Sort the content of the table by calls. The table is modified directly.
 * @param table A table of functions allocated by table_init().
 * @param dir The direction in which the sorting should happen.
 */
void table_sort_by_calls(FTable *table, enum Sort_direction dir);



#endif