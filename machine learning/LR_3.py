import numpy as np

def distance(x1, x2) -> float:
    distance = np.linalg.norm(x1-x2)
    return distance

class kNearestNeighbors:
    def __init__(self, dataset:list, k: int) -> None:
        self.k = k
        self.x_train = []
        self.y_train = []
        self.x_test = []
        for i in range(1, len(dataset)):
            data = dataset[i].split(',')
            if data[5][1] == 'r':
                self.y_train.append(int(data[4]))
                self.x_train.append([float(a) for a in data[:4]])
            else:
                self.x_test.append([float(a) for a in data[:4]])
        
    def classificate(self):
        x = self.x_test
        answer = []
        for i in range(len(x)):
            dist = []
            for j in range(len(self.x_train)):
                dist.append([distance(np.array(x[i]), np.array(self.x_train[j])), self.y_train[j]])
            dist.sort(key=lambda elem: elem[0])
            count = {}
            for i in dist[:self.k]:
                if i[1] in count:
                    count[i[1]] +=1
                else:
                    count[i[1]] = 1
            answer.append(max(count.items(), key=lambda x: x[1])[0])
        return answer

def main():
    f_input = open('input.txt', 'r')
    f_output = open('output.txt', 'w')
    dataset = f_input.readlines()
    a = kNearestNeighbors(dataset, 5)
    answer = a.classificate()
    for i in answer:
        print(i, file=f_output)
    f_input.close()
    f_output.close() 


if __name__ == "__main__":
    main()