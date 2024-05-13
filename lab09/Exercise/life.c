#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main (int argc, char* argv[])
{
	if ( argc < 0 ) { puts ("No argument specified. Provide n"); return 1; }

	int threadsNumber = atoi ( argv[1] );
	threadsNumber = ( threadsNumber > grid_width * grid_height ) ? grid_width * grid_height : threadsNumber;

	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	init_grid(foreground);

	while (true)
	{
		draw_grid(foreground);
		usleep(500 * 1000);

		// Step simulation

		// update_grid(foreground, background);
		update_grid_threads(foreground, background, threadsNumber);

		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	freeThreadsMemory(); 

	return 0;
}
