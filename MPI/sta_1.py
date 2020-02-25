
import matplotlib.pyplot as plt
import numpy


prog = ["avx", "2*2", "3*3", "4*4"]

size = [100, 200, 500, 1000, 2000, 3000]


y = numpy.zeros((len(prog), len(size)))
#y_avx = numpy.zeros((1, len(size)))
with open("profile_1.out", "r") as fo:

	for i in range(0, len(size)):

		for t in range(0, 3):

			#y_avx += float(fo.readline())
			for p in range(0, len(prog)):


				y[p][i] += float(fo.readline())



y = y / 3


plt.xlabel("input matrix length")
plt.ylabel("time / us")


for i in range(1, len(prog)):

	plt.plot(size, y[i], '-', label = prog[i])

plt.legend()
plt.show()
