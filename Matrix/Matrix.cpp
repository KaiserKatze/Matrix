// Matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>

#include "Matrix.h"
#include "Geometry.h"

#ifdef _DEBUG
#   define SET_DEBUG_NAME(var)     var.name = #var
#else
#   define SET_DEBUG_NAME(var)
#endif
#define ACTIVATE_MATRIX_TEST 1

int main()
{
    //==============================================
    // Matrix
#if ACTIVATE_MATRIX_TEST
    {
        MatrixMath::Matrix2i<> m2i1{
            1, 2,
            3, 4,
        };
        SET_DEBUG_NAME(m2i1);
        std::cout
            << "m2i1 = " << std::endl
            << m2i1.ToString()
            << "isTransposed: "
            << std::boolalpha << m2i1.IsTransposed() << std::noboolalpha
            << std::endl
            << std::endl;

        MatrixMath::Matrix2i<> m2i2(m2i1);
        SET_DEBUG_NAME(m2i2);
        std::cout
            << "m2i2 = " << std::endl
            << m2i2.ToString()
            << "isTransposed: "
            << std::boolalpha << m2i2.IsTransposed() << std::noboolalpha
            << std::endl
            << std::endl;

        MatrixMath::Matrix2i<> m2i3 = std::move(m2i2);
        SET_DEBUG_NAME(m2i3);
        std::cout
            << "m2i3 = " << std::endl
            << m2i3.ToString()
            << "isTransposed: "
            << std::boolalpha << m2i3.IsTransposed() << std::noboolalpha
            << std::endl
            << std::endl;

        typename decltype(m2i3)::Transposed m2i3t = m2i3.Transpose();
        std::cout
            << "m2i3.Transpose(); <--" << std::endl
            << "m2i3t = " << std::endl
            << m2i3t.ToString()
            << "isTransposed: "
            << std::boolalpha << m2i3t.IsTransposed() << std::noboolalpha
            << std::endl
            << std::endl;

        m2i3.SetElement(0, 0, 0);
        m2i3.SetElement(0, 1, 0);
        std::cout
            << "m2i3.SetElement(0, 0, 0); <--" << std::endl
            << "m2i3.SetElement(0, 1, 0); <--" << std::endl
            << "m2i3 = " << std::endl
            << m2i3.ToString()
            << "isTransposed: "
            << std::boolalpha << m2i3.IsTransposed() << std::noboolalpha
            << std::endl
            << std::endl;
        std::cout
            << "m2i2 = " << std::endl
            << m2i2.ToString()
            << "isTransposed: "
            << std::boolalpha << m2i2.IsTransposed() << std::noboolalpha
            << std::endl
            << std::endl;
        std::cout
            << "m2i1 = " << std::endl
            << m2i1.ToString()
            << "isTransposed: "
            << std::boolalpha << m2i1.IsTransposed() << std::noboolalpha
            << std::endl
            << std::endl;
    }
#endif

    //==============================================
    // SubMatrix
#if ACTIVATE_MATRIX_TEST
    {
        MatrixMath::Matrix4i<> m4i1{
            1, 2, 3, 4,
            0, 1, 2, 3,
            0, 0, 1, 2,
            0, 0, 0, 1,
        };
        std::cout
            << "m4i1 = "
            << std::endl
            << m4i1.ToString()
            << std::endl;

        auto m4i1c1 = m4i1.GetSubMatrix<0, 2, 0, 2>();
        auto m4i1c2 = m4i1.GetSubMatrix<0, 2, 2, 4>();
        auto m4i1c3 = m4i1.GetSubMatrix<2, 4, 0, 2>();
        auto m4i1c4 = m4i1.GetSubMatrix<2, 4, 2, 4>();

        std::cout
            << "m4i1c1 = "
            << std::endl
            << m4i1c1.ToString()
            << std::endl

            << "m4i1c2 = "
            << std::endl
            << m4i1c2.ToString()
            << std::endl

            << "m4i1c3 = "
            << std::endl
            << m4i1c3.ToString()
            << std::endl

            << "m4i1c4 = "
            << std::endl
            << m4i1c4.ToString()
            << std::endl;

        m4i1c1.SetElement(0, 1, 9);
        std::cout
            << "m4i1c1.SetElement(0, 1, 9); <--" << std::endl
            << std::endl

            << "m4i1c1 = "
            << std::endl
            << m4i1c1.ToString()
            << std::endl

            << "m4i1 = "
            << std::endl
            << m4i1.ToString()
            << std::endl;
    }
#endif

    //==============================================
    // Vector
#if ACTIVATE_MATRIX_TEST
    {
        // Test: Vector initialization
        MatrixMath::Vector4f<> v4f1;
        SET_DEBUG_NAME(v4f1);
        MatrixMath::Vector4i<> v4i1{
            1, 2, 3, 4,
            5, // intentional overflow
        };
        SET_DEBUG_NAME(v4i1);

        std::cout
            << "v4i1 =" << std::endl
            << v4i1.ToString()
            << std::endl
            << " -> "
            << (v4i1.GetElement(0) == 1
                && v4i1.GetElement(1) == 2
                && v4i1.GetElement(2) == 3
                && v4i1.GetElement(3) == 4
                ? "[Succeed]" : "[Fail]")
            << std::endl
            << std::endl;

        MatrixMath::Vector4i<> v4i2(v4i1);
        SET_DEBUG_NAME(v4i2);
        std::cout
            << "v4i1 == v4i2 -> "
            << (v4i1 == v4i2 ? "[Succeed]" : "[Fail]")
            << std::endl;

        MatrixMath::Vector4i<> v4i3(std::move(v4i1));
        SET_DEBUG_NAME(v4i3);
        std::cout
            << "v4i1 == v4i3 -> "
            << (v4i1 == v4i3 ? "[Succeed]" : "[Fail]")
            << std::endl
            << std::endl;
    }
#endif

    //==============================================
    // Cofactor
#if ACTIVATE_MATRIX_TEST
    {
        MatrixMath::Matrix4i<> m4i1{
            1, 2, 3, 4,
            0, 1, 2, 3,
            0, 0, 1, 2,
            0, 0, 0, 1,
        };

        auto m3i1 = m4i1.GetCofactor<0, 0>();
        std::cout
            << "m3i1 = "
            << std::endl
            << m3i1.ToString()
            << std::endl;

        auto acm4i1 = MatrixMath::AlgebraicCofactor<3, 3>(m4i1);
        auto acm4i2 = MatrixMath::AlgebraicCofactor<0, 1>(m4i1);
        std::cout
            << "AlgebraicCofactor<3, 3>(m4i1) = "
            << acm4i1
            << std::endl
            << "AlgebraicCofactor<0, 1>(m4i1) = "
            << acm4i2
            << std::endl
            << std::endl;
    }
#endif

    //==============================================
    // Scalar
#if ACTIVATE_MATRIX_TEST
    {
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

        // Test: Scalar with float
        MatrixMath::Scalar<float> s4{ 3.14f };
        std::cout
            << "s4 -> " << s4 << std::endl
            << "s4 != 3 -> "
            << (s4 != 3 ? "[Succeed]" : "[Fail]")
            << std::endl
            << std::endl;
    }
#endif

    //==============================================
    // Identity
#if ACTIVATE_MATRIX_TEST
    {
        // Test: Identity initialization

        MatrixMath::IdentityMatrix<float, 4> mi4f1;
        SET_DEBUG_NAME(mi4f1);
        std::cout
            << "mi4f1 =" << std::endl
            << mi4f1.ToString()
            << std::endl;

        MatrixMath::IdentityMatrix<float, 6> mi4f2;
        SET_DEBUG_NAME(mi4f2);
        std::cout
            << "mi4f2 =" << std::endl
            << mi4f2.ToString()
            << std::endl;

        MatrixMath::MatrixQ<float, 4> m4f1{ mi4f1 * mi4f1 };
        SET_DEBUG_NAME(m4f1);
        std::cout
            << "m4f1 == mi4f1 -> "
            << std::boolalpha << (m4f1 == mi4f1) << std::noboolalpha << " "
            << (m4f1 == mi4f1 ? "[Succeed]" : "[Fail]")
            << std::endl
            << std::endl;
    }
#endif

    //==============================================
    // Determinant
#if ACTIVATE_MATRIX_TEST
    {
        MatrixMath::Matrix3i<> m3i1{ 3, 4, 9, 7, 6, 5, 0, 1, 2 };
        SET_DEBUG_NAME(m3i1);
        std::cout
            << "imat = " << std::endl
            << m3i1.ToString();

        MatrixMath::Determinant detA1(m3i1);
        std::cout
            << "Result = " << detA1 << " "
            << (detA1 == 28 ? "[Succeed]" : "[Fail]")
            << std::endl;

        std::cout << std::endl;
    }
#endif

    //==============================================
    // ChangeOrder
#if ACTIVATE_MATRIX_TEST
    {
        MatrixMath::Matrix2f<> m2f1{
            1.0f, 0.0f,
            2.0f, 3.0f,
        };
        auto m2f2{ MatrixMath::ChangeOrder<MatrixMath::StorageOrder::ColumnMajor>(m2f1) };

        std::cout
            << "m2f1 = " << std::endl
            << m2f1.ToString()
            << std::endl

            << "m2f2 = " << std::endl
            << m2f2.ToString()
            << std::endl;
    }
#endif

#if ACTIVATE_MATRIX_TEST
    {
        MatrixMath::Matrix3i<> m3i1{
            1, -2, 2,
            -3, 1, 1,
            -2, -1, 6,
        };
        SET_DEBUG_NAME(m3i1);
        std::cout
            << "m3i1 = " << std::endl
            << m3i1.ToString();

        MatrixMath::Determinant detA(m3i1);
        std::cout
            << "det(m3i1) = "
            << detA
            << std::endl
            << "IsInvertible(m3i1) -> "
            << std::boolalpha
            << MatrixMath::IsInvertible(m3i1)
            << std::endl
            << std::endl;

#define _PRINT_COFACTOR_0(m) \
    std::cout \
        << #m " = " \
        << std::endl \
        << m.ToString() \
        << "det(" #m ") = " \
        << MatrixMath::Determinant(m) \
        << std::endl \
        << std::endl
#define PRINT_COFACTOR(m, x, y) \
    do { \
        auto _##m##x##y{ m.template GetCofactor<x, y>() }; \
        _PRINT_COFACTOR_0(_##m##x##y); \
    } while(0)

        PRINT_COFACTOR(m3i1, 0, 0);
        PRINT_COFACTOR(m3i1, 0, 1);
        PRINT_COFACTOR(m3i1, 0, 2);
        PRINT_COFACTOR(m3i1, 1, 0);
        PRINT_COFACTOR(m3i1, 1, 1);
        PRINT_COFACTOR(m3i1, 1, 2);
        PRINT_COFACTOR(m3i1, 2, 0);
        PRINT_COFACTOR(m3i1, 2, 1);
        PRINT_COFACTOR(m3i1, 2, 2);

        MatrixMath::Matrix3i<> m3i2{ MatrixMath::AdjointMatrix(m3i1) };
        std::cout
            << "m3i2 = Adjoint(m3i1) = "
            << std::endl
            << m3i2.ToString()
            << std::endl
            << std::endl;
    }
#endif

    //==============================================
    // Merge Matrix
#if ACTIVATE_MATRIX_TEST
    {
        MatrixMath::Matrix3i<> identity = MatrixMath::IdentityMatrix<int, 3>();
        MatrixMath::Matrix<int, 3, 4> left{
            1, 2, 3, 4,
            1, 1, 1, 1,
            2, 3, 4, 5,
        };
        auto mrgres = MatrixMath::Merge<MatrixMath::Matrix<int, 3, 4>, MatrixMath::Matrix3i<>, MatrixMath::MergeMode::ROW, MatrixMath::StorageOrder::RowMajor>(left, identity);
        std::cout
            << "Merge Result = " << std::endl
            << mrgres.ToString()
            << std::endl;
    }
#endif

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
