#include <array>
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

Matrix getSubMatrix(int i, int j, int dimensionSize, const Matrix & a) {
    Matrix resultantMatrix(dimensionSize - 1);
    int m = 0;
    for (int r = 0; r < dimensionSize; ++r) {
        for (int c = 0; c < dimensionSize; ++c) {
            if (i == r) break;
            if (j == c) continue;
            resultantMatrix[m].push_back(a[r][c]);
        }
        if (!resultantMatrix[m].empty()) ++m;
    }
    return resultantMatrix;
}

double determinantHelper(const Matrix & a) {
    // If the input Matrix is a 2x2 Matrix, and we just calculate the cofactor for it.
    if (a.size() == 2) {
        return a[0][0] * a[1][1] - a[0][1] * a[1][0];
    }
    double determinantResult = 0.0;
    // Initialize the size of the input matrix once and use it everywhere.
    int dimensionSize = a.size();
    for (int r = 0; r < dimensionSize; ++r) {
        // Initialize a sub-Matrix using the getSubMatrix function.
        Matrix subMatrix = getSubMatrix(r, 0, dimensionSize, a);
        determinantResult += pow(-1, r) * a[r][0] * determinantHelper(subMatrix);
    }
    return determinantResult;
}

double determinant(const Matrix & a) {
    // If it is NOT a square matrix, determinant cannot be calculated.
    if (a.empty() || (a.size() != a[0].size())) return -1;
    // If only one entry, then we can just return the value of that entry.
    if (a.size() == 1) return a[0][0];
    return determinantHelper(a);
}

std::array<double, 3> cramersRule(const Matrix & a, const Matrix & b) {
    std::array<double, 3> result;
    double mainDeterminant = determinant(a);

    for (int c = 0; c < a.size(); ++c) {
        Matrix temp = a;
        for (int r = 0; r < a.size(); ++r) {
            temp[r][c] = b[r][0];
        }
        result[c] = determinant(temp) / mainDeterminant;
    }
    return result;
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

    std::cout << "The determinant of Matrix a: " << determinant(a) << "\n";


    return 0;
}
