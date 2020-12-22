#include "matrix.h"

Matrix::Matrix() 
    : data(std::vector<std::vector<int>>()), 
      rows(0), cols(0)   
{

}

Matrix::Matrix(int nrow, int ncol) 
    : data(std::vector<std::vector<int>>(nrow, std::vector<int>(ncol, 0))), 
      rows(nrow), cols(ncol)
{

}


void Matrix::generateMatrix()
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            data[i][j] = rand() % 10;
}


Matrix& Matrix::operator *(const Matrix& m)
{

}


int Matrix::nrow()
{
    return rows;
}

int Matrix::ncol()
{
    return cols;
}