import numpy as np


class LogicRegression:
    def __init__(self, dataset: list) -> None:
        # данные предоставляются в формате х1, х2, х3, х4, у, пометка о сете (train/test)
        self.x_train = []
        self.y_train = []
        self.x_test = []
        self.y_test = []
        self.b = 0
        self.learning_rate = 0.1        
        self.iterations = 100
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
        self.w = np.zeros(len(self.x_train[0]))

    def learn(self) -> None:
        X = np.array(self.x_train) 
        Y = np.array(self.y_train) 
        rows = len(self.x_train) 
        for i in range(self.iterations) :            
            A = 1 / (1 + np.exp(-(X.dot(self.w) + self.b)))
            loss = (A - Y.T)        
            loss = np.reshape(loss, rows)        
            dW = np.dot(X.T, loss) / rows         
            db = np.sum(loss) / rows 
            self.w = self.w - self.learning_rate * dW    
            self.b = self.b - self.learning_rate * db

    def classificate(self) -> list():
        X = np.array(self.x_test)
        z = 1 / (1 + np.exp(-( X.dot(self.w) + self.b))) 
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
    a = LogicRegression(dataset)
    print(a.learn())
    answer = a.classificate()
    for i in answer:
        print(i, file=f_output)
    f_input.close()
    f_output.close()    


if __name__ == '__main__':
    main()