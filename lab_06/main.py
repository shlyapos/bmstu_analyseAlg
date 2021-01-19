import random
import pprint
import numpy as np
import math


def create_mart(n, a=1, b=9):
    res = []
    for i in range(n):
        tmp = []
        for j in range(i):
            tmp.append(res[j][i])
        for j in range(1):
            tmp.append(np.inf)
        for j in range(n - 1 - i):
            tmp.append(random.randint(a, b))
        res.append(tmp)
    matrix = np.array(res, float)
    return matrix


def brute(dist):
    n = len(dist)

    for i in range(n):
        for j in range(len(dist[i])):
            if dist[i][j] == 0:
                dist[i][j] = np.inf

    iteration = math.factorial(n)
    res = []

    for i in range(iteration):
        tmp = []
        elem = [i for i in range(n)]

        for j in range(n):
            a = math.factorial(n - j - 1)
            index = ((i // a) % (n - j))
            tmp.append(elem[index])
            elem.pop(index)
        res.append(tmp)

    shortcut = ("placeholder", np.inf)
    for path in res:
        tmp = dist[path[0]][path[-1]]
        for i in range(1, len(path)):
            tmp += dist[path[i - 1]][path[i]]
        if tmp < shortcut[1]:
            shortcut = (path, tmp)
    return shortcut


class Ant:
    def __init__(self, dist, ant, elite, iteration, decay, alpha=1., beta=1.):
        self.dist = dist
        self.ant = ant
        self.elite = elite
        self.decay = decay
        self.alpha = alpha
        self.beta = beta
        self.iteration = iteration

        self.pheromone = np.ones(dist.shape) / len(dist)
        self.cities = range(len(dist))

    def run(self):
        shortcut = ("placeholder", np.inf)
        for i in range(self.iteration):
            all_paths = self.gen_paths()
            self.update_pheromone(all_paths, self.elite)
            tmp = min(all_paths, key=lambda x: x[1])
            if tmp[1] < shortcut[1]:
                shortcut = tmp
            self.pheromone = self.pheromone * self.decay
        return shortcut

    def update_pheromone(self, all_paths, elite):
        sorted_paths = sorted(all_paths, key=lambda x: x[1])
        for path, len in sorted_paths[:elite]:
            for move in path:
                self.pheromone[move] += 1.0 / self.dist[move]

    def probe_move(self, pheromone, dist, visited):
        tmp = np.copy(pheromone)
        for i in range(len(tmp)):
            if i in visited:
                tmp[i] = 0
        chance = tmp ** self.alpha * ((1.0 / dist) ** self.beta)
        chance = chance / chance.sum()
        move = np.random.choice(self.cities, 1, p=chance)[0]
        return move

    def gen_path(self, start):
        path = []
        visited = set()
        visited.add(start)
        prev = start
        for i in range(len(self.dist) - 1):
            move = self.probe_move(self.pheromone[prev], self.dist[prev], visited)
            path.append((prev, move))
            prev = move
            visited.add(move)
        path.append((prev, start))
        return path

    def path_len(self, path):
        len = 0
        for city in path:
            len += self.dist[city]
        return len

    def gen_paths(self):
        all_paths = []
        # Размещаем муравьев в города
        for i in range(self.ant):
            path = self.gen_path(0)
            all_paths.append((path, self.path_len(path)))
        return all_paths


n = 5
err = 0
a = create_mart(n)
print(a)
ant = Ant(a, n * 2, n // 2, 100, 0.95, 1, 1)
ant_run = ant.run()
brute_res = brute(a)
print(brute_res)

for iteration in range(1, n * 4):
    a = create_mart(n)
    ant = Ant(a, n * 2, n // 2, iteration, 0.95, 1, 1)
    ant_run = ant.run()
    brute_res = brute(a)
    if ant_run[1] != brute_res[1]:
        err += 1
        print(iteration)
print(err)




# for all_ant in range(1, n):
#     for elite in range(all_ant):
#         for iteration in range(1, n * 4):
#             for decay in range(20):
#                 for alpha in range(20):
#                     for beta in range(20):
#                         a = create_mart(n)
#                         ant = Ant(a, all_ant - elite, elite, iteration, decay, alpha * 0.05, beta * 0.05)