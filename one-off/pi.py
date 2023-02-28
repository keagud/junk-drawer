from itertools import accumulate, count, cycle, islice
from operator import add
from sys import argv

leibniz_series = ((s * (1 / d)) for s, d in zip(cycle((1, -1)), count(1, step=2)))

iterations = 1000

if len(argv) > 1:
    iterations = int(argv[1])


for t in islice(accumulate(leibniz_series, add), 0, iterations):
    print(4 * t)
