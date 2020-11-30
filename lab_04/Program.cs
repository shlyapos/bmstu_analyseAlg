using System;
using System.Diagnostics;

namespace lab_04
{
    class Program
    {
        static Random rand = new Random();

        static void Main(string[] args)
        {
            while (true)
            {
                Console.WriteLine(">> Choose operation: ");
                Console.WriteLine(">> [1] - Standart Vinograd");
                Console.WriteLine(">> [2] - Parallel Vinograd");
                Console.WriteLine(">> [0] - Exit");

                Console.Write("\n>>> Choice: ");
                int choice = Convert.ToInt32(Console.ReadLine());

                if (choice == 0)
                {
                    break;
                }
                else if (choice == 3)
                {

                }
                else
                {
                    Console.WriteLine("\n>> For matrix A:");
                    Matrix A = InitMatrix();

                    Console.WriteLine("\n>> For matrix B:");
                    Matrix B = InitMatrix();

                    Matrix C = new Matrix(A.N, B.M);

                    switch (choice)
                    {
                        case 1:
                            C = A * B;

                            Console.WriteLine("\n>> Result: ");
                            PrintMatrix(C);
                            Console.WriteLine();

                            break;
                        case 2:
                            Console.Write(">> Input thred number: ");
                            int threadCount = Convert.ToInt32(Console.ReadLine());

                            C.ParallelMult(A, B, threadCount);

                            Console.WriteLine("\n>> Result: ");
                            PrintMatrix(C);
                            Console.WriteLine();

                            break;
                        default:
                            Console.WriteLine(">>> Wrong operation number, try again...\n");
                            break;
                    }
                }
            }
        }


        // For matrix
        static Matrix InitMatrix()
        {
            Matrix newMatrix;

            Console.Write(">>> Input row number: ");
            int n = Convert.ToInt32(Console.ReadLine());

            Console.Write(">>> Input column number: ");
            int m = Convert.ToInt32(Console.ReadLine());

            if (n < 0 || m < 0)
            {
                newMatrix = null;
            }
            else
            {
                newMatrix = new Matrix(n, m);

                Console.WriteLine("\n>> Input elements: ");
                InputMatrix(ref newMatrix);
                //Console.WriteLine();
            }

            return newMatrix;
        }

        static void PrintMatrix(Matrix matrix)
        {
            for (int i = 0; i < matrix.N; i++)
            {
                for (int j = 0; j < matrix.M; j++)
                {
                    Console.Write(matrix[i, j] + "\t");
                }

                Console.WriteLine();
            }
        }

        static void InputMatrix(ref Matrix matrix)
        {
            for (int i = 0; i < matrix.N; i++)
            {
                for (int j = 0; j < matrix.M; j++)
                {
                    Console.Write(">>> ");
                    matrix[i, j] = Convert.ToInt32(Console.ReadLine());
                }
                Console.WriteLine();
            }
        }

        static Matrix GenerateMatrix(int n, int m)
        {
            Matrix newMatrix = new Matrix(n, m);

            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    newMatrix[i, j] = rand.Next(1000);
                }
            }

            return newMatrix;
        }


        // Time tests
        static void DoTimeTest()
        {
            Stopwatch stopWatch = new Stopwatch();
        }
    }
}
