
import matplotlib.pyplot as plt
import numpy


prog = ["row_static", "row_guided", "row_dynamic", "block_static", "block_dynamic", "block_guided", "raw"]

size = [50, 100, 200, 500, 1000, 2000, 3000]


y = numpy.zeros((len(prog), len(size)))

with open("profile_1.out", "r") as fo:

	for i in range(0, len(size)):

		for t in range(0, 3):

			for p in range(0, len(prog)):

				y[p][i] += float(fo.readline())



y = y / 3


plt.xlabel("input matrix length")
plt.ylabel("time / us")


for i in range(0, len(prog)):

	plt.plot(size, y[i], '-', label = prog[i])

plt.legend()
plt.show()
