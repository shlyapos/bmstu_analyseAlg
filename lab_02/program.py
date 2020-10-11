import random
from time import *


def inputMatrix():
    row = int(input(">> Input row number: "))
    col = int(input(">> Input column number: "))

    matrix = [[int(j) for j in input("{} row: ".format(i + 1)).split()] for i in range(row)]

    for i in range(len(matrix)):
        if (len(matrix[i]) != col):
            print("Wrong number of elements per line")
            return False
    
    return matrix


def printMatrix(matrix):
    for row in matrix:
        for element in row:
            print("{:4d}".format(element), end="")
        print()
    print("\n")


def generateMatrix(size):
    return [[random.randint(0, 9) for _ in range(size)] 
                                  for _ in range(size)]


def classicMultiplication(A, B, isPrint):
    result = [[0 for j in range(len(A))] for i in range(len(B[0]))]

    for i in range(len(A)):
        for j in range(len(B[0])):
            for k in range(len(A[i])):
                result[i][j] += A[i][k] * B[k][j]
    
    if (isPrint):
        print("\n>>> Result with classic method:")
        printMatrix(result)


def vinogradMultiplication(A, B, isPrint):
    n1, m1 = len(A), len(A[0])
    n2, m2 = len(B), len(B[0])

    mulH = [0 for _ in range(n1)]
    mulV = [0 for _ in range(m2)]

    result = [[0 for j in range(n1)] for i in range(m2)]

    for i in range(n1):
        for j in range(int(m1 / 2)):
            mulH[i] = mulH[i] + A[i][j * 2] * A[i][j * 2 + 1]
        
    for i in range(m2):
        for j in range(int(n2 / 2)):
            mulV[i] = mulV[i] + B[j * 2][i] * B[j * 2 + 1][i]

    for i in range(n1):
        for j in range(m2):
            result[i][j] = -mulH[i] - mulV[j]

            for k in range(int(m1 / 2)):
                result[i][j] = result[i][j] + ((A[i][2 * k + 1] + B[2 * k][j]) * 
                                              (A[i][2 * k] + B[2 * k + 1][j]))
    
    if (m1 % 2):
        for i in range(n1):
            for j in range(m2):
                result[i][j] = result[i][j] + A[i][m1 - 1] * B[m1 - 1][j]
    
    if (isPrint):
        print("\n>>> Result with vinograd method:")
        printMatrix(result)


def vinogradMultiplicationModified(A, B, isPrint):
    n1, m1 = len(A), len(A[0])
    n2, m2 = len(B), len(B[0])

    mulH = [0 for _ in range(n1)]
    mulV = [0 for _ in range(m2)]

    result = [[0 for j in range(n1)] for i in range(m2)]

    m1Mod2 = m1 % 2
    n2Mod2 = n2 % 2

    for i in range(n1):
        for j in range(0, m1 - m1Mod2, 2):
            mulH[i] += A[i][j] * A[i][j + 1]
    
    for i in range(m2):
        for j in range(0, n2 - n2Mod2, 2):
            mulV[i] += B[j][i] * B[j + 1][i]
    
    for i in range(n1):
        for j in range(m2):
            buff = -(mulH[i] + mulV[j])

            for k in range(0, m1 - m1Mod2, 2):
                buff += ((A[i][k + 1] + B[k][j]) * 
                         (A[i][k] + B[k + 1][j]))

            result[i][j] = buff
    
    if m1Mod2:
        m1Min = m1 - 1
        
        for i in range(n1):
            for j in range(m2):
                result[i][j] += A[i][m1Min] * B[m1Min][j]

    if isPrint:
        print("\n>>> Result with modified vinograd method:")
        printMatrix(result)


def doTimeTest(method, A, B):
    t1 = process_time()
    method(A, B, False)
    t2 = process_time()

    return t2 - t1


def multiplicationTimeTest():
    sizesMod2 = [100, 150, 200]
    sizesNotMod2 = [101, 151, 201]

    print("==================================================Even sizes==================================================")
    for size in sizesMod2:
        A = generateMatrix(size)
        B = generateMatrix(size)

        print(">>> For classic with       len = ", size, "Time:", doTimeTest(classicMultiplication, A, B))
        print(">>> For Vinograd with      len = ", size, "Time:", doTimeTest(vinogradMultiplication, A, B))
        print(">>> For Vinograd mod. with len = ", size, "Time:", doTimeTest(vinogradMultiplicationModified, A, B))
    print("==============================================================================================================")
    print("\n")
    print("==================================================Odd sizes===================================================")
    for size in sizesNotMod2:
        A = generateMatrix(size)
        B = generateMatrix(size)

        print(">>> For classic with       len = ", size, "Time:", doTimeTest(classicMultiplication, A, B))
        print(">>> For Vinograd with      len = ", size, "Time:", doTimeTest(vinogradMultiplication, A, B))
        print(">>> For Vinograd mod. with len = ", size, "Time:", doTimeTest(vinogradMultiplicationModified, A, B))
    print("==============================================================================================================")


def main():
    done = False

    while (not done):
        print("Choose method:")
        print("[1] - Classic;")
        print("[2] - Vinograd;")
        print("[3] - Modified vinograd;")
        print("[4] - Time test;")
        print("[0] - Exit.\n")

        try:
            action = int(input(">> Choice: "))
        except:
            print("\n >>> Wrong operation \n")
            continue
            
        if (action == 0):
            done = True
            continue
        elif (action == 4):
            multiplicationTimeTest()
            continue
        else:
            print("\n>> Input for matrix A: ")
            A = inputMatrix()

            if A:
                print("\n>> Input for matrix B")
                B = inputMatrix()

        if (A and B):
            if (action == 1):
                classicMultiplication(A, B, True)
            elif (action == 2):
                vinogradMultiplication(A, B, True)
            elif (action == 3):
                vinogradMultiplicationModified(A, B, True)
            else:
                print("\n >>> Wrong operation \n")


main()