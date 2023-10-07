import numpy as np

def sigmoid(z):
    return 1. / (1. + np.eps(z))

class LinearRegression:
    def __init__(self, dataset: list) -> None:
        self.x_train = []
        self.y_train = []
        self.x_test = []
        self.y_test = []
        self.w = []
        for i in range(1, len(dataset)):
            data = dataset[i].split(',')
            if data[5][1] == 'r':
                self.y_train.append(int(data[4]))
                x = [1.] 
                for j in range(4):
                    x.append(float(data[j]))
                self.x_train.append(x)
            else:
                self.y_test.append(int(data[4]))
                x = [1.]
                for j in range(4):
                    x.append(float(data[j]))
                self.x_test.append(x)

    def learn(self) -> None:
        X = np.array(self.x_train)
        Y = np.array(self.y_train)
        W = np.zeros(len(self.x_train[0]))
        def loss_func(W):
            L = 0
            for i in range(len(W)):
                L += Y[i]*np.log(1. / (1 + np.exp(-X[i]*W[i]))) + (1 - Y[i])*np.log(1. / (1 + np.exp(X[i]*W[i])))
            return -sum(L)




    def classificate(self) -> list():
        X = np.array(self.x_test)
        z = 1 / (1 + np.exp(-( X.dot(self.w)))) 
        print(z)
        for i in range(len(z)):
            self.y_test[i] = 1 if z[i] > 0.5 else 0 
        return self.y_test

    def __str__(self) -> str:
        print("x_train = ", self.x_train)
        print("y_train = ", self.y_train)
        print("x_test = ", self.x_test)
        print("y_test = ", self.y_test)
        return ' '



def main():
    f_input = open('input.txt', 'r')
    f_output = open('output.txt', 'w')

    dataset = f_input.readlines()
    a = LinearRegression(dataset)
    a.learn()
    answer = a.classificate()
    for i in answer:
        print(i, file=f_output)
    f_input.close()
    f_output.close()    


if __name__ == '__main__':
    main()