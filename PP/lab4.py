import matplotlib.pyplot as plt

def mean(a: list):
    return sum(a) / len(a)

n = 4
m = 10 ** 6
t_standart = 0.0332
t_second = [ 0.003504, 0.003613,  0.003651, 0.003111, 0.003071,  0.003064 ]
t_third = [ 0.002455, 0.003894, 0.002745, 0.003633, 0.003042, 0.003231 ]
print(round(mean(t_second)*m, n))
print(round(mean(t_third)*m, n))

a2048_512_1 = [ 0.001156640, 0.001150144, 0.001151712, 0.001149792, 0.001165184, 0.001154496 ]
a2048_512_2 = [ 0.000158560, 0.000187584, 0.000157600, 0.000158080, 0.000157152, 0.000157568 ]
a2048_512_3 = [ 0.000134240, 0.000133888, 0.000134784, 0.000134368, 0.000133920, 0.000134400 ]

a512_512_1 = [ 0.001222240, 0.001220288, 0.001220928, 0.001224416, 0.001222144, 0.001220864 ]
a512_512_2 = [ 0.000130720, 0.000130272, 0.000128992, 0.000131456, 0.000129824, 0.000130208 ]
a512_512_3 = [ 0.000093408, 0.000092800, 0.000093984, 0.000094048, 0.000094528, 0.000094240 ]

a2048_128_1 = [ 0.001213440, 0.001215872, 0.001215232, 0.001220000, 0.001219680, 0.001219264 ]
a2048_128_2 = [ 0.000132480, 0.000130240, 0.000129792, 0.000128704, 0.000129696, 0.000128832 ]
a2048_128_3 = [ 0.000094464, 0.000094304, 0.000094720, 0.000094432, 0.000093920, 0.000093312 ]

a512_128_1 = [ 0.001260384, 0.001257888, 0.001252736, 0.001253760, 0.001258016, 0.001250464 ]
a512_128_2 = [ 0.000137632, 0.000139808, 0.000137408, 0.000138624, 0.000138784, 0.000139008 ]
a512_128_3 = [ 0.000099104, 0.000099872, 0.000099008, 0.000098016, 0.000099424, 0.000106752 ]

a2048_32_1 = [ 0.001894848, 0.001895008, 0.001895584, 0.001897184, 0.001898560, 0.001896160 ]
a2048_32_2 = [ 0.000220064, 0.000218368, 0.000217472, 0.000218368, 0.000216864, 0.000218400 ]
a2048_32_3 = [ 0.000157856, 0.000155648, 0.000155392, 0.000156096, 0.000155488, 0.000155872 ]

a512_32_1 = [ 0.001987840, 0.001985568, 0.001992512, 0.001989184, 0.001991072, 0.001991712 ]
a512_32_2 = [ 0.000221536, 0.000222944, 0.000220832, 0.000222336, 0.000233984, 0.000221312 ]
a512_32_3 = [ 0.000156160, 0.000157056, 0.000156768, 0.000157088, 0.000156544, 0.000156960 ]

print("t_standart: ", t_standart * m, '\n')

print("2040x512 on 4 500 000 elem: ", round(mean(a2048_512_1) * m, n))
print("2040x512 on 450 000 elem: ", round(mean(a2048_512_2) * m, n))
print("2040x512 on 300 000 elem: ", round(mean(a2048_512_3) * m, n), '\n')

print("512x512 on 4 500 000 elem: ", round(mean(a512_512_1) * m, n))
print("512x512 on 450 000 elem: ", round(mean(a512_512_2) * m, n))
print("512x512 on 300 000 elem: ", round(mean(a512_512_3) * m, n), '\n')

print("2048x128 on 4 500 000 elem: ", round(mean(a2048_128_1) * m, n))
print("2048x128 on 450 000 elem: ", round(mean(a2048_128_2) * m, n))
print("2048x128 on 300 000 elem: ", round(mean(a2048_128_3) * m, n), '\n')

print("512x128 on 4 500 000 elem: ", round(mean(a512_128_1) * m, n))
print("512x128 on 450 000 elem: ", round(mean(a512_128_2) * m, n))
print("512x128 on 300 000 elem: ", round(mean(a512_128_3) * m, n), '\n')

print("2048x32 on 4 500 000 elem: ", round(mean(a2048_32_1) * m, n))
print("2048x32 on 450 000 elem: ", round(mean(a2048_32_2) * m, n))
print("2048x32 on 300 000 elem: ", round(mean(a2048_32_3) * m, n), '\n')

print("512x32 on 4 500 000 elem: ", round(mean(a512_32_1) * m, n))
print("512x32 on 450 000 elem: ", round(mean(a512_32_2) * m, n))
print("5128x32 on 300 000 elem: ", round(mean(a512_32_3) * m, n), '\n')


print("ускорение на 4 500 000 элементах векторов: ")
print(round(t_standart / mean(a2048_512_1), n))
print(round(t_standart / mean(a512_512_1), n))
print(round(t_standart / mean(a2048_128_1), n))
print(round(t_standart / mean(a512_128_1), n))
print(round(t_standart / mean(a2048_32_1), n))
print(round(t_standart / mean(a512_32_1), n))

print("ускорение на 450 000 элементах векторов: ")
print(round(mean(t_second) / mean(a2048_512_2), n))
print(round(mean(t_second) / mean(a512_512_2), n))
print(round(mean(t_second) / mean(a2048_128_2), n))
print(round(mean(t_second) / mean(a512_128_2), n))
print(round(mean(t_second) / mean(a2048_32_2), n))
print(round(mean(t_second) / mean(a512_32_2), n))

print("ускорение на 300 000 элементах векторов: ")
print(round(mean(t_third) / mean(a2048_512_3), n))
print(round(mean(t_third) / mean(a512_512_3), n))
print(round(mean(t_third) / mean(a2048_128_3), n))
print(round(mean(t_third) / mean(a512_128_3), n))
print(round(mean(t_third) / mean(a2048_32_3), n))
print(round(mean(t_third) / mean(a512_32_3), n))

x = [4500000, 450000, 300000]

a2048_512 = [mean(a2048_512_1), mean(a2048_512_2), mean(a2048_512_3)]
a512_512 = [mean(a512_512_1), mean(a512_512_2), mean(a512_512_3)]
a2048_128 = [mean(a2048_128_1), mean(a2048_128_2), mean(a2048_128_3)]
a512_128 = [mean(a512_128_1), mean(a512_128_2), mean(a512_128_3)]
a2048_32 = [mean(a2048_32_1), mean(a2048_32_2), mean(a2048_32_3)]
a512_32 = [mean(a512_32_1), mean(a512_32_2), mean(a512_32_3)]
legend = ['2048x512', '512x512', '2048x128', '512x128', '2048x32', '512x32']
plt.xlabel("N элементов")
plt.ylabel("Время работы, мкс")
plt.plot(x, a2048_512, 'r')
plt.plot(x, a512_512, 'g')
plt.plot(x, a2048_128, 'b')
plt.plot(x, a512_128, 'y')
plt.plot(x, a2048_32, 'pink')
plt.plot(x, a512_32, 'black')
plt.legend(legend, loc="upper left")
plt.grid()
plt.show()

t = [t_standart, mean(t_second), mean(t_third)]
plt.xlabel("N элементов")
plt.ylabel("Ускорение")
plt.plot(x, [t[i] / a2048_512[i] for i in range(3)], 'r')
plt.plot(x, [t[i] / a512_512[i] for i in range(3)], 'g')
plt.plot(x, [t[i] / a2048_128[i] for i in range(3)], 'b')
plt.plot(x, [t[i] / a512_128[i] for i in range(3)], 'y')
plt.plot(x, [t[i] / a2048_32[i] for i in range(3)], 'pink')
plt.plot(x, [t[i] / a512_32[i] for i in range(3)], 'black')
plt.legend(legend, loc="upper left")
plt.grid()
plt.show()