from collections import deque
from itertools import combinations_with_replacement, product


def iter_squares(board_size: int = 8):
    x_range = range(1, board_size + 1)
    y_range = range(1, board_size + 1)
    return product(x_range, y_range)


def get_candidate_spots(prev_spots: list[tuple[int, int]], board_size: int = 8):
    def diagonal_iter(anchor_pos):
        x_iter, y_iter = anchor_pos
        for x_step, y_step in combinations_with_replacement((-1, 1), 2):
            while (0 < x_iter <= board_size) and (0 < y_iter <= board_size):
                x_iter += x_step
                y_iter += y_step
                yield (x_iter, y_iter)

            x_iter, y_iter = anchor_pos

    def check_diagonal(pos):
        for check_pos in diagonal_iter(pos):
            if check_pos in prev_spots:
                return False
        return True

    visited_x = [t[0] for t in prev_spots]
    visited_y = [t[1] for t in prev_spots]

    def check_candidate(tup):
        if tup[0] in visited_x or tup[1] in visited_y:
            return False

        return check_diagonal(tup)

    candidates = filter(check_candidate, iter_squares())
    return list(candidates)


def solve():
    states_queue = deque([[s] for s in iter_squares()])
    solutions = []
    counter = 0

    while len(states_queue) > 0:
        board_state = states_queue.pop()

        if len(board_state) > 7 and set(board_state) not in solutions:
            return board_state
            solutions.append(set(board_state))
            counter += 1

            print(f"{counter} {board_state}")

        new_candidates = get_candidate_spots(board_state)

        if not new_candidates:
            continue

        states_queue.extend([board_state + [s] for s in new_candidates])


def main():
    solution = solve()
    print(solution)


if __name__ == "__main__":
    main()
