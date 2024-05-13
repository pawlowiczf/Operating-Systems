#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{
    int count = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

// Changed:

typedef struct ThreadJob {
    //
    int start, end; 
    char* src; 
    char* dst; 
} ThreadJob;

pthread_t* threads = NULL; 
ThreadJob* threadsJob = NULL;

void* update_grid_thread (void* data) {
    //
    ThreadJob* threadJob = (ThreadJob*) data; 

    while (1) {
        //
        for (int a = threadJob->start; a < threadJob->end; a++) {
            threadJob->dst[a] = is_alive (a / grid_width, a % grid_height, threadJob->src);
        }

        pause (); 

        char* tmp; 
        tmp = threadJob->dst; 
        threadJob->dst = threadJob->src; 
        threadJob->src = tmp; 
    }

    return NULL;
}

void handler() {}

void update_grid_threads(char *src, char *dst, int threadsNumber) {
    //
    int err; 
    int blockSize = grid_height * grid_width / threadsNumber; 

    if ( threads == NULL ) {
        //
        // struct sigaction sa; 
        // sa.sa_handler = &handler; 
        // sigaction (SIGALRM, &sa, NULL);
        signal (SIGALRM, &handler);
        
        threads = malloc ( sizeof(pthread_t) * threadsNumber );
        threadsJob = malloc ( sizeof(ThreadJob) * threadsNumber );

        for (int a = 0; a < threadsNumber; a++) {
            //
            err = pthread_create ( &threads[a], NULL, &update_grid_thread, &threadsJob[a] );
            if ( err < 0 ) { perror ("Error: pthread_create()"); exit(EXIT_FAILURE); }

            threadsJob[a].src = src; 
            threadsJob[a].dst = dst; 
            threadsJob[a].start = a * blockSize; 
            threadsJob[a].end = a * blockSize + blockSize; 

            if (a == threadsNumber - 1) { threadsJob[a].end = grid_height * grid_width; }
        }
    }

    for (int a = 0; a < threadsNumber; a++) {
        pthread_kill ( threads[a], SIGALRM );
    }

}

void freeThreadsMemory() {
    free (threadsJob);
    free (threads);
}