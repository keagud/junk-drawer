#ifndef SCREEN_H
#define SCREEN_H

#include <curses.h>
#include <ncurses.h>

#ifndef SCREEN_LINES
#define SCREEN_LINES 20
#endif

#ifndef SCREEN_COLS
#define SCREEN_COLS 40
#endif

typedef char SCREEN_ARR[SCREEN_LINES][SCREEN_COLS];
typedef struct
{

    char left;
    char right;
    char top;
    char bottom;

    char top_left;
    char top_right;
    char bottom_left;
    char bottom_right;

} WinBorder;

typedef enum {

    NONE,
    UP,
    RIGHT,
    DOWN,
    LEFT

} DIRECTION;

typedef struct
{
    int row;
    int col;
} Point;

/*
 * convert a DIRECTION to a point on the grid, given the current location
 * does NOT do any checking for out of bounds
 * */
Point get_direction_coordinates(int current_row, int current_col, DIRECTION move_dir);

bool check_oob(Point p);

void initialize(void);


void debug_print(char* message, WINDOW* win);



/* translate a keypress to DIRECTION */
DIRECTION get_direction_input(WINDOW* win);

WINDOW* make_window(void);

void fill_screen(char arr[SCREEN_LINES][SCREEN_COLS], char fill_char);

void delete_window(WINDOW* win);

/*set the contents of a char array as the window contents, and refereshes*/
void print_to_win(WINDOW* win, char matrix[SCREEN_LINES][SCREEN_COLS], int height, int width);

/*cleanup and deallocation*/
void end_curses(WINDOW* win);

#endif
