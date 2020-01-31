#pragma once

#include <algorithm>
#include <array>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <sstream>
#include <string>
#include <type_traits>

namespace MatrixMath
{
    struct StorageOrder
    {
        struct RowMajor;
        struct ColumnMajor;
    };

    struct StorageOrder::RowMajor : StorageOrder
    {
        template <int Height, int Width>
        inline static int convert2index(const int& row, const int& column);
    };

    struct StorageOrder::ColumnMajor : StorageOrder
    {
        template <int Height, int Width>
        inline static int convert2index(const int& row, const int& column);
    };

    template <typename _Ty, int Height, int Width, typename order>
    class ProtoMatrix
    {
        static_assert(Width > 0, "Template argument 'Width' has negative value!");
        static_assert(Height > 0, "Template argument 'Height' has negative value!");
        static_assert(std::is_same_v<StorageOrder::RowMajor, order> || std::is_same_v<StorageOrder::ColumnMajor, order>, "Template argument 'order' is invalid type!");

    public:
        using ElementType = _Ty;

        inline constexpr bool IsVector() const;
        inline constexpr bool IsSquare() const;
        inline constexpr bool IsScalar() const;
        inline constexpr int GetWidth() const;
        inline constexpr int GetHeight() const;
    };

    template <typename _Ty, int Height, int Width, typename order>
    class ProtoMatrixData
        : public ProtoMatrix<_Ty, Height, Width, order>
    {
    protected:
        std::array<_Ty, Width * Height> data; // 'Width * Height' here cannot be replaced by 'Size',
                                              // otherwise a compiler error (C2244) will be thrown at compile time
                                              // if the project is compiled with Microsoft VC++

        ProtoMatrixData();
        ProtoMatrixData(const ProtoMatrixData& other);
        ProtoMatrixData(const _Ty* pSrc, const _Ty* pDst);
        ProtoMatrixData(const std::array<_Ty, Width * Height>& other);
        ProtoMatrixData(const std::initializer_list<_Ty>& init);
        ~ProtoMatrixData() {};

    public:
        const std::array<_Ty, Width * Height>& GetData() const;
        std::array<_Ty, Width * Height>& GetData();
    };

    template <typename _Ty, int Height, int Width, typename order = StorageOrder::RowMajor>
    class Matrix
        : public ProtoMatrixData<_Ty, Height, Width, order>
    {
    public:
        using Transposed = Matrix<_Ty, Width, Height, order>;

        Matrix();
        Matrix(const Matrix& other);
        Matrix(const std::initializer_list<_Ty>& init);

        // Access data

        inline void SetElement(const int& index, const _Ty& value);
        inline const _Ty& GetElement(const int& index) const;
        inline void SetElement(const int& row, const int& column, const _Ty& value);
        inline const _Ty& GetElement(const int& row, const int& column) const;
        inline _Ty& GetElement(const int& index);
        inline _Ty& GetElement(const int& row, const int& column);

        Transposed Transpose() const;

        // Always output a string representing the matrix in row-major order
        const std::string ToString() const;
    };

    // Vector

    template <typename _Ty, int N, typename order>
    class Matrix<_Ty, N, 1, order>;

    // Scalar

    template <typename _Ty, typename order>
    class Matrix<_Ty, 1, 1, order>;

    // Identity

    template <typename _Ty, int N, typename order = StorageOrder::RowMajor>
    class IdentityMatrix
        : public Matrix<_Ty, N, N, order>
    {
    public:
        IdentityMatrix();
    };

    // Conduct template instantiation of IdentityMatrix
#ifdef MakeIdentityMatrix
#   error "Failed to conduct template instantiation of IdentityMatrix!"
#else
#   define MakeIdentityMatrix(_Ty, N, initializer) template <typename order> \
    class IdentityMatrix<_Ty, N, order> : public Matrix<_Ty, N, N, order> \
    { public: IdentityMatrix() : Matrix<_Ty, N, N, order>initializer {} }

    MakeIdentityMatrix(int, 2, ({ 1,0,0,1 }));
    MakeIdentityMatrix(int, 3, ({ 1,0,0,0,1,0,0,0,1 }));
    MakeIdentityMatrix(int, 4, ({ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 }));
    MakeIdentityMatrix(float, 2, ({ 1.0f,0.0f,0.0f,1.0f }));
    MakeIdentityMatrix(float, 3, ({ 1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f }));
    MakeIdentityMatrix(float, 4, ({ 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f }));
    MakeIdentityMatrix(double, 2, ({ 1.0,0.0,0.0,1.0 }));
    MakeIdentityMatrix(double, 3, ({ 1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0 }));
    MakeIdentityMatrix(double, 4, ({ 1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0 }));

#   undef MakeIdentityMatrix
#endif

    // Alias

    template <typename _Ty, int N, typename order = StorageOrder::RowMajor>
    using MatrixQ = Matrix<_Ty, N, N, order>;

    template <typename _Ty, int N, typename order = StorageOrder::RowMajor>
    using Vector = Matrix<_Ty, N, 1, order>;

    template <typename _Ty, typename order = StorageOrder::RowMajor>
    using Scalar = Matrix<_Ty, 1, 1, order>;

    template <typename order = StorageOrder::RowMajor>
    using Matrix2i = MatrixQ<int, 2, order>;

    template <typename order = StorageOrder::RowMajor>
    using Matrix3i = MatrixQ<int, 3, order>;

    template <typename order = StorageOrder::RowMajor>
    using Matrix4i = MatrixQ<int, 4, order>;

    template <typename order = StorageOrder::RowMajor>
    using Matrix2f = MatrixQ<float, 2, order>;

    template <typename order = StorageOrder::RowMajor>
    using Matrix3f = MatrixQ<float, 3, order>;

    template <typename order = StorageOrder::RowMajor>
    using Matrix4f = MatrixQ<float, 4, order>;

    template <typename order = StorageOrder::RowMajor>
    using Matrix2d = MatrixQ<double, 2, order>;

    template <typename order = StorageOrder::RowMajor>
    using Matrix3d = MatrixQ<double, 3, order>;

    template <typename order = StorageOrder::RowMajor>
    using Matrix4d = MatrixQ<double, 4, order>;

    template <typename order = StorageOrder::RowMajor>
    using Vector2i = Vector<int, 2, order>;

    template <typename order = StorageOrder::RowMajor>
    using Vector3i = Vector<int, 3, order>;

    template <typename order = StorageOrder::RowMajor>
    using Vector4i = Vector<int, 4, order>;

    template <typename order = StorageOrder::RowMajor>
    using Vector2f = Vector<float, 2, order>;

    template <typename order = StorageOrder::RowMajor>
    using Vector3f = Vector<float, 3, order>;

    template <typename order = StorageOrder::RowMajor>
    using Vector4f = Vector<float, 4, order>;

    template <typename order = StorageOrder::RowMajor>
    using Vector2d = Vector<double, 2, order>;

    template <typename order = StorageOrder::RowMajor>
    using Vector3d = Vector<double, 3, order>;

    template <typename order = StorageOrder::RowMajor>
    using Vector4d = Vector<double, 4, order>;

    // Basic algorithms

    template <typename _Ty, int Height, int Width, typename order>
    void operator+=(Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs);

    template <typename _Ty, int Height, int Width, typename order>
    void operator-=(Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs);

    template <typename _Ty, int Height, int Width, typename order>
    void operator*=(Matrix<_Ty, Height, Width, order>& lhs, const _Ty& rhs);

    template <typename _Ty, int Height, int Width, typename order>
    void operator/=(Matrix<_Ty, Height, Width, order>& lhs, const _Ty& rhs);

    template <typename _Ty, int Height, int Width, typename order>
    Matrix<_Ty, Height, Width, order> operator+(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs);

    template <typename _Ty, int Height, int Width, typename order>
    Matrix<_Ty, Height, Width, order> operator-(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs);

    template <typename _Ty, int Height, int Width, typename order>
    Matrix<_Ty, Height, Width, order> operator/(const Matrix<_Ty, Height, Width, order>& lhs, const _Ty& rhs);

    template <typename _Ty, int Height, int Width, typename order>
    bool operator==(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs);

    template <typename _Ty, int Height, int Width, typename order>
    bool operator!=(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs);

    // Multiplication algorithms

    template <typename _Ty, int Height, int Width, typename order>
    Matrix<_Ty, Height, Width, order> operator*(const Matrix<_Ty, Height, Width, order>&, const _Ty&);

    template <typename _Ty, int Height, int Width, typename order>
    Matrix<_Ty, Height, Width, order> operator*(const _Ty&, const Matrix<_Ty, Height, Width, order>&);

    template <typename _Ty, typename order>
    _Ty operator*(const Scalar<_Ty, order>&, const _Ty&);

    template <typename _Ty, typename order>
    _Ty operator*(const _Ty&, const Scalar<_Ty, order>&);

    // multiplying a (M x P) matrix and a (P x N) matrix,
    // result: a (M x N) matrix
    // no optimization is applied in the following function
    // @see: https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
    template <typename _Ty, int M, int P, int N, typename order>
    Matrix<_Ty, M, N, order> operator*(const Matrix<_Ty, M, P, order>&, const Matrix<_Ty, P, N, order>&);

    template <typename _Ty, typename order>
    MatrixQ<_Ty, 2, order> operator*(const MatrixQ<_Ty, 2, order>&, const MatrixQ<_Ty, 2, order>&);

    template <typename _Ty, typename order>
    MatrixQ<_Ty, 3, order> operator*(const MatrixQ<_Ty, 3, order>&, const MatrixQ<_Ty, 3, order>&);

    template <typename _Ty, typename order>
    MatrixQ<_Ty, 4, order> operator*(const MatrixQ<_Ty, 4, order>&, const MatrixQ<_Ty, 4, order>&);

    template <typename _Ty, int Height, int Width, typename order>
    Matrix<_Ty, Height, Width, order> operator*(const Matrix<_Ty, Height, Width, order>&, const Scalar<_Ty, order>&);

    template <typename _Ty, int Height, int Width, typename order>
    Matrix<_Ty, Height, Width, order> operator*(const Scalar<_Ty, order>&, const Matrix<_Ty, Height, Width, order>&);
} /* NAMESPACE: MatrixMath */


template <typename _Ty, int Height, int Width, typename order>
inline constexpr bool
MatrixMath::ProtoMatrix<_Ty, Height, Width, order>::
IsVector() const
{
    return (Height == 1 && Width > 1) || (Width == 1 && Height > 1);
}

template <typename _Ty, int Height, int Width, typename order>
inline constexpr bool
MatrixMath::ProtoMatrix<_Ty, Height, Width, order>::
IsSquare() const
{
    return Height == Width && Height > 1;
}

template <typename _Ty, int Height, int Width, typename order>
inline constexpr bool
MatrixMath::ProtoMatrix<_Ty, Height, Width, order>::
IsScalar() const
{
    return Height == 1 && Width == 1;
}

template <typename _Ty, int Height, int Width, typename order>
inline constexpr int
MatrixMath::ProtoMatrix<_Ty, Height, Width, order>::
GetWidth() const
{
    return Width;
}

template <typename _Ty, int Height, int Width, typename order>
inline constexpr int
MatrixMath::ProtoMatrix<_Ty, Height, Width, order>::
GetHeight() const
{
    return Height;
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
ProtoMatrixData()
    : data{ 0 } // initialize std::array with 0
{
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
ProtoMatrixData(const ProtoMatrixData& other)
    : ProtoMatrixData(other.data)
{
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
ProtoMatrixData(const _Ty* src, const _Ty* dst)
    : ProtoMatrixData()
{
    // prevent buffer overflow attack
    const _Ty* end{ src + std::min<ptrdiff_t>(dst - src, Width * Height) };
    std::copy(src, end, std::begin(data));
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
ProtoMatrixData(const std::array<_Ty, Width * Height>& other)
{
    std::copy(std::begin(other), std::end(other), std::begin(data));
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
ProtoMatrixData(const std::initializer_list<_Ty>& init)
    : ProtoMatrixData(init.begin(), init.end())
{
}

template <typename _Ty, int Height, int Width, typename order>
const std::array<_Ty, Width * Height>&
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
GetData() const
{
    return this->data;
}

template <typename _Ty, int Height, int Width, typename order>
std::array<_Ty, Width * Height>&
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
GetData()
{
    return this->data;
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix()
    : ProtoMatrixData<_Ty, Height, Width, order>()
{
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix(const Matrix& other)
    : ProtoMatrixData<_Ty, Height, Width, order>(other)
{
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix(const std::initializer_list<_Ty>& init)
    : ProtoMatrixData<_Ty, Height, Width, order>(init)
{
}

template <typename _Ty, int Height, int Width, typename order>
inline void
MatrixMath::Matrix<_Ty, Height, Width, order>::
SetElement(const int& index, const _Ty& value)
{
    this->data[index] = value;
}

template <typename _Ty, int Height, int Width, typename order>
inline const _Ty&
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetElement(const int& index) const
{
    return this->data[index];
}

template <typename _Ty, int Height, int Width, typename order>
inline void
MatrixMath::Matrix<_Ty, Height, Width, order>::
SetElement(const int& row, const int& column, const _Ty& value)
{
    this->SetElement(order::convert2index<Height, Width>(row, column), value);
}

template <typename _Ty, int Height, int Width, typename order>
inline const _Ty&
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetElement(const int& row, const int& column) const
{
    return this->GetElement(order::convert2index<Height, Width>(row, column));
}

template <typename _Ty, int Height, int Width, typename order>
inline _Ty&
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetElement(const int& index)
{
    return this->data[index];
}

template <typename _Ty, int Height, int Width, typename order>
inline _Ty&
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetElement(const int& row, const int& column)
{
    return this->GetElement(order::convert2index<Height, Width>(row, column));
}

template <typename _Ty, int Height, int Width, typename order>
typename MatrixMath::Matrix<_Ty, Height, Width, order>::Transposed
MatrixMath::Matrix<_Ty, Height, Width, order>::
Transpose() const
{
    Transposed result;

    for (int row = 0; row < Width; row++)
    {
        for (int column = 0; column < Height; column++)
        {
            result.SetElement(row, column, this->GetElement(column, row));
        }
    }

    return result;
}

template <typename _Ty, int Height, int Width, typename order>
const std::string
MatrixMath::Matrix<_Ty, Height, Width, order>::
ToString() const
{
    std::stringstream ss;

    for (int row = 0; row < Height; row++)
    {
        ss << "| ";
        for (int column = 0; column < Width; column++)
        {
            ss << GetElement(row, column)
                << " ";
        }
        ss << "|"
            << std::endl;
    }

    return ss.str();
}

template <typename _Ty, int N, typename order>
MatrixMath::IdentityMatrix<_Ty, N, order>::
IdentityMatrix()
    : MatrixQ<_Ty, N, order>()
{
    for (int i = 0; i < N; i++)
        this->SetElement(i, i, 1);
}


template <typename _Ty, int Height, int Width, typename order>
void
MatrixMath::
operator+=(Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs)
{
    for (int i{ 0 }; i < Width * Height; i++)
        lhs.GetElement(i) += rhs.GetElement(i);
}

template <typename _Ty, int Height, int Width, typename order>
void
MatrixMath::
operator-=(Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs)
{
    for (int i{ 0 }; i < Width * Height; i++)
        lhs.GetElement(i) -= rhs.GetElement(i);
}

template <typename _Ty, int Height, int Width, typename order>
void
MatrixMath::
operator*=(Matrix<_Ty, Height, Width, order>& lhs, const _Ty& rhs)
{
    for (int i{ 0 }; i < Width * Height; i++)
        lhs.GetElement(i) *= rhs;
}

template <typename _Ty, int Height, int Width, typename order>
void
MatrixMath::
operator/=(Matrix<_Ty, Height, Width, order>& lhs, const _Ty& rhs)
{
    for (int i{ 0 }; i < Width * Height; i++)
        lhs.GetElement(i) /= rhs;
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator+(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs)
{
    Matrix result(lhs);
    result += rhs;
    return result;
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator-(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs)
{
    Matrix result(lhs);
    result -= rhs;
    return result;
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator/(const Matrix<_Ty, Height, Width, order>& lhs, const _Ty& rhs)
{
    Matrix result(lhs);
    result /= rhs;
    return result;
}

template <typename _Ty, int Height, int Width, typename order>
bool
MatrixMath::
operator==(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs)
{
    return &lhs == &rhs
        || lhs.GetData() == rhs.GetData();
}

template <typename _Ty, int Height, int Width, typename order>
bool
MatrixMath::
operator!=(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs)
{
    return !(lhs == rhs);
}


template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator*(const MatrixMath::Matrix<_Ty, Height, Width, order>& lhs, const _Ty& rhs)
{
    Matrix result(lhs);
    result *= rhs;
    return result;
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator*(const _Ty& lhs, const MatrixMath::Matrix<_Ty, Height, Width, order>& rhs)
{
    return rhs * lhs;
}

template <typename _Ty, typename order>
_Ty
MatrixMath::
operator*(const Scalar<_Ty, order>& lhs, const _Ty& rhs)
{
    return lhs.GetData() * rhs;
}

template <typename _Ty, typename order>
_Ty
MatrixMath::
operator*(const _Ty& lhs, const Scalar<_Ty, order>& rhs)
{
    return lhs * rhs.GetData();
}

template <typename _Ty, int M, int P, int N, typename order>
MatrixMath::Matrix<_Ty, M, N, order>
MatrixMath::
operator*(const MatrixMath::Matrix<_Ty, M, P, order>& lhs, const MatrixMath::Matrix<_Ty, P, N, order>& rhs)
{
    MatrixMath::Matrix<_Ty, M, N, order> result;

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            _Ty sum = 0;

            for (int k = 0; k < P; k++)
                sum += lhs.GetElement(i, k) * rhs.GetElement(k, j);

            result.SetElement(i, j, sum);
        }
    }

    return result;
}

template <typename _Ty, typename order>
MatrixMath::MatrixQ<_Ty, 2, order>
MatrixMath::
operator*(const MatrixQ<_Ty, 2, order>& lhs, const MatrixQ<_Ty, 2, order>& rhs)
{
    const _Ty& a11 = lhs.GetElement(0, 0);
    const _Ty& a12 = lhs.GetElement(0, 1);
    const _Ty& a21 = lhs.GetElement(1, 0);
    const _Ty& a22 = lhs.GetElement(1, 1);
    const _Ty& b11 = rhs.GetElement(0, 0);
    const _Ty& b12 = rhs.GetElement(0, 1);
    const _Ty& b21 = rhs.GetElement(1, 0);
    const _Ty& b22 = rhs.GetElement(1, 1);

    const _Ty c11 = a11 * b11 + a12 * b21;
    const _Ty c12 = a11 * b12 + a12 * b22;
    const _Ty c21 = a21 * b11 + a22 * b21;
    const _Ty c22 = a21 * b12 + a22 * b22;

    if constexpr (order == MatrixMath::StorageOrder::ROW_MAJOR)
        return MatrixQ<_Ty, 2, order>{ c11, c12, c21, c22 };
    else
        return MatrixQ<_Ty, 2, order>{ c11, c21, c12, c22 };
}

template <typename _Ty, typename order>
MatrixMath::MatrixQ<_Ty, 3, order>
MatrixMath::
operator*(const MatrixQ<_Ty, 3, order>& lhs, const MatrixQ<_Ty, 3, order>& rhs)
{
    const _Ty& a11 = lhs.GetElement(0, 0);
    const _Ty& a12 = lhs.GetElement(0, 1);
    const _Ty& a13 = lhs.GetElement(0, 2);
    const _Ty& a21 = lhs.GetElement(1, 0);
    const _Ty& a22 = lhs.GetElement(1, 1);
    const _Ty& a23 = lhs.GetElement(1, 2);
    const _Ty& a31 = lhs.GetElement(2, 0);
    const _Ty& a32 = lhs.GetElement(2, 1);
    const _Ty& a33 = lhs.GetElement(2, 2);
    const _Ty& b11 = rhs.GetElement(0, 0);
    const _Ty& b12 = rhs.GetElement(0, 1);
    const _Ty& b13 = rhs.GetElement(0, 2);
    const _Ty& b21 = rhs.GetElement(1, 0);
    const _Ty& b22 = rhs.GetElement(1, 1);
    const _Ty& b23 = rhs.GetElement(1, 2);
    const _Ty& b31 = rhs.GetElement(2, 0);
    const _Ty& b32 = rhs.GetElement(2, 1);
    const _Ty& b33 = rhs.GetElement(2, 2);

    const _Ty c11 = a11 * b11 + a12 * b21 + a13 * b31;
    const _Ty c12 = a11 * b12 + a12 * b22 + a13 * b32;
    const _Ty c13 = a11 * b13 + a12 * b23 + a13 * b33;
    const _Ty c21 = a21 * b11 + a22 * b21 + a23 * b31;
    const _Ty c22 = a21 * b12 + a22 * b22 + a23 * b32;
    const _Ty c23 = a21 * b13 + a22 * b23 + a23 * b33;
    const _Ty c31 = a31 * b11 + a32 * b21 + a33 * b31;
    const _Ty c32 = a31 * b12 + a32 * b22 + a33 * b32;
    const _Ty c33 = a31 * b13 + a32 * b23 + a33 * b33;

    if constexpr (order == MatrixMath::StorageOrder::ROW_MAJOR)
        return MatrixQ<_Ty, 2, order>{ c11, c12, c13, c21, c22, c23, c31, c32, c33 };
    else
        return MatrixQ<_Ty, 2, order>{ c11, c21, c31, c12, c22, c32, c13, c23, c33 };
}

template <typename _Ty, typename order>
MatrixMath::MatrixQ<_Ty, 4, order>
MatrixMath::
operator*(const MatrixQ<_Ty, 4, order>& lhs, const MatrixQ<_Ty, 4, order>& rhs)
{
    const _Ty& a11 = lhs.GetElement(0, 0);
    const _Ty& a12 = lhs.GetElement(0, 1);
    const _Ty& a13 = lhs.GetElement(0, 2);
    const _Ty& a14 = lhs.GetElement(0, 3);
    const _Ty& a21 = lhs.GetElement(1, 0);
    const _Ty& a22 = lhs.GetElement(1, 1);
    const _Ty& a23 = lhs.GetElement(1, 2);
    const _Ty& a24 = lhs.GetElement(1, 3);
    const _Ty& a31 = lhs.GetElement(2, 0);
    const _Ty& a32 = lhs.GetElement(2, 1);
    const _Ty& a33 = lhs.GetElement(2, 2);
    const _Ty& a34 = lhs.GetElement(2, 3);
    const _Ty& a41 = lhs.GetElement(3, 0);
    const _Ty& a42 = lhs.GetElement(3, 1);
    const _Ty& a43 = lhs.GetElement(3, 2);
    const _Ty& a44 = lhs.GetElement(3, 3);
    const _Ty& b11 = rhs.GetElement(0, 0);
    const _Ty& b12 = rhs.GetElement(0, 1);
    const _Ty& b13 = rhs.GetElement(0, 2);
    const _Ty& b14 = rhs.GetElement(0, 3);
    const _Ty& b21 = rhs.GetElement(1, 0);
    const _Ty& b22 = rhs.GetElement(1, 1);
    const _Ty& b23 = rhs.GetElement(1, 2);
    const _Ty& b24 = rhs.GetElement(1, 3);
    const _Ty& b31 = rhs.GetElement(2, 0);
    const _Ty& b32 = rhs.GetElement(2, 1);
    const _Ty& b33 = rhs.GetElement(2, 2);
    const _Ty& b34 = rhs.GetElement(2, 3);
    const _Ty& b41 = rhs.GetElement(3, 0);
    const _Ty& b42 = rhs.GetElement(3, 1);
    const _Ty& b43 = rhs.GetElement(3, 2);
    const _Ty& b44 = rhs.GetElement(3, 3);

    const _Ty c11 = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
    const _Ty c12 = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
    const _Ty c13 = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
    const _Ty c14 = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;
    const _Ty c21 = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
    const _Ty c22 = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
    const _Ty c23 = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
    const _Ty c24 = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;
    const _Ty c31 = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
    const _Ty c32 = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
    const _Ty c33 = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
    const _Ty c34 = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;
    const _Ty c41 = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
    const _Ty c42 = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
    const _Ty c43 = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
    const _Ty c44 = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

    if constexpr (order == MatrixMath::StorageOrder::ROW_MAJOR)
        return MatrixQ<_Ty, 4, order>{ c11, c12, c13, c14, c21, c22, c23, c24, c31, c32, c33, c34, c41, c42, c43, c44 };
    else
        return MatrixQ<_Ty, 4, order>{ c11, c21, c31, c41, c12, c22, c32, c42, c13, c23, c33, c43, c14, c24, c34, c44 };
}

// Vector de facto
template <typename _Ty, int N, typename order>
class MatrixMath::Matrix<_Ty, N, 1, order>
    : public MatrixMath::ProtoMatrixData<_Ty, N, 1, order>
{
public:
    using Transposed = Matrix<_Ty, 1, N, order>;

    Matrix();
    Matrix(const Matrix& other);
    Matrix(const std::initializer_list<_Ty>& init);
    ~Matrix() {}

    void SetElement(const int& index, const _Ty& value);
    const _Ty& GetElement(const int& index) const;
};

template <typename _Ty, int N, typename order>
MatrixMath::Matrix<_Ty, N, 1, order>::
Matrix()
    : ProtoMatrixData<_Ty, N, 1, order>()
{
}

template <typename _Ty, int N, typename order>
MatrixMath::Matrix<_Ty, N, 1, order>::
Matrix(const Matrix& other)
    : ProtoMatrixData<_Ty, N, 1, order>(other)
{
}

template <typename _Ty, int N, typename order>
MatrixMath::Matrix<_Ty, N, 1, order>::
Matrix(const std::initializer_list<_Ty>& init)
    : ProtoMatrixData<_Ty, N, 1, order>(init)
{
}

template <typename _Ty, int N, typename order>
void
MatrixMath::Matrix<_Ty, N, 1, order>::
SetElement(const int& index, const _Ty& value)
{
    this->data[index] = value;
}

template <typename _Ty, int N, typename order>
const _Ty&
MatrixMath::Matrix<_Ty, N, 1, order>::
GetElement(const int& index) const
{
    return this->data[index];
}


// Scalar de facto
template <typename _Ty, typename order>
class MatrixMath::Matrix<_Ty, 1, 1, order>
    : public MatrixMath::ProtoMatrix<_Ty, 1, 1, order>
{
private:
    _Ty data;

public:
    Matrix();
    Matrix(const _Ty&);
    ~Matrix() {}

    void SetData(const _Ty& value);
    const _Ty& GetData() const;
    operator _Ty() const;

    void operator+=(const Matrix&);
    void operator+=(const _Ty&);
    void operator-=(const Matrix&);
    void operator-=(const _Ty&);
    void operator*=(const Matrix&);
    void operator*=(const _Ty&);
    void operator/=(const Matrix&);
    void operator/=(const _Ty&);
    _Ty operator+(const Matrix&) const;
    _Ty operator+(const _Ty&) const;
    _Ty operator-(const Matrix&) const;
    _Ty operator-(const _Ty&) const;
    _Ty operator/(const Matrix&) const;
    _Ty operator/(const _Ty&) const;
    bool operator==(const Matrix&) const;
    bool operator!=(const Matrix&) const;
    bool operator==(const _Ty&) const;
    bool operator!=(const _Ty&) const;
    _Ty Transpose() const;

    const std::string ToString() const;
};

template <typename _Ty, typename order>
MatrixMath::Matrix<_Ty, 1, 1, order>::
Matrix()
    : data{ 0 }
{
}

template <typename _Ty, typename order>
MatrixMath::Matrix<_Ty, 1, 1, order>::
Matrix(const _Ty& value)
    : data{ value }
{
}

template <typename _Ty, typename order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
SetData(const _Ty& value)
{
    this->data = value;
}

template <typename _Ty, typename order>
const _Ty&
MatrixMath::Matrix<_Ty, 1, 1, order>::
GetData() const
{
    return this->data;
}

template <typename _Ty, typename order>
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator _Ty() const
{
    return this->data;
}

template <typename _Ty, typename order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator+=(const Matrix& other)
{
    this->data += other.data;
}

template <typename _Ty, typename order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator+=(const _Ty& other)
{
    this->data += other;
}

template <typename _Ty, typename order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator-=(const Matrix& other)
{
    this->data -= other.data;
}

template <typename _Ty, typename order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator-=(const _Ty& other)
{
    this->data -= other;
}

template <typename _Ty, typename order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator*=(const Matrix& multiplier)
{
    this->data *= multiplier.data;
}

template <typename _Ty, typename order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator*=(const _Ty& multiplier)
{
    this->data *= multiplier;
}

template <typename _Ty, typename order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator/=(const Matrix& divider)
{
    this->data /= divider.data;
}

template <typename _Ty, typename order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator/=(const _Ty& divider)
{
    this->data /= divider;
}

template <typename _Ty, typename order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator+(const Matrix& other) const
{
    return this->data + other.data;
}

template <typename _Ty, typename order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator+(const _Ty& other) const
{
    return this->data + other;
}

template <typename _Ty, typename order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator-(const Matrix& other) const
{
    return this->data - other.data;
}

template <typename _Ty, typename order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator-(const _Ty& other) const
{
    return this->data - other;
}

template <typename _Ty, typename order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator/(const _Ty& divider) const
{
    return this->data / divider;
}

template <typename _Ty, typename order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator/(const Matrix& divider) const
{
    return this->data / divider.data;
}

template <typename _Ty, typename order>
bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator==(const Matrix& other) const
{
    return this->data == other.data;
}

template <typename _Ty, typename order>
bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator!=(const Matrix& other) const
{
    return this->data != other.data;
}

template <typename _Ty, typename order>
bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator==(const _Ty& other) const
{
    return this->data == other;
}

template <typename _Ty, typename order>
bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator!=(const _Ty& other) const
{
    return this->data != other;
}

template <typename _Ty, typename order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
Transpose() const
{
    return this->data;
}

template<typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator*(const Matrix<_Ty, Height, Width, order>& lhs, const Scalar<_Ty, order>& rhs)
{
    Matrix<_Ty, Height, Width, order> result(lhs);
    result *= rhs.GetData();
    return result;
}

template<typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator*(const Scalar<_Ty, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs)
{
    return rhs * lhs;
}


template <int Height, int Width>
inline static int
MatrixMath::StorageOrder::RowMajor::
convert2index(const int& row, const int& column)
{
    return column + row * Width;
}

template <int Height, int Width>
inline static int
MatrixMath::StorageOrder::ColumnMajor::
convert2index(const int& row, const int& column)
{
    return row + column * Height;
}