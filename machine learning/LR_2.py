import numpy as np

def getGaussianProbability(x: np.ndarray, x_mean: np.ndarray, x_std:np.ndarray) -> np.ndarray:
    return np.exp(-(x-x_mean)**2/(2*x_std**2))*(1/(np.sqrt(2*np.pi)*x_std))

class BayesianClassifier:
    def __init__(self, dataset:list) -> None:
        x_train = []
        y_train = []
        self.x_test = []
        for i in range(1, len(dataset)):
            data = dataset[i].split(',')
            if data[5][1] == 'r':
                y_train.append(int(data[4]))
                x_train.append([float(a) for a in data[:4]])
            else:
                self.x_test.append([float(a) for a in data[:4]])
        self.x_test = np.array(self.x_test)
        self.n = len(x_train)
        self.train_data = {} # создаем словарь, ключами которого будут названия классов, а значениями - массивы черт
        for i in range(len(y_train)):
            if y_train[i] not in self.train_data.keys():
                self.train_data[y_train[i]] = [x_train[i]]
            else:
                self.train_data[y_train[i]].append(x_train[i])
        for i in self.train_data.keys():
            self.train_data[i] = np.array(self.train_data[i])


    def learn(self) -> None:
        self.mean_per_class = {}
        self.std_per_class = {}
        for i in self.train_data.keys():
            self.mean_per_class[i] = np.mean(self.train_data[i],axis=0)
            self.std_per_class[i] = np.std(self.train_data[i],axis=0)
        
    
    def getProbPerClass(self, x):
        probs = {}
        for i in self.train_data.keys():
            probs[i] = np.prod(getGaussianProbability(x, self.mean_per_class[i], self.std_per_class[i]))
            probs[i] = probs[i] * self.train_data[i].shape[0] / self.n
        max_prob = 0
        class_name = 0
        for k, v in probs.items():
            if v > max_prob:
                max_prob = v
                class_name = k
        return class_name

    def classificate(self) -> list:
        answer = []
        for x in self.x_test:
            answer.append(self.getProbPerClass(x))
        return answer


def main():
    f_input = open('input.txt', 'r')
    f_output = open('output.txt', 'w')
    dataset = f_input.readlines()
    a = BayesianClassifier(dataset)
    a.learn()
    answer = a.classificate()
    for i in answer:
        print(i, file=f_output)
    f_input.close()
    f_output.close() 


if __name__ == "__main__":
    main()