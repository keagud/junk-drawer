#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "screen.h"
#include "snake.h"

/* make a new snake of size 2 to start */
Snake* make_snake(int start_row, int start_col)
{

    Segment* head;
    Segment* tail;
    Snake* snek;

    head = malloc(sizeof(Segment));
    tail = malloc(sizeof(Segment));

    head->pos.row = start_row;
    head->pos.col = start_col;

    tail->pos.row = start_row + 1;
    tail->pos.col = start_col;

    head->next = tail;
    tail->prev = head;

    head->prev = NULL;
    tail->next = NULL;

    snek = malloc(sizeof(Snake));

    snek->head = head;
    snek->tail = tail;
    snek->move_dir = UP;

    return snek;
}

bool update_segments(Point update_pos, Segment* move_segment, Segment* head)
{

    Point old_segment_pos;

    old_segment_pos = move_segment->pos;

    move_segment->pos.col = update_pos.col;
    move_segment->pos.row = update_pos.row;

    if (move_segment != head && move_segment->pos.col == head->pos.col && move_segment->pos.row == head->pos.row) {
        return false;
    }

    if (move_segment->next != NULL) {
        return update_segments(old_segment_pos, move_segment->next, head);
    }

    return true;
}

/*
 * Move the head in a given DIRECTION
 * Returns true if this move is valid, false if it is into
 * a wall or segement
 * */

bool move_snake(Snake* snek, DIRECTION move_dir)
{

    Segment* head;
    Point next;

    head = snek->head;

    snek->last_tail = snek->tail->pos;

    next = get_direction_coordinates(head->pos.row, head->pos.col, move_dir);

    if (check_oob(next)) {
        return false;
    }

    snek->move_dir = move_dir;

    return update_segments(next, head, head);
}

/*
 * write the snake to the screen for the first time
 * print_snake only updates the head and the tail,
 * so this is needed to initialize
 * */
void init_print_snake(Snake* snek, WINDOW* win)
{
    Segment* s;

    s = snek->head;

    while (s != NULL) {
        mvwaddch(win, s->pos.row, s->pos.col, SNAKE_BODY_CHAR);
        s = s->next;
    }
}

/*
 * Update the head and tail to of the snake
 * the other segments we get for free because of how snakes are!
 */

void print_snake(Snake* snek, WINDOW* win)
{
    Point head_pos;
    Point tail_pos;

    head_pos = snek->head->pos;
    tail_pos = snek->tail->pos;

    mvwaddch(win, head_pos.row, head_pos.col, SNAKE_BODY_CHAR);
    mvwaddch(win, tail_pos.row, tail_pos.col, SNAKE_BODY_CHAR);

    mvwaddch(win, snek->last_tail.row, snek->last_tail.col, ' ');

    wrefresh(win);
}

void extend_snake(Snake* snek)
{

    Segment* new_segment;

    new_segment = malloc(sizeof(Segment));

    new_segment->pos.row = snek->last_tail.row;
    new_segment->pos.col = snek->last_tail.col;

    new_segment->next = NULL;
    new_segment->prev = snek->tail;
    snek->tail->next = new_segment;

    snek->tail = new_segment;
    snek->length++;
}

bool same_point(Point p, Point f)
{
    return (p.row == f.row && p.col == f.col);
}

Point make_food(Snake* snek)
{
    int food_row;
    int food_col;
    Segment* s;
    Point f;
    /* make a random row and column */
    food_row = (rand() % (SCREEN_LINES - 5)) + 2;
    food_col = (rand() % (SCREEN_COLS - 5)) + 2;

    /* check if the chosen location is inside the snake
     * if it is, try again
     */

    s = snek->head;
    f.row = food_row;
    f.col = food_col;

    while (s != NULL) {

        if (same_point(s->pos, f)) {
            return make_food(snek);
        }

        s = s->next;
    }

    return f;
}

/* triggers when food is eaten
 * Adds a segment, makes a new food item, prints it to the
 * screen and returns it
 *
 */
Point munch(Snake* snek, WINDOW* win)
{

    Point new_food_location;

    extend_snake(snek);

    new_food_location = make_food(snek);

    mvwaddch(win, new_food_location.row, new_food_location.col, FOOD_CHAR);

    return new_food_location;
}
