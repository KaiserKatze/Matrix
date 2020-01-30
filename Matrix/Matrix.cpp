// Matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Matrix.h"

using namespace MatrixMath;

int main()
{
    std::cout << "Hello World!" << std::endl;

    MatrixMath::Matrix<int, 3, 3> m1{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
    };

    std::cout << "m1 = " << std::endl
        << m1.ToString();

    if (m1.GetData(0, 0) == 1
        && m1.GetData(0, 1) == 0
        && m1.GetData(0, 2) == 0
        && m1.GetData(1, 0) == 0
        && m1.GetData(1, 1) == 1
        && m1.GetData(1, 2) == 0
        && m1.GetData(2, 0) == 0
        && m1.GetData(2, 1) == 0
        && m1.GetData(2, 2) == 1)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;

    std::cout << std::endl;

    //================================================

    MatrixMath::Matrix<int, 3, 3> m2{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
    };

    std::cout << "m2 = " << std::endl
        << m2.ToString();

    if (m2.GetData(0, 0) == 1
        && m2.GetData(0, 1) == 2
        && m2.GetData(0, 2) == 3
        && m2.GetData(1, 0) == 4
        && m2.GetData(1, 1) == 5
        && m2.GetData(1, 2) == 6
        && m2.GetData(2, 0) == 7
        && m2.GetData(2, 1) == 8
        && m2.GetData(2, 2) == 9)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;

    std::cout << "m1 != m2 ";
    if (m1 != m2)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    
    std::cout << std::endl;

    //================================================

    auto m3 = MatrixMath::Matrix(m1);

    std::cout << "m3 = " << std::endl
        << m3.ToString();

    if (m3.GetData(0, 0) == 1
        && m3.GetData(0, 1) == 0
        && m3.GetData(0, 2) == 0
        && m3.GetData(1, 0) == 0
        && m3.GetData(1, 1) == 1
        && m3.GetData(1, 2) == 0
        && m3.GetData(2, 0) == 0
        && m3.GetData(2, 1) == 0
        && m3.GetData(2, 2) == 1)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;

    std::cout << "m1 == m2 ";
    if (m1 == m3)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;

    std::cout << std::endl;

    //================================================

    auto m4 = m3.Transpose();

    std::cout << "m4 = " << std::endl
        << m4.ToString();

    if (m4.GetData(0, 0) == 1
        && m4.GetData(0, 1) == 0
        && m4.GetData(0, 2) == 0
        && m4.GetData(1, 0) == 0
        && m4.GetData(1, 1) == 1
        && m4.GetData(1, 2) == 0
        && m4.GetData(2, 0) == 0
        && m4.GetData(2, 1) == 0
        && m4.GetData(2, 2) == 1)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;

    std::cout << std::endl;

    //================================================

    IdentityMatrix<int, 4> m5;

    std::cout << "m5 = " << std::endl
        << m5.ToString() << std::endl;

    //================================================

    IdentityMatrix<int, 6> m6;

    std::cout << "m6 = " << std::endl
        << m6.ToString() << std::endl;

    //================================================

    m1 *= 40;

    std::cout << "m1 = " << std::endl
        << m1.ToString() << std::endl;

    //================================================

    m1 /= 2;

    std::cout << "m1 = " << std::endl
        << m1.ToString() << std::endl;

    //================================================

    m1 -= m2;

    std::cout << "m1 = " << std::endl
        << m1.ToString() << std::endl;

    //================================================

    m2 += m1;

    std::cout << "m2 = " << std::endl
        << m2.ToString() << std::endl;

    //================================================

    m2 = m1 * 3 + m2 / 2 - m1;

    std::cout << "m2 = " << std::endl
        << m2.ToString() << std::endl;

    //================================================

    Scalar<int> s1 = 1;

    //================================================

    MatrixMath::MatrixQ<int, 3> m9 = s1 * m1 + m1 * s1;

    //================================================

    int i1 = s1 * 2 + 2 * s1;

    std::cout << "i1 = " << i1 << std::endl;
    if (i1 == 4)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

    //================================================

    s1 = 100;

    std::cout << "s1 = " << s1 << std::endl;
    if (s1 == 100)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

    //================================================

    int i2 = s1.Transpose();

    std::cout << "i2 = " << i2 << std::endl;
    if (i2 == 100)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

    //================================================

    Scalar<int> s2{ 100 };
    int i3 = s2;

    std::cout << "i3 = " << i3 << std::endl;
    if (i3 == 100)
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

    //================================================

    MatrixMath::Matrix4f<> m4f{ IdentityMatrix<float, 4>() };

    //================================================

    MatrixMath::Matrix<int, 2, 3> mi23 {
        1, 2, 3,
        3, 2, 1,
    };
    MatrixMath::Matrix<int, 3, 4> mi34{
        1, 2, 3, 4,
        1, 2, 3, 4,
        1, 2, 3, 4,
    };

    auto mi24{ mi23 * mi34 };

    std::cout
        << "mi23 = " << std::endl
        << mi23.ToString();
    if (mi23.GetData() == std::array<int, 6>{1, 2, 3, 3, 2, 1})
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

    std::cout
        << "mi34 = " << std::endl
        << mi34.ToString();
    if (mi34.GetData() == std::array<int, 12>{ 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, })
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

    std::cout
        << "mi24 = mi23 * mi34 = " << std::endl
        << mi24.ToString();
    if (mi24.GetData() == std::array<int, 8>{ 6, 12, 18, 24, 6, 12, 18, 24, })
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

    MatrixMath::Matrix<int, 2, 3, StorageOrder::ColumnMajor> mi23c {
        1, 2, 3,
        3, 2, 1,
    };
    MatrixMath::Matrix<int, 3, 2, StorageOrder::ColumnMajor> mi32c{
        1, 2,
        2, 2,
        3, 2,
        1, // intended buffer overflow attack
    };
    auto mi22c{ mi23c * mi32c };

    std::cout
        << "mi23c = " << std::endl
        << mi23c.ToString();
    if (mi23c.GetData() == std::array<int, 6>{ 1, 2, 3, 3, 2, 1, })
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

    std::cout
        << "mi32c = " << std::endl
        << mi32c.ToString();
    if (mi32c.GetData() == std::array<int, 6>{ 1, 2, 2, 2, 3, 2, })
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

    std::cout
        << "mi22c = mi23c * mi32c = " << std::endl
        << mi22c.ToString();
    if (mi22c.GetData() == std::array<int, 4>{ 14, 12, 10, 12 })
        std::cout << "Succeed" << std::endl;
    else
        std::cout << "Failed" << std::endl;
    std::cout << std::endl;

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
