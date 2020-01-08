// Matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Matrix.h"

using namespace MatrixMath;

int main()
{
    std::cout << "Hello World!" << std::endl;

    Matrix<int, 3, 3> m1{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
    };

    Matrix<int, 3, 3> m2{
        1, 2, 3,
        2, 4, 7,
        3, 7, 14,
    };

    auto m3 = Matrix(m1);

    auto m4 = m3.Transpose();

    IdentityMatrix<int, 5> m5;
    IdentityMatrix<int, 2> m6;
    IdentityMatrix<int, 6> m7;

    m1 *= 40;
    m1 /= 2;
    m1 -= m2;
    m2 += m1;
    m2 = m1 * 3 + m2 / 2 - m1;

    if (m4 == m3)
    {
        std::cout << "m4 == m3" << std::endl;
    }
    else
    {
        std::cout << "m4 != m3" << std::endl;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
