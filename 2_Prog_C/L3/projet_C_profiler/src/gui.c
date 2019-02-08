#include <assert.h>
#include <math.h>

#include <MLV/MLV_all.h>

#include <gui.h>
#include <stack.h>

/* #define PROFILER_ENABLED */
#include <macro_profiler.h>

int text_can_fit(const char *text, int container_width, int container_height)
{
	PROFILE
	int text_width;
	int text_height;

	MLV_get_size_of_text(text, &text_width, &text_height);
	int res = (text_width <= container_width && text_height <= container_height);
	return res;
}

MLV_Color get_color_for_node(Node *node, int depth, double root_duration)
{
	PROFILE

	MLV_Color result;
	double t;

	assert(node != NULL);

	t = (node->funct_duration / root_duration) * 255.0;

	Uint8 r;
	Uint8 g;
	Uint8 b;

	g = (Uint8)(255 - t);
	b = (Uint8)fmin(depth * 30, 255);
	b = 0;
	r = (Uint8)t;

	result = MLV_convert_rgba_to_color(r, g, b, 255);
	return result;
}

void draw_function_tree__(Tree root, int x, int y, int container_width, int container_height, int current_depth, double root_duration, Stack *stack)
{
	PROFILE

	if(root != NULL)
	{
		int text_width;
		int text_height;
		int brothers_count;
		Token *t;

		MLV_get_size_of_text(root->funct_name, &text_width, &text_height);
		brothers_count = get_brothers_count(root);


		if(container_width >= container_height)
		{
			/* Expand right. */
			int current_width = container_width / brothers_count;

			MLV_draw_filled_rectangle(x, y, current_width, container_height, get_color_for_node(root, current_depth, root_duration));
			MLV_draw_rectangle(x, y, current_width, container_height, MLV_COLOR_BLACK);

			if((t = new_token(root, x, y, current_width, container_height)) == NULL)
			{
				/* ...???... */
				fprintf(stderr, "Memory allocation error!\n");
				exit(EXIT_FAILURE);
			}
			push_token(stack, t);

			if(text_can_fit(root->funct_name, current_width, container_height))
			{
				MLV_draw_text(x, y, root->funct_name, MLV_COLOR_BLACK);

				draw_function_tree__(root->Left_son, x + 5, y + text_height, current_width - 2 * 5, container_height - text_height - 5, current_depth + 1, root_duration, stack);
			}
			else
			{
				/* Display "..." ? */
				MLV_draw_text(x, y, "...", MLV_COLOR_BLACK);
			}

			draw_function_tree__(root->Right_bro, x + current_width, y, container_width - current_width, container_height, current_depth, root_duration, stack);
		}
		else
		{
			/* Expand down. */
			int current_height = container_height / brothers_count;

			MLV_draw_filled_rectangle(x, y, container_width, current_height, get_color_for_node(root, current_depth, root_duration));
			MLV_draw_rectangle(x, y, container_width, current_height, MLV_COLOR_BLACK);

			if((t = new_token(root, x, y, container_width, current_height)) == NULL)
			{
				/* ...???... */
				fprintf(stderr, "Memory allocation error!\n");
				exit(EXIT_FAILURE);
			}
			push_token(stack, t);

			if(text_can_fit(root->funct_name, container_width, current_height))
			{
				MLV_draw_text(x, y, root->funct_name, MLV_COLOR_BLACK);

				draw_function_tree__(root->Left_son, x + 5, y + text_height, container_width - 2 * 5, current_height - text_height - 5, current_depth + 1, root_duration, stack);
			}
			else
			{
				/* Display "..." ? */
				MLV_draw_text(x, y, "...", MLV_COLOR_BLACK);
			}

			draw_function_tree__(root->Right_bro, x, y + current_height, container_width, container_height - current_height, current_depth, root_duration, stack);
		}

	}
	return ;
}

void draw_function_tree(Tree root, int x, int y, int container_width, int container_height, Stack *stack)
{
	PROFILE

	assert(stack != NULL);

	if(root != NULL)
	{
		draw_function_tree__(root, x, y, container_width, container_height, 0, root->funct_duration, stack);
	}
	return ;
}


void draw_function_table(FTable *table, int x, int y, int container_width, int container_height)
{
	PROFILE

	int text_height;
	int displayable_count;
	int i;

	assert(table != NULL);
	assert(table->functions != NULL);

	MLV_draw_filled_rectangle(x, y, container_width, container_height, MLV_COLOR_BLUE);
	MLV_draw_text(x + 5, y + 5, "Calls | Total duration | Function", MLV_COLOR_WHITE);

	/* Compute the maximal number of functions that can be displayed */
	MLV_get_size_of_text("a", NULL, &text_height);
	y += text_height;

	displayable_count = (container_height - text_height - 5) / text_height;
	if(displayable_count > table->size)
		displayable_count = table->size;

	for(i = 0; i < displayable_count; ++i)
	{
		MLV_draw_text(
			x + 5,
			y + 5 + i * text_height,
			"%05d | %05.8f | %s",
			MLV_COLOR_WHITE,
			table->functions[i].calls_count,
			table->functions[i].total_duration,
			table->functions[i].funct_name);
	}
	return ;
}


void draw_buttons(int button_x, int button_height, MLV_Image *im_up_arraw, int up_arraw_x, MLV_Image *im_down_arraw, int down_arraw_x, int updown_arraw_y)
{
	PROFILE
	MLV_draw_image(im_up_arraw, up_arraw_x, updown_arraw_y);
	MLV_draw_image(im_down_arraw, down_arraw_x, updown_arraw_y); 
	/* First Button */
	MLV_draw_text_box(button_x, 0,
		200, button_height,
		"Sort by name", 3,
		MLV_COLOR_RED, MLV_COLOR_RED,
		MLV_rgba(50, 200, 150, 255),
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER);
  	/* Second Button */
	MLV_draw_text_box(button_x, button_height+30,
       		200, button_height,
		"Sort by duration", 3,
		MLV_COLOR_RED, MLV_COLOR_RED,
	       	MLV_rgba(50, 200, 150, 255),
       		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
	       	MLV_VERTICAL_CENTER);
	/* Third Button */
     	MLV_draw_text_box(button_x, (button_height+30)*2,
       		200, button_height,
		"Sort by calls count", 3,
		MLV_COLOR_RED, MLV_COLOR_RED,
	       	MLV_rgba(50, 200, 150, 255),
       		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
	       		  MLV_VERTICAL_CENTER);
	/* Fourth Button */
	MLV_draw_text_box(button_x, (button_height+30)*3,
       		200, button_height,
		"Reset Tree", 3,
		MLV_COLOR_RED, MLV_COLOR_RED,
	       	MLV_rgba(50, 200, 150, 255),
       		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
	       	MLV_VERTICAL_CENTER);
	return ;
}


int gui_main(Tree root, FTable *table, int window_width, int window_height)
{
	PROFILE

	Tree current_root = root;
	Token *new_root;
	Stack stack = NULL;

	int event_type;
	MLV_Keyboard_button key;
	int mouse_x;
	int mouse_y;

	MLV_create_window("C PROFILER", NULL, window_width + 100, window_height);

	int button_x = window_width-100;
	int button_height = (window_height/4) - 30;

	int button_up_x = button_x+50;
	int padding_updown_button = 60;
	int button_down_x = button_up_x + padding_updown_button;

	int button_updown_y = (button_height+30)*3 + button_height;
	MLV_Image *im_up_arraw= MLV_load_image("bin/up_arraw.png");
	MLV_Image *im_down_arraw = MLV_load_image("bin/down_arraw.png");

	if(im_up_arraw == NULL || im_down_arraw == NULL)
	{
		return 0;
	}

	int etat = -1;

	while(1)
	{
		MLV_clear_window(MLV_COLOR_WHITE);
		draw_buttons(button_x, button_height, im_up_arraw, button_up_x, im_down_arraw, button_down_x, button_updown_y);

		draw_function_tree(current_root, 0, 0, (window_width-100) / 2, window_height, &stack);
		draw_function_table(table, (window_width-100) / 2, 0, (window_width-100) / 2, window_height);

		MLV_update_window();
		event_type = MLV_wait_keyboard_or_mouse(&key, NULL, NULL, &mouse_x, &mouse_y);

		if(event_type == MLV_KEY)
		{
			switch(key)
			{
				case MLV_KEYBOARD_BACKSPACE:
					current_root = root;
					break;
				default:
					break;
			}
		}
		if(event_type == MLV_MOUSE_BUTTON)
		{
			/* First button */
			if(mouse_x > button_x && mouse_y < button_height)
			{
				table_sort_by_name(table, SORT_ASCENDING);
				etat = 1;
			}
			/* Second button */
			else if(mouse_x > button_x && (mouse_y > button_height+30 && mouse_y < button_height+30+button_height))
			{
				table_sort_by_duration(table, SORT_ASCENDING);
				etat = 2;
			}
			/* Third button */
			else if(mouse_x > button_x && (mouse_y > (button_height+30)*2 && mouse_y < (button_height+30)*3))
			{
				table_sort_by_calls(table, SORT_ASCENDING);
				etat = 3;
			}
			/* Fourth button */
			else if(mouse_x > button_x && (mouse_y > (button_height+30)*3 && mouse_y < (button_height+30)*3 + button_height))
			{
				current_root = root;
			}
			/* Button up */
			else if((mouse_x > button_up_x && mouse_x < button_up_x+40) && mouse_y > button_updown_y)
			{
				if(etat== 1)
					table_sort_by_name(table, SORT_ASCENDING);
				else if(etat== 2)
					table_sort_by_duration(table, SORT_ASCENDING);
				else if(etat== 3)
					table_sort_by_calls(table, SORT_ASCENDING);
			}
			/*Button down */
			else if((mouse_x > button_down_x && mouse_x < button_down_x+40) && mouse_y > button_updown_y)
			{
				if(etat == 1)
					table_sort_by_name(table, SORT_DESCENDING);
				else if(etat== 2)
					table_sort_by_duration(table, SORT_DESCENDING);
				else if(etat== 3)
					table_sort_by_calls(table, SORT_DESCENDING);
			}
			new_root = which_token(stack, mouse_x, mouse_y);
			if(new_root != NULL)
			{
				current_root = new_root->function;
			}
		}
	}
	free_stack(&stack);

	MLV_free_window();

	return 1;
}
