#ifndef _H_GUI_
#define _H_GUI_

/**
 * @file
 * @brief GUI function declarations
 */

#include <function_tree.h>
#include <function_table.h>

/**
 * @brief Opens a window, then draws the profiler's interface and handles user input until quitting.
 * @param root A tree of function calls to display.
 * @param table A table of functions to display.
 * @param window_width The width of the window.
 * @param window_height The height of the window.
 * @return 0 upon error, 1 otherwise.
 */
int gui_main(Tree root, FTable *table, int window_width, int window_height);


#endif
