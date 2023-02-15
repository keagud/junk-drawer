#ifndef SNAKE_H
#define SNAKE_H

#include "screen.h"

#define INIT_LENGTH 3
#define INIT_POSITIONS_COUNT 5

#define SNAKE_BODY_CHAR 'O'
#define SNAKE_HEAD_CHAR '0'
#define FOOD_CHAR '*'

typedef struct segment {
    struct segment* next;
    struct segment* prev;
    Point pos;

} Segment;

typedef struct snake {

    Segment* head;
    Segment* tail;
    DIRECTION move_dir;
    int length;
    Point last_tail;

} Snake;

Snake* make_snake(int start_row, int start_col);

void extend_snake(Snake* snek);

/*
 * Move the head in a given DIRECTION
 * Returns true if this move is valid, false if it is into
 * a wall or segement
 * */
bool move_snake(Snake* snek, DIRECTION move_dir);

void print_snake(Snake* snek, WINDOW* win);

void init_print_snake(Snake* snek, WINDOW* win);

void dealloc_snake(Snake* snek);

bool same_point(Point p, Point f);

Point make_food(Snake* snek);


Point munch(Snake* snek, WINDOW* win);

#endif
