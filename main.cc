#include <cmath>
#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<double>>;

Matrix addMatrices(const Matrix & a, const Matrix & b) {
    // Matrices must have the same dimensions.
    if (a.empty() || a.size() != b.size() || a[0].size() != b[0].size()) return Matrix();
    // Initialize the resultant Matrix with the first entry's number of rows.
    Matrix result(a.size());
    // Loop through each row and column, then add the corresponding entries and insert them in the resultant Matrix.
    for (int r = 0; r < a.size(); ++r) {
        for (int c = 0; c < a[0].size(); ++c) {
            result[r].push_back(a[r][c] + b[r][c]);
        }
    }
    return result;
}

Matrix subtractMatrices(const Matrix & a, const Matrix & b) {
    // Matrices must have the same dimensions.
    if (a.empty() || a.size() != b.size() || a[0].size() != b[0].size()) return Matrix();
    // Initialize the resultant Matrix with the first entry's number of rows.
    Matrix result(a.size());
    // Loop through each row and column, then subtract the corresponding entries and insert them in the resultant Matrix.
    for (int r = 0; r < a.size(); ++r) {
        for (int c = 0; c < a[0].size(); ++c) {
            result[r].push_back(a[r][c] - b[r][c]);
        }
    }
    return result;
}

double multiplicationHelper(int r, int c, int commonExtent, const Matrix & a, const Matrix & b) {
    double productResult = 0.0;
    // It looks at the resultant Matrix's entry that we want to calculate, and makes the proper multiplication of a and b entries.
    for (int k = 0; k < commonExtent; ++k) {
        productResult += a[r][k] * b[k][c];
    }
    return productResult;
}

Matrix multiplyMatrices(const Matrix & a, const Matrix & b) {
    // Number of columns of first Matrix and number of rows of second Matrix must match.
    if (a.empty() || a[0].size() != b.size()) return Matrix();
    // The resultant Matrix should have the number of columns of the first entry, and the number of columns of the second entry.
    Matrix result(a.size());
    int commonExtent = a[0].size();

    for (int r = 0; r < a.size(); ++r) {
        for (int c = 0; c < b[0].size(); ++c) {
            // Call the helper function that will calculate the value for our entry properly.
            result[r].push_back(multiplicationHelper(r, c, commonExtent, a, b));
        }
    }
    return result;
}

Matrix transpose(const Matrix & a) {
    // If the input Matrix is empty, we don't need to do anything, just return a default-constructed Matrix.
    if (a.empty()) return Matrix();
    // Initialize the resultant Matrix to the number of columns of the input Matrix.
    Matrix result(a[0].size());

    for (int r = 0; r < a.size(); ++r) {
        for (int c = 0; c < a[0].size(); ++c) {
            result[c].push_back(a[r][c]);
        }
    }
    return result;
}

double determinantHelper(const Matrix & a) {
    // Base case - the input Matrix is a 2x2 Matrix, and we just calculate the cofactor for it.
    if (a.size() == 2) {
        return a[0][0] * a[1][1] - a[0][1] * a[1][0];
    }
    double determinantResult = 0.0;
    // Initialize a sub-Matrix, which its rows and columns will be reduced by 1.
    int dimensionSize = a.size();
    Matrix subMatrix(dimensionSize - 1);
    for (int r = 0; r < dimensionSize; ++r) {
        int subi = 0;
        for (int c = 1; c < dimensionSize; ++c) {
            int subj = 0;
            for (int k = 0; k < dimensionSize; ++k) {
                if (r == c) continue;
                subMatrix[subi][subj] = a[r][c];
                ++subj;
            }
            ++subi;
        }
        determinantResult += pow(-1, r) * a[0][r] * determinantHelper(subMatrix);
    }
    return determinantResult;
}

double determinant(const Matrix & a) {
    // If it is NOT a square matrix, determinant cannot be calculated.
    if (a.empty() || a.size() != a[0].size()) return -1;
    return determinantHelper(a);
}

void printMatrix(const Matrix & sampleMatrix) {
    for (int r = 0; r < sampleMatrix.size(); ++r) {
        std::cout << "[ ";
        for (int c = 0; c < sampleMatrix[0].size(); ++c) {
            std::cout << " " << sampleMatrix[r][c] << " ";
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

int main() {
    // Initialize first Matrix.
    Matrix a    {{1, 2, 3},
                {3, 5, 6},
                {4, 1, 8}};
    // Initialize second Matrix.
    Matrix b    {{3, 5, 1},
                {4, 7, 2},
                {3, 5, 1}};
    
    // test matrices
    Matrix result = addMatrices(a, b);
    printMatrix(result);

    result = subtractMatrices(a, b);
    printMatrix(result);

    result = multiplyMatrices(a, b);
    printMatrix(result);

    // Initialize a new non-square Matrix.
    Matrix nonsquareMatrix   {{4, 5},
                              {7, 8},
                              {1, 3}};
    result = transpose(nonsquareMatrix);
    printMatrix(result);

    return 0;
}
