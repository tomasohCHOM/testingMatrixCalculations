#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <iomanip>
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
    // Initialize the resultant Matrix's rows to the number of columns of the input Matrix.
    Matrix result(a[0].size());

    for (int r = 0; r < a.size(); ++r) {
        for (int c = 0; c < a[0].size(); ++c) {
            // If the input matrix has a i-j value, then the result Matrix should make it a j-i value
            // (where i is the row and j is the column).
            // Push back values using c (column) as our new row.
            result[c].push_back(a[r][c]);
        }
    }
    return result;
}

Matrix getSubMatrix(int i, int j, int dimensionSize, const Matrix & a) {
    Matrix resultantMatrix(dimensionSize - 1);
    int m = 0;
    // Loop through rows and columns.
    for (int r = 0; r < dimensionSize; ++r) {
        for (int c = 0; c < dimensionSize; ++c) {
            // If it happens to be the same row, break. If same column, continue to the next iteration.
            if (i == r) break;
            if (j == c) continue;
            // Otherwise, push the value to the subMatrix.
            resultantMatrix[m].push_back(a[r][c]);
        }
        // If added values to the subMatrix successfully, move to the next subMatrix row.
        if (!resultantMatrix[m].empty()) ++m;
    }
    return resultantMatrix;
}

double determinantHelper(const Matrix & a) {
    // If the input Matrix is a 2x2 Matrix, we just calculate the cofactor for it.
    if (a.size() == 2) {
        return a[0][0] * a[1][1] - a[0][1] * a[1][0];
    }
    double determinantResult = 0.0;
    // Initialize the size of the input matrix once and use it everywhere.
    int dimensionSize = a.size();
    for (int r = 0; r < dimensionSize; ++r) {
        // Initialize a sub-Matrix using the getSubMatrix function.
        Matrix subMatrix = getSubMatrix(r, 0, dimensionSize, a);
        // Accumulate the determinant for Matrix a, making a recursive call passing that subMatrix.
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
            if (c != sampleMatrix[0].size() - 1)
                std::cout << std::setprecision(4) << sampleMatrix[r][c] << std::setw(8);
            else
                std::cout << std::setprecision(4) << sampleMatrix[r][c];
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

Matrix rref(const Matrix & a) {
    /*
    if (a.empty()) return a;

    Matrix result = a;
    const int rows = result.size();
    const int cols = result[0].size();
    int lead = 0;
    while (lead < rows) {
        double divisor, multiplier;
        for (int r = 0; r < rows; ++r) {
            divisor = result[lead][lead];
            multiplier = result[r][lead] / divisor;
            for (int c = 0; c < cols; ++c) {
                if (r == lead)
                    result[r][c] /= divisor;
                else
                    result[r][c] -= result[lead][c] * multiplier;
            } 
        }
        ++lead;
        printMatrix(result);
    }
    return result;
    */
    Matrix result = a;
    int lead = 0; // The current leading column
    int rowCount = result.size(); // The number of rows in the matrix
    int colCount = result[0].size(); // The number of columns in the matrix
    for (int r = 0; r < rowCount; r++) { // For each row...
        if (colCount <= lead) { // If we've processed all columns, we're done
            return result;
        }
        int i = r;
        // Search for a row with a non-zero entry in the current leading column
        while (abs(result[i][lead]) < 1e-10) {
            i++;
            if (rowCount == i) { // If we've processed all rows, move to the next column
                i = r;
                lead++;
                if (colCount == lead) { // If we've processed all columns, we're done
                    return result;
                }
            }
        }
        // Swap the current row with the row we found
        std::swap(result[i], result[r]);
        double lv = result[r][lead];
        // Divide the current row by its leading coefficient to make it a leading 1
        for (int j = 0; j < colCount; j++) {
            result[r][j] /= lv;
        }
        // Subtract multiples of the current row from all other rows to make their leading coefficients zero
        for (int i = 0; i < rowCount; i++) {
            if (i != r) {
                double lv = result[i][lead];
                for (int j = 0; j < colCount; j++) {
                    result[i][j] -= lv * result[r][j];
                }
            }
        }
        lead++; // Move to the next leading column
    }
    return result;
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

    std::cout << "The determinant of a non-square matrix: " << determinant(result) << "\n";
    std::cout << "The determinant of Matrix adding both a and b: " << determinant(addMatrices(a, b)) << "\n";

    Matrix constantB {{5},
                      {3},
                      {8}};

    std::array<double, 3> cramersResult = cramersRule(a, constantB);
    std::cout << "Applying Cramer's rule with Matrix a and constantB:" << "\n";
    unsigned int count = 1;
    for (const double & result : cramersResult) {
        std::cout << "x" << count << " = " << result << "\n";
        ++count;
    }

    Matrix testRREF  {{5, -6, -7, 7},
                    {3, -2,  5, -17},
                    {2,  4, -3,  29}};
    
    std::cout << "Testing Reduced Row Echelon Form: \n";
    printMatrix(testRREF);
    testRREF = rref(testRREF);

    return 0;
}
