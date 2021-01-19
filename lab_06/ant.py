import numpy as np
from numpy.random import choice as np_choice


def read_input():
    data = []
    while True:
        line = input().split()
        if len(line) == 0:
            break
        data.append(line)
    matrix = np.array(data, float)
    return matrix


class AntColony(object):
    def __init__(self, distances, n_ants, n_best, n_iterations, decay, alpha=1., beta=1.):
        # избавляемся от нулей в матрице
        i = 0
        j = 0
        while i < len(distances):
            while j < len(distances):
                if distances[i][j] == 0:
                    distances[i][j] = np.inf
                    i += 1
                    j += 1
                else:
                    continue
        self.distances = distances #матрица растояний
        self.pheromone = np.ones(self.distances.shape) / len(distances) # данные о феромонах
        self.all_inds = range(len(distances)) # список городов
        self.n_ants = n_ants # колличество муравьев
        self.n_best = n_best # колличество элитных муравьев
        self.n_iterations = n_iterations # колличество итераций
        self.decay = decay # испарения феромона
        self.alpha = alpha
        self.beta = beta

    def run(self):
        all_time_shortest_path = ("placeholder", np.inf)
        for i in range(self.n_iterations):
            all_paths = self.gen_all_paths()
            self.spread_pheronome(all_paths, self.n_best)
            shortest_path = min(all_paths, key=lambda x: x[1])
            if shortest_path[1] < all_time_shortest_path[1]:
                all_time_shortest_path = shortest_path
        return all_time_shortest_path

    def spread_pheronome(self, all_paths, n_best):
        sorted_paths = sorted(all_paths, key=lambda x: x[1])
        for path, dist in sorted_paths[:n_best]:
            for move in path:
                self.pheromone[move] += 1.0 / self.distances[move]

    def gen_path_dist(self, path):
        total_dist = 0
        for ele in path:
            total_dist += self.distances[ele]
        return total_dist

    def gen_all_paths(self):
        all_paths = []
        for i in range(self.n_ants):
            path = self.gen_path(0)
            all_paths.append((path, self.gen_path_dist(path)))
        return all_paths

    def gen_path(self, start):
        path = []
        visited = set()
        visited.add(start)
        prev = start
        for i in range(len(self.distances) - 1):
            move = self.pick_move(self.pheromone[prev], self.distances[prev], visited)
            path.append((prev, move))
            prev = move
            visited.add(move)
        path.append((prev, start)) # going back to where we started
        return path

    def pick_move(self, pheromone, dist, visited):
        pheromone = np.copy(pheromone)
        pheromone[list(visited)] = 0
        row = pheromone ** self.alpha * ((1.0 / dist) ** self.beta)
        norm_row = row / row.sum()
        move = np_choice(self.all_inds, 1, p=norm_row)[0]
        return move


distance = read_input()
ant_colony = AntColony(distance, len(distance)*4, 5, len(distance)*4, 0.95, alpha=1, beta=1)
shortest_path = ant_colony.run()
print (int(shortest_path[1]))