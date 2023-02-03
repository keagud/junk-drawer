from itertools import product
from itertools import combinations_with_replacement


class Board:
    squares: dict[tuple[int, int], bool]
    size: int
    filled: list[tuple[int, int]]

    def clear_board(self):
        self.squares = {}

        for x, y in product(range(1, self.size + 1), range(1, self.size + 1)):
            self.squares[(x, y)] = False

    def __iter__(self):
        x_range = range(1, self.size + 1)
        y_range = range(1, self.size + 1)
        for x, y in product(x_range, y_range):
            yield ((x, y), self.squares.get((x, y), False))

    def __init__(self, size: int = 8):
        self.size = size
        self.squares = {}
        self.filled = []

        self.clear_board()

    def set_at(self, x: int, y: int):
        self.squares[(x, y)] = True
        self.filled.append((x, y))

    def _check_horizontal(self, pos):
        _, y = pos
        for x_delta in range(1, self.size + 1):
            if self.squares.get((x_delta, y)):
                return True
        return False

    def _check_vertical(self, pos):
        x, _ = pos
        for y_delta in range(1, self.size + 1):
            if self.squares.get((x, y_delta)):
                return True
        return False

    def _diagonal_iter(self, anchor_pos):
        x_iter, y_iter = anchor_pos

        for x_step, y_step in combinations_with_replacement((-1, 1), 2):
            while (0 < x_iter <= self.size) and (0 < y_iter <= self.size):
                x_iter += x_step
                y_iter += y_step
                yield (x_iter, y_iter)

            x_iter, y_iter = anchor_pos

    def _check_diagonal(self, pos):
        for check_pos in self._diagonal_iter(pos):
            if self.squares.get(check_pos):
                return True
        return False

    def add_queen(self):
        for pos, val in self.__iter__():
            if val or pos in self.filled:
                continue
            if self._check_horizontal(pos):
                continue

            if self._check_vertical(pos):
                continue

            if self._check_diagonal(pos):
                continue

            self.squares[pos] = True
            return pos

        return None

    def permute(self):
        self.clear_board()


        for start_pos , _ in self.__iter__():
            self.clear_board()

            self.squares[start_pos] = True

            spots = []
            while (result:= self.add_queen()) is not None:
                spots.append(result)

            yield spots



            



board = Board()
solutions = list(board.permute())

solutions.sort(key=lambda x: len(x))
solutions.reverse()

print(solutions)

