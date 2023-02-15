
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "snake.h"

/* time to wait for user input on each frame */
#define FRAME_TIME 2

int main(void)
{

    WINDOW* win;
    Snake* snek;
    int score;
    Point food_location;
    DIRECTION input_dir;

    /* initial setup and initialization for display */

    initscr();
    noecho();
    curs_set(0);
    halfdelay(FRAME_TIME);

    /*seed rng for spawning food*/
    srand(time(NULL));

    win = make_window();
    keypad(win, true);

    snek = make_snake(4, 4);
    extend_snake(snek);
    /* main game loop */
    score = 0;

    food_location = make_food(snek);

    mvwaddch(win, food_location.row, food_location.col, FOOD_CHAR);

    while (true) {

        mvwprintw(win, 0, SCREEN_COLS - 10, "Score: %d", score);
        input_dir = get_direction_input(win);

        if (input_dir == NONE) {
            input_dir = snek->move_dir;
        }

        if (!move_snake(snek, input_dir)) {
            /* game over */
            break;
        }

        if (same_point(snek->head->pos, food_location)) {
            food_location = munch(snek, win);
            score++;
        }

        print_snake(snek, win);
    }

    /* cleanup */
    end_curses(win);
    endwin();


    printf("Game over!\n");
    printf("You scored %d points\n", score);
    

    return 0;
}
