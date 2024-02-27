import matplotlib.pyplot as plt

fin = open('t.txt')
s = fin.read()
a = [float(i) for i in s.split()]
d = [0] * 7
left = min(a)
right = max(a)
delta = (right - left) / 7
b = [left + delta*i for i in range(8)] # отрезки
print(b)
for i in a:
    for j in range(7):
        if (i <= b[j+1]) and (i > b[j]):
            d[j]+=1
#print(d)
d1 = [i / 100. for i in d ]
print(d1)
x = [i for i in range(7)]
fig = plt.figure(figsize=(6, 4))
ax = fig.add_subplot()
ax.bar(x, d1)
plt.show()

fin.close()