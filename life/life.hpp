#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

constexpr char ALIVE_CHAR = '#';
constexpr char DEAD_CHAR = ' ';

struct Cell {

public:
    bool alive = false;
    bool marked = false;

    Cell() {};

    explicit Cell(bool status)
        : alive(status) {};
};

typedef std::vector<std::vector<Cell>> cell_matrix;

cell_matrix init_grid(int width, int height, bool randomize_start = true);

std::optional<std::pair<Cell, int>> count_neighbors(cell_matrix& input_grid, int row, int col);

Cell make_cell_status(bool random);

cell_matrix iter_cells(cell_matrix input_grid, bool abort_on_end = false);

cell_matrix& apply_marks(cell_matrix& input_grid);

std::string display(cell_matrix input_grid);

std::vector<std::vector<bool>> serialize(cell_matrix& input_grid);
