import matplotlib.pyplot as plt

def mean(a: list):
    return sum(a) / len(a)

x = [5, 10, 15]
t_standart = 0.0332

mpi1_5 = [ 0.020121, 0.019133, 0.018881 ]
mpi1_10 = [ 0.018674, 0.018875, 0.019315 ]
mpi1_15 = [ 0.0189 ]

mpi2_5 = [ 0.019525, 0.019509, 0.018417]
mpi2_10 = [ 0.019674, 0.019875, 0.019315 ]
mpi2_15 = [ 0.0195 ]

t_single =  [ 0.0131, 0.0178, 0.0242 ]
t_dynamic = [ 0.0174, 0.0230, 0.0367 ]
t_guided =  [ 0.0088, 0.0093, 0.0104 ]

t_mpi1 =    [ mean(mpi1_5), mean(mpi1_10), mean(mpi1_15) ]
t_mpi2 =    [ mean(mpi2_5), mean(mpi2_10), mean(mpi2_15)]

for i in t_mpi1:
    print(round(i, 4))

for i in t_mpi2:
    print(round(i, 4))

for i in range(3):
    print(round(t_standart / t_mpi1[i], 4))

for i in range(3):
    print(round(t_standart / t_mpi2[i], 4))

legend = ['static', 'dynamic', 'guided', 'mpi 1', 'mpi 2']
plt.xlabel("N потоков/процессов")
plt.ylabel("Время работы, c")
plt.plot(x, t_single, 'r')
plt.plot(x, t_dynamic, 'g')
plt.plot(x, t_guided, 'b')
plt.plot(x, t_mpi1, 'y')
plt.plot(x, t_mpi2, 'pink')
plt.legend(legend, loc="upper left")
plt.grid()
plt.show()

plt.xlabel("N потоков/процессов")
plt.ylabel("Ускорение")
plt.plot(x, [t_standart / i for i in t_single], 'r')
plt.plot(x, [t_standart / i for i in t_dynamic], 'g')
plt.plot(x, [t_standart / i for i in t_guided], 'b')
plt.plot(x, [t_standart / i for i in t_mpi1], 'y')
plt.plot(x, [t_standart / i for i in t_mpi2], 'pink')
plt.legend(legend, loc="upper left")
plt.grid()
plt.show()

t_standart = 0.3984

t_single =  [ 0.0989, 0.0788, 0.0610 ]
t_dynamic = [ 0.1035, 0.0726, 0.0785 ]
t_guided =  [ 0.0940, 0.0587, 0.0534 ]

t_mpi1 =    [ 0.0186, 0.0192, 0.0191 ]
t_mpi2 =    [ 0.0197, 0.0194, 0.0190 ]

plt.xlabel("N потоков/процессов")
plt.ylabel("Время работы, с")
plt.plot(x, t_single, 'r')
plt.plot(x, t_dynamic, 'g')
plt.plot(x, t_guided, 'b')
plt.plot(x, t_mpi1, 'y')
plt.plot(x, t_mpi2, 'pink')
plt.legend(legend, loc="upper left")
plt.grid()
plt.show()

plt.xlabel("N потоков/процессов")
plt.ylabel("Ускорение")
plt.plot(x, [t_standart / i for i in t_single], 'r')
plt.plot(x, [t_standart / i for i in t_dynamic], 'g')
plt.plot(x, [t_standart / i for i in t_guided], 'b')
plt.plot(x, [t_standart / i for i in t_mpi1], 'y')
plt.plot(x, [t_standart / i for i in t_mpi2], 'pink')
plt.legend(legend, loc="upper left")
plt.grid()
plt.show()

for i in t_mpi2:
    print(t_standart / i)

