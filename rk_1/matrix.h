#include <vector>
#include <random>

class Matrix
{
private:
    std::vector<std::vector<int>> data;
    int rows, cols;

public:
    Matrix();
    Matrix(int, int);

    void generateMatrix();

    Matrix& operator *(const Matrix&);

    int nrow();
    int ncol();
};