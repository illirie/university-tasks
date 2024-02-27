import matplotlib.pyplot as plt
x = [ 700, 2100, 6300 ]
m = 1
def mean(a: list):
    return sum(a) / len(a)

n700 = [ 5.668333333 ]
n2100 = [ 195.30833333 ]
n6300 = [ 8946.936666667 ]

n700cpu = [ 0.188671380, 0.184290558, 0.185793415, 0.185305342, 0.202263549, 0.184750527, 0.188671380, 0.184290558, 0.185793415, 0.185305342, 0.202263549, 0.184750527 ]
n2100cpu = [ 0.983417273, 0.982228100, 0.981985092, 0.971361935, 0.982267737, 0.983336031, 0.983417273, 0.982228100, 0.981985092, 0.971361935, 0.982267737, 0.983336031 ]
n6300cpu = [ 12.116611481, 12.220718384, 12.132096291, 12.125092506, 12.232748985, 12.216617584, 12.116611481, 12.220718384, 12.132096291, 12.125092506, 12.232748985, 12.216617584 ]

n700gpu = [ 0.157131299, 0.157118365, 0.156042814, 0.157801062, 0.155279204, 0.172093064, 0.157131299, 0.157118365, 0.156042814, 0.157801062, 0.155279204, 0.172093064 ]
n2100gpu = [ 0.391367584, 0.392120630, 0.397672296, 0.389998645, 0.392433703, 0.390913397, 0.391367584, 0.392120630, 0.397672296, 0.389998645, 0.392433703, 0.390913397 ]
n6300gpu = [ 6.353066444, 6.328887463, 6.339453697, 6.320838928, 6.324519157, 6.328909397, 6.353066444, 6.328887463, 6.339453697, 6.320838928, 6.324519157, 6.328909397 ]

t = [mean(n700), mean(n2100), mean(n6300)]
cpu = [mean(n700cpu), mean(n2100cpu), mean(n6300cpu)]
gpu = [mean(n700gpu), mean(n2100gpu), mean(n6300gpu)]

print('consistent time: ')
for i in range(3):
    print(round(t[i] * m, 4))

print('cpu time: ')
for i in range(3):
    print(round(cpu[i] *m, 4))

print('gpu time: ')
for i in range(3):
    print(round(gpu[i] *m, 4))

print('cpu a: ')
for i in range(3):
    print(round(t[i] / cpu[i], 4))

print('gpu a: ')
for i in range(3):
    print(round(t[i] / gpu[i], 4))


legend = ['cpu generated', 'gpu generated']
plt.xlabel("N элементов")
plt.ylabel("Время работы")
plt.plot(x, cpu, 'c')
plt.plot(x, gpu, 'g')
plt.legend(legend, loc="upper left")
plt.grid()
plt.show()


plt.xlabel("N элементов")
plt.ylabel("Ускорение")
plt.plot(x, [t[i] / cpu[i] for i in range(3)], 'c')
plt.plot(x, [t[i] / gpu[i] for i in range(3)], 'g')
plt.legend(legend, loc="upper left")
plt.grid()
plt.show()
