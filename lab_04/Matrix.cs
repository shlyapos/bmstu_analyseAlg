using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace lab_04
{
    class Matrix
    {
        private int[,] data;

        private int n;
        public int N { get => this.n; }

        private int m;
        public int M { get => this.m; }

        public Matrix(int n, int m)
        {
            this.n = n;
            this.m = m;
            this.data = new int[n, m];
        }

        public int this[int x, int y]
        {
            get 
            { 
                return this.data[x, y]; 
            }
            set 
            { 
                this.data[x, y] = value;  
            }
        }

        public static Matrix operator* (Matrix A, Matrix B)
        {
            int n1 = A.N;
            int n2 = B.N;

            if (n1 == 0 || n2 == 0)
            {
                return null;
            }

            int m1 = A.M;
            int m2 = B.M;

            int[] mulH = new int[n1];
            int[] mulV = new int[m2];

            Matrix C = new Matrix(n1, m2);

            for (int i = 0; i < n1; i++)
            {
                for (int j = 0; j < m1 / 2; j++)
                {
                    mulH[i] += A[i, j * 2] * A[i, j * 2 + 1];
                }
            }

            for (int i = 0; i < m2; i++)
            {
                for (int j = 0; j < n2 / 2; j++)
                {
                    mulV[i] += B[j * 2, i] * B[j * 2 + 1, i];
                }
            }

            for (int i = 0; i < n1; i++)
            {
                for (int j = 0; j < m2; j++)
                {
                    C[i, j] = -mulH[i] - mulV[j];

                    for (int k = 0; k < m1 / 2; k++)
                    {
                        C[i, j] += (A[i, 2 * k + 1] + B[2 * k, j]) * 
                                   (A[i, 2 * k] + B[2 * k + 1, j]);
                    }
                }
            }

            if (m1 % 2 == 1)
            {
                for (int i = 0; i < n1; i++)
                {
                    for (int j = 0; j < m2; j++)
                    {
                        C[i, j] += A[i, m1 - 1] * B[m1 - 1, j];
                    }
                }
            }

            return C;
        }
        

        // For parallel vinograd method
        private class ParallelMultParameters
        {
            public Matrix matrix;
            public int[] mul;
            public int st;
            public int end, end2;

            public ParallelMultParameters(Matrix matrix, int[] mul, int st, int end, int end2)
            {
                this.matrix = matrix;
                this.mul = mul;

                this.st = st;

                this.end = end;
                this.end2 = end2;
            }
        }

        private class ParallelMultAllParameters
        {
            public Matrix A, B, C;
            public int[] mulV, mulH;
            public int st;
            public int end, end2, end3;

            public ParallelMultAllParameters(Matrix C, Matrix A, Matrix B, int[] mulV, int[] mulH, int st, int end, int end2, int end3)
            {
                this.A = A;
                this.B = B;
                this.C = C;

                this.mulV = mulV;
                this.mulH = mulH;

                this.st = st;

                this.end = end;
                this.end2 = end2;
                this.end3 = end3;
            }
        }

        private static void ParallelMultMainCycle(object obj)
        {
            ParallelMultAllParameters p = (ParallelMultAllParameters)obj;

            Matrix C = p.C;
            Matrix A = p.A;
            Matrix B = p.B;

            int[] mulH = p.mulH;
            int[] mulV = p.mulV;

            int st = p.st;

            int end = p.end;
            int end2 = p.end2;
            int end3 = p.end3;

            for (int i = st; i < end; i++)
            {
                for (int j = 0; j < end2; j++)
                {
                    C[i, j] = -mulH[i] - mulV[j];

                    for (int k = 0; k < end3 / 2; k++)
                    {
                        C[i, j] += (A[i, 2 * k + 1] + B[2 * k, j]) * (A[i, 2 * k] + B[2 * k + 1, j]);
                    }
                }
            }
        }

        //public Matrix ParallelMult(Matrix A, Matrix B, int threadCount)
        public bool ParallelMult(Matrix A, Matrix B, int threadCount)
        {
            int n1 = A.N;
            int n2 = B.N;

            if (n1 == 0 || n2 == 0)
            {
                return false;
            }

            int m1 = A.M;
            int m2 = A.M;

            if (m1 != n2)
            {
                return false;
            }

            int[] mulH = new int[n1];
            int[] mulV = new int[m2];

            //Matrix C = new Matrix(n1, m2);

            Thread[] t = new Thread[threadCount];

            for (int i = 0; i < n1; i++)
            {
                for (int j = 0; j < m1 / 2; j++)
                {
                    mulH[i] += A[i, j * 2] * A[i, j * 2 + 1];
                }
            }

            for (int i = 0; i < n1; i++)
            {
                for (int j = 0; j < m1 / 2; j++)
                {
                    mulV[i] += B[j * 2, i] * B[j * 2 + 1, i];
                }
            }

            int rowsForThread = n1 / threadCount;
            int n1St = 0;

            for (int i = 0; i < threadCount; i++)
            {
                int n1End = n1St + rowsForThread;

                if (i == threadCount - 1)
                {
                    n1End = n1;
                }

                //ParallelMultAllParameters p = new ParallelMultAllParameters(C, A, B, mulV, mulH, n1St, n1End, m2, m1);
                ParallelMultAllParameters p = new ParallelMultAllParameters(this, A, B, mulV, mulH, n1St, n1End, m2, m1);

                t[i] = new Thread(ParallelMultMainCycle);
                t[i].Start(p);

                n1St = n1End;
            }

            foreach (Thread thread in t)
            {
                thread.Join();
            }

            if (m1 % 2 == 1)
            {
                for (int i = 0; i < n1; i++)
                {
                    for (int j = 0; j < m2; j++)
                    {
                        this.data[i, j] += A[i, m1 - 1] * B[m1 - 1, j];
                    }
                }
            }

            return true;
        }
    }
}
