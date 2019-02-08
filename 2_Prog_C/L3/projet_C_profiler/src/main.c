#include <stdio.h>
#include <stdlib.h>

#include <function_tree.h>
#include <gui.h>
#include <function_table.h>

/* #define PROFILER_ENABLED */
#define MAIN_FILE
#include <macro_profiler.h>

#define LOG_FILE_NAME	(argv[1])


int main(int argc, char **argv)
{
	PROFILE

	if(argc >= 2)
	{
		Tree  log_tree = NULL;
		FILE *log_file = NULL;
		FTable *func_table = NULL;

		/* TODO: parse these values from the command line. */

		const int window_width = 800;
		const int window_height = 600;

		if(NULL != (log_file = fopen(LOG_FILE_NAME, "r")))
		{
			func_table = table_init();
			construct_tree_logFile(&log_tree, log_file, func_table);
			fclose(log_file);

			fprintf(stderr, "Log file successfully parsed.\n");

			if(!gui_main(log_tree, func_table, window_width, window_height))
			{
				fprintf(stderr, "GUI error!\n");
			}

			erase_Tree(&log_tree);
			table_free(func_table);

			return 0;
		}
		else
		{
			fprintf(stderr, "Error: %s not found or unavailable!\n", LOG_FILE_NAME);
			return 1;
		}
	}
	else
	{
		fprintf(stderr, "Error: please provide a path to a .log file.\n");
		return 1;
	}
}
