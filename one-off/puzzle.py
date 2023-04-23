"""
puzzle
"""

# 2023-04-21

import re
import sys
from functools import reduce
from itertools import chain


def get_dictwords():
    with open("/usr/share/dict/words") as infile:
        words = set(
            line.strip().upper() for line in infile.readlines() if len(line.strip()) > 2
        )

    return words


def get_rows_cols(puzzle):
    rows = puzzle.split("\n")

    def itercols(ls: list, b: str):
        if not ls:
            ls = [[] for _ in b.strip()]

        for i, c in enumerate(b.strip()):
            ls[i].append(c)

        return ls

    cols = ["".join(c) for c in reduce(itercols, rows, [])]
    return (rows, cols)


def iter_words(line: str):
    for run in (tok for tok in line.split("#") if tok and len(tok) > 2):
        yield run


def search(rows, cols):
    all_lines = [line.strip() for line in chain(rows, cols) if line]

    found = set()
    words = get_dictwords()

    duck = False

    for line_word in chain.from_iterable([iter_words(line) for line in all_lines]):
        if line_word in words:
            found.add(line_word)
            continue

        if "*" in line_word:
            for word in words:
                regex_match = re.match(line_word, word)
                if not regex_match:
                    continue

                found.add(line_word)
                duck = True
                continue

    return (duck, found)


def get_params() -> tuple[str, str]:
    puzzle_path = None
    key_letters = None

    try:
        puzzle_path = sys.argv[1]
        key_letters = sys.argv[2]

    except IndexError:
        if puzzle_path is None:
            puzzle_path = input("Enter path to puzzle file: ")

        if key_letters is None:
            key_letters = input("Enter your letters: ")

    return (puzzle_path, key_letters)


def solve(puzzle, letters):
    payouts = {
        4: 2,
        5: 5,
        6: 10,
        7: 25,
        8: 50,
        9: 100,
        10: 1000,
        11: 20000,
    }

    # letters = "auieocxhnfpyqwbmgd"

    for letter in letters:
        puzzle = puzzle.replace(letter, letter.upper())

    rows, cols = get_rows_cols(puzzle)

    duck, found = search(rows, cols)

    print(f"Found {len(found)} words: ")

    for w in found:
        print(f"\t{w}")

    base_payout = payouts.get(len(found), 0)

    print(f"Get ${base_payout} {len(found)} words")

    if duck:
        print(f"duck bonus: x2 for total of {base_payout * 2}")


def main():
    path, letters = get_params()

    with open(path, "r") as infile:
        file_contents = infile.read().lower()

    solve(file_contents, letters)


if __name__ == "__main__":
    main()
