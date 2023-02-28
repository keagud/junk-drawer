/* 2023-02-27 */

#include <chrono>
#include <ctime>
#include <iostream>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <unistd.h>
#include <utility>
#include <vector>

/*
#include <boost/python.hpp>
#include <boost/python/module.hpp>
*/

using std::pair;
using std::string;
using std::vector;

constexpr char ALIVE_CHAR = '#';
constexpr char DEAD_CHAR = ' ';

struct Cell {

public:
    bool alive = false;
    bool marked = false;
};

typedef vector<vector<Cell>> cell_matrix;

class LifeGrid {

    std::optional<pair<Cell, int>> count_neighbors(cell_matrix input_grid, int row, int col)
    {

        auto oob = [=](int r, int c) {
            return (r >= this->height || r < 0) || (c >= this->width || c < 0);
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

    std::optional<pair<Cell, int>> count_neighbors(int row, int col)
    {
        return count_neighbors(this->grid, row, col);
    }

public:
    const int width {};
    const int height {};

    cell_matrix grid {};

    Cell make_cell_status(bool random)
    {

        if (!random) {
            return Cell { false };
        }

        return Cell { bool(std::rand() % 2) };
    }

    LifeGrid(int width, int height, bool randomize_start = true)
        : width(width)
        , height(height)
    {

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
    }

    LifeGrid(bool randomize_start = true)
        : LifeGrid(60, 30, randomize_start) {};

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

    cell_matrix iter_cells(cell_matrix input_grid, bool abort_on_end = false)
    {

        // set up the new target grid
        cell_matrix new_grid { input_grid };

        bool any_alive = false;

        for (unsigned r = 0; r < input_grid.size(); r++) {
            for (unsigned c = 0; c < input_grid[r].size(); c++) {

                auto cell_neighbors = count_neighbors(r, c);

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

        new_grid = apply_marks(new_grid);
        this->grid = new_grid;

        return new_grid;
    }

    cell_matrix iter_cells(bool abort_on_end = false)
    {
        return this->iter_cells(this->grid, abort_on_end);
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

    string display() { return display(this->grid); }

    vector<vector<bool>> serialize(cell_matrix input_grid)
    {

        vector<vector<bool>> bool_vec {};

        bool_vec.reserve(input_grid.size());

        for (auto row : input_grid) {

            vector<bool> row_values {};
            row_values.reserve(row.size());

            // i can't believe it's not haskell!
            auto new_row = std::accumulate(
                row.begin(), row.end(), row_values,
                [](std::vector<bool>& acc, Cell c) {
                    acc.push_back(c.alive);
                    return acc;
                });

            bool_vec.push_back(new_row);
        }

        return bool_vec;
    };

    vector<vector<bool>> serialize() { return serialize(this->grid); };
};

/*
BOOST_PYTHON_MODULE(life)
{

    using namespace boost::python;

    class_<LifeGrid>("LifeGrid", init<int, int>())
        .def("greet", &LifeGrid::greet);
}
*/

int main()
{

    auto g = LifeGrid {};

    while (1) {

        std::cout << string(100, '\n') << std::endl;

        std::cout << g.display();
        ;
        std::cout << "=========================" << std::endl;
        g.iter_cells();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
