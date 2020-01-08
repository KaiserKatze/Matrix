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

    std::cout << "m1 = " << m1.ToString() << std::endl;

    Matrix<int, 3, 3> m2{
        1, 2, 3,
        2, 4, 7,
        3, 7, 14,
    };

    std::cout << "m2 = " << m2.ToString() << std::endl;

    auto m3 = Matrix(m1);

    std::cout << "m3 = " << m3.ToString() << std::endl;

    auto m4 = m3.Transpose();

    std::cout << "m4 = " << m4.ToString() << std::endl;

    IdentityMatrix<int, 4> m5;

    std::cout << "m5 = " << m5.ToString() << std::endl;

    IdentityMatrix<int, 6> m6;

    std::cout << "m6 = " << m6.ToString() << std::endl;

    m1 *= 40;

    std::cout << "m1 = " << m1.ToString() << std::endl;

    m1 /= 2;

    std::cout << "m1 = " << m1.ToString() << std::endl;

    m1 -= m2;

    std::cout << "m1 = " << m1.ToString() << std::endl;

    m2 += m1;

    std::cout << "m2 = " << m2.ToString() << std::endl;

    m2 = m1 * 3 + m2 / 2 - m1;

    std::cout << "m2 = " << m2.ToString() << std::endl;

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
