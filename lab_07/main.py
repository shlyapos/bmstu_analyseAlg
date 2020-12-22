from faker import Faker
from random import *
from time import process_time


faker = Faker(['ru-RU'])
segmentCount = 6


def fill(dictionary, count):
    for i in range(count):
        tempD = {'key': i, 'address': faker.name()}
        dictionary.append(tempD)
    
    return dictionary


def bruteAlgorithm(dictionary, key):
    for item in dictionary:
       if item['key'] == key:
            return item

    return None


def binaryAlgorithm(dictionary, key):
    minE = 0
    maxE = len(dictionary) - 1
    midE = (minE + maxE) // 2

    if dictionary[minE]['key'] > key:
        return None
    elif dictionary[minE]['key'] == key:
        return dictionary[minE]

    if dictionary[maxE]['key'] < key:
        return None
    elif dictionary[maxE]['key'] == key:
        return dictionary[maxE] 

    # Поиск от среднего значения
    tmp = dictionary[midE]['key']

    while key != tmp:
        if key < tmp:
            maxE = midE
        else:
            minE = midE

        midE = (minE + maxE) // 2
        tmp = dictionary[midE]['key']

    return dictionary[midE]


def prepareSegment(dictionary):
    chance = [[] for i in range(10)]

    for i in range(len(dictionary)):
        chance[dictionary[i]['key'] % 10].append(dictionary[i])
    
    return chance


def segment(chance, key):
    segment = chance[key % 10]
    return binaryAlgorithm(segment, key)


def doTimeTest(size, repeat):
    dictionary = []
    chance = []
    key = 0

    dictionary = fill(dictionary, size)

    t1 = process_time()
    for i in range(repeat):
        bruteAlgorithm(dictionary, key)
        key += 1
    t2 = process_time()

    print("Time for brute -\t", (t2 - t1) / repeat)

    key = 0

    t1 = process_time()
    for i in range(repeat):
        binaryAlgorithm(dictionary, key)
        key += 1
    t2 = process_time()

    print("Time for binary -\t", (t2 - t1) / repeat)


    key = 0

    t1 = process_time()
    chance = prepareSegment(dictionary)
    for i in range(repeat):
        segment(chance, key)
        key += 1
    t2 = process_time()

    print("Time for segment -\t", (t2 - t1) / repeat)


if __name__ == "__main__":
    dictionary = []
    chance = []
 
    length = 10

    dictionary = fill(dictionary, length)

    while True:
        print("[1] - Brute")
        print("[2] - Binary")
        print("[3] - Segment")
        print("[4] - Time test")
        print("[0] - Exit")

        c = int(input("Choice - "))

        if c == 1:
            key = int(input("Input key: "))
            print(bruteAlgorithm(dictionary, key))
        elif c == 2:
            key = int(input("Input key: "))
            print(binaryAlgorithm(dictionary, key))
        elif c == 3:
            chance = prepareSegment(dictionary)

            key = int(input("Input key: "))
            print(segment(chance, key))
        elif c == 4:
            sizes = [100, 1000, 10000, 100000]
            repeats = [1000, 1000, 1000, 1000]

            for i in range(len(sizes)):
                print("\n", sizes[i], 100000)
                doTimeTest(sizes[i], 100000)
        else:
            break