/* 2023-02-06 */

#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "screen.h"

void initialize(void)
{

    initscr();
    cbreak();
    noecho();
    curs_set(0);
}

WINDOW* make_window(void)
{

    int height;
    int width;
    int startx;
    int starty;

    WINDOW* new_win;

    height = SCREEN_LINES;
    width = SCREEN_COLS;

    starty = (LINES / 2) - (height / 2);
    startx = (COLS / 2) - (width / 2);

    new_win = newwin(height + 1, width + 1, starty, startx);

    box(new_win, 0, 0);
    wrefresh(new_win);

    return new_win;
}

Point get_direction_coordinates(int current_row, int current_col, DIRECTION move_dir)
{
    Point return_vals;
    switch (move_dir) {
    case UP:
        current_row--;
        break;

    case DOWN:
        current_row++;
        break;

    case LEFT:
        current_col--;
        break;

    case RIGHT:
        current_col++;
        break;

    default:
        break;
    }

    return_vals.row = current_row;
    return_vals.col = current_col;
    return return_vals;
}

bool check_oob(Point p)
{

    int r;
    int c;

    r = p.row;
    c = p.col;

    if (r < 1 || r > SCREEN_LINES) {
        return true;
    }

    if (c < 1 || c > SCREEN_COLS) {
        return true;
    }
    return false;
}

/*cleanup and deallocation*/
void end_curses(WINDOW* win)
{

    delwin(win);
    win = NULL;

    endwin();
}

void fill_screen(char arr[SCREEN_LINES][SCREEN_COLS], char fill_char)
{

    int r;
    for (r = 0; r < SCREEN_LINES; r++) {
      int c;
        for (c = 0; c < SCREEN_COLS; c++) {
            arr[r][c] = fill_char;
        }
    }
}

DIRECTION get_direction_input(WINDOW* win)
{

    int input = wgetch(win);

    switch (input) {

    case KEY_UP:
    case 'w':
        return UP;

    case KEY_DOWN:
    case 's':
        return DOWN;
    case KEY_LEFT:
    case 'a':
        return LEFT;

    case KEY_RIGHT:
    case 'd':
        return RIGHT;

    default:
        return NONE;
    }
}
