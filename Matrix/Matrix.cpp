// Matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Matrix.h"
#include "Geometry.h"

int main()
{
    //==============================================
    // Scalar

    // Test: Scalar initialization
    MatrixMath::Scalar<int> s1;
    MatrixMath::Scalar<int> s2{ 100 };
    MatrixMath::Scalar<int> s3(s2);

    std::cout
        << "s1 -> " << s1 << " "
        << (s1 == 0 ? "[Succeed]" : "[Fail]")
        << std::endl
        << "s2 -> " << s2 << " "
        << (s2 == 100 ? "[Succeed]" : "[Fail]")
        << std::endl
        << "s3 -> " << s3 << " "
        << (s3 == s2 ? "[Succeed]" : "[Fail]")
        << std::endl;

    // Test: Scalar data direct access
    s1.SetElement(1453);
    std::cout << "s1.SetElement(1453) == 1453 -> "
        << (s1 == 1453 ? "[Succeed]" : "[Fail]")
        << std::endl;

    // Test: Scalar user-defined conversion
    int is1{ s2 };

    // Test: Scalar calculation
    s1 += 12;
    std::cout << "s1 += 12 == 1465 -> "
        << (s1 == 1465 ? "[Succeed]" : "[Fail]")
        << std::endl;

    s1 -= 45;
    std::cout << "s1 -= 44 == 1420 -> "
        << (s1 == 1420 ? "[Succeed]" : "[Fail]")
        << std::endl;

    s1 /= 2;
    std::cout << "s1 /= 2 == 710 -> "
        << (s1 == 710 ? "[Succeed]" : "[Fail]")
        << std::endl;

    s1 = -s2;
    std::cout << "s1 = -s2 -> -100 -> "
        << (s1 == -100 ? "[Succeed]" : "[Fail]")
        << std::endl;

    s2 *= s2;
    std::cout
        << "s2 = (s2 * s2) -> " << s2
        << " " << (s2 == 10000 ? "[Succeed]" : "[Fail]")
        << std::endl;

    s3 *= 20;
    std::cout
        << "s3 = (s3 * 20) -> " << s3
        << " " << (s3 == 2000 ? "[Succeed]" : "[Fail]")
        << std::endl;

    std::cout
        << "s1 == s1.Transpose() -> "
        << (s1 == s1.Transpose() ? "[Succeed]" : "[Fail]")
        << std::endl;

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
