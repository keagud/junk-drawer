/* 2023-02-27 */

#include <algorithm>
#include <boost/python/list.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <unistd.h>
#include <utility>
#include <vector>

#include "life.hpp"

#include <boost/python.hpp>
#include <boost/python/module.hpp>

#include <boost/python/converter/implicit.hpp>

using std::pair;
using std::string;
using std::vector;

cell_matrix init_grid(int width, int height, bool randomize_start)
{

    cell_matrix grid {};

    if (randomize_start) {
        std::srand(std::time(nullptr));
    }

    grid.reserve(height);

    for (int h = 0; h < height; h++) {

        vector<Cell> row {};

        row.reserve(width);

        for (int w = 0; w < width; w++) {
            row.push_back(make_cell_status(randomize_start));
        }

        grid.push_back(row);
    }

    return grid;
}

std::optional<pair<Cell, int>> count_neighbors(cell_matrix& input_grid, int row, int col)
{

    int height = input_grid.size();
    int width = input_grid.front().size();

    auto oob = [=](int r, int c) {
        return (r >= height || r < 0) || (c >= width || c < 0);
    };

    auto check_cell = [=](int r, int c) {
        return ((!oob(r, c)) && input_grid[r][c].alive) ? 1 : 0;
    };

    if (oob(row, col)) {
        return std::nullopt;
    }

    auto cell_status = input_grid[row][col];

    int sum {};

    // check all 8 neighbors
    sum += check_cell(row, col + 1);
    sum += check_cell(row + 1, col);

    sum += check_cell(row, col - 1);
    sum += check_cell(row - 1, col);

    sum += check_cell(row + 1, col + 1);
    sum += check_cell(row - 1, col - 1);

    sum += check_cell(row - 1, col + 1);
    sum += check_cell(row + 1, col - 1);

    return pair { cell_status, sum };
}

Cell make_cell_status(bool random)
{

    if (!random) {
        return Cell { false };
    }

    return Cell { bool(std::rand() % 2) };
}

cell_matrix iter_cells(cell_matrix input_grid, bool abort_on_end)
{

    // set up the new target grid
    cell_matrix new_grid { input_grid };

    bool any_alive = false;

    for (unsigned r = 0; r < input_grid.size(); r++) {
        for (unsigned c = 0; c < input_grid[r].size(); c++) {

            auto cell_neighbors = count_neighbors(input_grid, r, c);

            if (!cell_neighbors.has_value()) {
                continue;
            }

            auto& new_cell = new_grid[r][c];

            auto [cell, neighbors_count] = cell_neighbors.value();

            // mark a dead cell  with 3 neighbors for revival
            if (!cell.alive) {
                new_cell.marked = (neighbors_count == 3);
                continue;
            }

            // any live cell with fewer than two or more than three neighbors dies
            new_cell.marked = (neighbors_count < 2 || neighbors_count > 3);
            any_alive = true;
        }
    }

    if (abort_on_end && !any_alive) {
        exit(EXIT_SUCCESS);
    }

    return apply_marks(new_grid);
}

cell_matrix& apply_marks(cell_matrix& input_grid)
{

    for (auto& row : input_grid) {

        for (auto& cell : row) {

            if (!cell.marked) {
                continue;
            }

            cell.alive = (!cell.alive);
            cell.marked = false;
        }
    }

    return input_grid;
}
string display(cell_matrix input_grid)
{

    std::stringstream display_str;

    for (auto row : input_grid) {

        string row_str {};

        for (auto cell : row) {
            row_str += cell.alive ? ALIVE_CHAR : DEAD_CHAR;
        }

        display_str << row_str << "\n";
    }

    return display_str.str();
}

boost::python::list serialize(cell_matrix& input_grid)
{

    vector<vector<bool>> bool_vec {};

    boost::python::list bools_list {};

    bool_vec.reserve(input_grid.size());

    for (auto row : input_grid) {

        boost::python::list row_list {};

        // i can't believe it's not haskell!
        auto new_row = std::accumulate(
            row.begin(), row.end(), row_list,
            [](boost::python::list& acc, Cell c) {
                acc.push_back(c.alive);
                return acc;
            });

        bool_vec.push_back(new_row);
    }

    return bool_vec;
}

vector<vector<bool>> init_wrapper(int width, int height)
{
    auto grid = init_grid(width, height);
    return serialize(grid);
}

boost::python::list iter_wrapper(boost::python::list ls)
{

    boost::python::list new_list {};

    // convert the bool vector to Cell type
    cell_matrix new_matrix {};

    for (auto row : input_grid) {

        vector<Cell> r {};

        // i'm so sorry for saying functional patterns in rust were too verbose
        std::transform(row.begin(), row.end(),
            std::back_insert_iterator(r),
            [=](bool x) {
                return Cell { x };
            });

        new_matrix.push_back(r);
    }

    auto iterated_cells = iter_cells(new_matrix);
    return serialize(iterated_cells);
}

/////////////////////////////////////

BOOST_PYTHON_MODULE(life)
{

    using namespace boost::python;
    // def init_grid(height: int, width: int)-> list[list[bool]]
    // def iter_cells(grid: list[list[bool]])-> list[list[bool]]

    def("init_grid", &init_wrapper);
    def("iter_cells", &iter_wrapper);
};

/*
int main()
{

    auto g = init_grid(40, 20);
    std::cout << display(g) << "\n";

    auto s = serialize(g);

    for (auto r : s) {

        for (auto c : r) {

            std::cout << (c ? ALIVE_CHAR : DEAD_CHAR);
        }
        std::cout << std::endl;
    }
}
*/
