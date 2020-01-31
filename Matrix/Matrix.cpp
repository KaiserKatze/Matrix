// Matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Matrix.h"
#include "Geometry.h"

int main()
{
    // Test: Scalar initialization
    MatrixMath::Scalar<int> s1;
    MatrixMath::Scalar<int> s2{ 100 };
    MatrixMath::Scalar<int> s3(s2);

    std::cout
        << "s1 -> " << s1 << std::endl
        << "s2 -> " << s2 << std::endl
        << "s3 -> " << s3 << std::endl;

    // Test: Scalar user-defined conversion
    int is1{ s2 };

    // Test: Scalar multiplication
    s2 *= s2;
    s3 *= 20;

    std::cout
        << "s2 = (s2 * s2) -> " << s2 << std::endl
        << "s3 = (s3 * 20) -> " << s3 << std::endl;

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
