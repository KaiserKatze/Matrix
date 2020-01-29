#pragma once

#include <algorithm>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <sstream>
#include <string>

namespace MatrixMath
{
    enum StorageOrder : unsigned char
    {
        ROW_MAJOR = 1,
        COLUMN_MAJOR = 2,
    };

    // the entries in this class are stored in column-major order
    // template argument 'typename _Ty' can be one of the following:
    //  - real number (such as int, float, double, etc.)
    //  - complex number (std::complex<double>, etc.)
    //  - matrix
    template <typename _Ty, int Height, int Width, StorageOrder order = StorageOrder::ROW_MAJOR>
    class Matrix
    {
        static_assert(Width > 0, "Template argument 'Width' has negative value!");
        static_assert(Height > 0, "Template argument 'Height' has negative value!");

    private:
        _Ty data[Width * Height];

        int convert2index(const int& row, const int& column) const;
        Matrix(const _Ty*, const _Ty*);

    public:
        using Transposed = Matrix<_Ty, Width, Height, order>;

        // Default constructor, initialize its inner array with '0'
        Matrix();
        // Copy data
        Matrix(const Matrix&);
        // The elements of std::initializer_list is guaranteed to be contiguous
        Matrix(const std::initializer_list<_Ty>&);
        // Deconstructor
        virtual ~Matrix() {}

        // Access data
        void SetData(const int& row, const int& column, const _Ty& value);
        // Access data
        const _Ty& GetData(const int& row, const int& column) const;
        constexpr bool IsVector() const;
        constexpr bool IsSquare() const;
        constexpr int GetWidth() const;
        constexpr int GetHeight() const;
        void operator+=(const Matrix&);
        void operator-=(const Matrix&);
        void operator*=(const _Ty&);
        void operator/=(const _Ty&);
        Matrix operator+(const Matrix&) const;
        Matrix operator-(const Matrix&) const;
        Matrix operator/(const _Ty&) const;
        bool operator==(const Matrix&) const;
        Transposed Transpose() const;
        const std::string ToString() const;
    };

    template <typename _Ty, int N, StorageOrder order = StorageOrder::ROW_MAJOR>
    using MatrixQ = Matrix<_Ty, N, N, order>;

    template <typename _Ty, int N, StorageOrder order = StorageOrder::ROW_MAJOR>
    using Vector = Matrix<_Ty, N, 1, order>;

    template <typename _Ty, StorageOrder order>
    class Matrix<_Ty, 1, 1, order>;

    template <typename _Ty, StorageOrder order = StorageOrder::ROW_MAJOR>
    using Scalar = Matrix<_Ty, 1, 1, order>;

    template <typename _Ty, int N, StorageOrder order = StorageOrder::ROW_MAJOR>
    class IdentityMatrix
        : public MatrixQ<_Ty, N, order>
    {
    public:
        IdentityMatrix();
    };

    template <typename _Ty, int Height, int Width, StorageOrder order>
    Matrix<_Ty, Height, Width, order> operator*(const Matrix<_Ty, Height, Width, order>&, const _Ty&);

    template <typename _Ty, int Height, int Width, StorageOrder order>
    Matrix<_Ty, Height, Width, order> operator*(const _Ty&, const Matrix<_Ty, Height, Width, order>&);

    template <typename _Ty, StorageOrder order>
    _Ty operator*(const Scalar<_Ty, order>&, const _Ty&);

    template <typename _Ty, StorageOrder order>
    _Ty operator*(const _Ty&, const Scalar<_Ty, order>&);

    // multiplying a (M x P) matrix and a (P x N) matrix,
    // result: a (M x N) matrix
    // no optimization is applied in the following function
    // @see: https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
    template <typename _Ty, int M, int P, int N, StorageOrder order>
    Matrix<_Ty, M, N, order> operator*(const Matrix<_Ty, M, P, order>&, const Matrix<_Ty, P, N, order>&);

    template <typename _Ty, StorageOrder order>
    MatrixQ<_Ty, 2, order> operator*(const MatrixQ<_Ty, 2, order>&, const MatrixQ<_Ty, 2, order>&);

    template <typename _Ty, StorageOrder order>
    MatrixQ<_Ty, 3, order> operator*(const MatrixQ<_Ty, 3, order>&, const MatrixQ<_Ty, 3, order>&);

    template <typename _Ty, StorageOrder order>
    MatrixQ<_Ty, 4, order> operator*(const MatrixQ<_Ty, 4, order>&, const MatrixQ<_Ty, 4, order>&);

    // Conduct template instantiation of IdentityMatrix
#ifdef MakeIdentityMatrix
#   error "Failed to conduct template instantiation of IdentityMatrix!"
#else
#   define MakeIdentityMatrix(_Ty, N, initializer) template <StorageOrder order> \
    class IdentityMatrix<_Ty, N, order> : public MatrixQ<_Ty, N, order> \
    { public: IdentityMatrix() : MatrixQ<_Ty, N, order>initializer {} }

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

    template <typename _Ty, int Height, int Width, StorageOrder order>
    Matrix<_Ty, Height, Width, order> operator*(const Matrix<_Ty, Height, Width, order>&, const Scalar<_Ty, order>&);

    template <typename _Ty, int Height, int Width, StorageOrder order>
    Matrix<_Ty, Height, Width, order> operator*(const Scalar<_Ty, order>&, const Matrix<_Ty, Height, Width, order>&);
} /* NAMESPACE: MatrixMath */


template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
int 
MatrixMath::Matrix<_Ty, Height, Width, order>::
convert2index(const int& row, const int& column) const
{
    if (row < 0)
        throw std::out_of_range("Invalid argument: row < 0!");
    if (row >= Height)
        throw std::out_of_range("Invalid argument: row >= Height!");
    if (column < 0)
        throw std::out_of_range("Invalid argument: column < 0!");
    if (column >= Width)
        throw std::out_of_range("Invalid argument: column >= Width!");

    if constexpr (order == MatrixMath::StorageOrder::COLUMN_MAJOR)
        return row + column * Height;
    else if constexpr (order == MatrixMath::StorageOrder::ROW_MAJOR)
        return column + row * Width;
    else
        throw std::invalid_argument("Invalid template argument 'order'!");
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix(const _Ty* src, const _Ty* dst)
    : Matrix()
{
    std::copy(src, dst, this->data);
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix()
    : data{ 0 }
{
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix(const Matrix& other)
    : Matrix(other.data, other.data + Width * Height)
{
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix(const std::initializer_list<_Ty>& init)
    : Matrix(init.begin(), init.end())
{
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, Height, Width, order>::
SetData(const int& row, const int& column, const _Ty& value)
{
    this->data[this->convert2index(row, column)] = value;
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
const _Ty&
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetData(const int& row, const int& column) const
{
    return this->data[this->convert2index(row, column)];
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
constexpr bool
MatrixMath::Matrix<_Ty, Height, Width, order>::
IsVector() const
{
    return (Height == 1 && Width > 1) || (Width == 1 && Height > 1);
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
constexpr bool
MatrixMath::Matrix<_Ty, Height, Width, order>::
IsSquare() const
{
    return Height == Width && Height > 1;
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
constexpr int
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetWidth() const
{
    return Width;
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
constexpr int
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetHeight() const
{
    return Height;
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
void 
MatrixMath::Matrix<_Ty, Height, Width, order>::
operator+=(const Matrix& other)
{
    for (int i = 0; i < Width * Height; i++)
    {
        this->data[i] += other.data[i];
    }
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, Height, Width, order>::
operator-=(const Matrix& other)
{
    for (int i = 0; i < Width * Height; i++)
    {
        this->data[i] -= other.data[i];
    }
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, Height, Width, order>::
operator*=(const _Ty& multiplier)
{
    for (int i = 0; i < Width * Height; i++)
    {
        this->data[i] *= multiplier;
    }
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, Height, Width, order>::
operator/=(const _Ty& divider)
{
    for (int i = 0; i < Width * Height; i++)
    {
        this->data[i] /= divider;
    }
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
operator+(const Matrix& other) const
{
    Matrix result(*this);
    result += other;
    return result;
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
operator-(const Matrix& other) const
{
    Matrix result(*this);
    result -= other;
    return result;
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
operator/(const _Ty& divider) const
{
    Matrix result(*this);
    result /= divider;
    return result;
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
bool
MatrixMath::Matrix<_Ty, Height, Width, order>::
operator==(const Matrix& other) const
{
    return this == &other
        || std::equal(std::begin(data), std::end(data),
            std::begin(other.data), std::end(other.data));
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
typename MatrixMath::Matrix<_Ty, Height, Width, order>::Transposed
MatrixMath::Matrix<_Ty, Height, Width, order>::
Transpose() const
{
    Transposed result;

    for (int row = 0; row < Width; row++)
    {
        for (int column = 0; column < Height; column++)
        {
            result.SetData(row, column, this->GetData(column, row));
        }
    }

    return result;
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
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
            ss << GetData(row, column)
                << " ";
        }
        ss << "|"
            << std::endl;
    }

    return ss.str();
}

template <typename _Ty, int N, MatrixMath::StorageOrder order>
MatrixMath::IdentityMatrix<_Ty, N, order>::
IdentityMatrix()
    : MatrixQ<_Ty, N, order>()
{
    for (int i = 0; i < N; i++)
        this->SetData(i, i, 1);
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator*(const MatrixMath::Matrix<_Ty, Height, Width, order>& first, const _Ty& second)
{
    Matrix result(first);
    result *= second;
    return result;
}

template <typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator*(const _Ty& first, const MatrixMath::Matrix<_Ty, Height, Width, order>& second)
{
    return second * first;
}

template <typename _Ty, MatrixMath::StorageOrder order>
_Ty
MatrixMath::
operator*(const Scalar<_Ty, order>& first, const _Ty& second)
{
    return first.GetData() * second;
}

template <typename _Ty, MatrixMath::StorageOrder order>
_Ty
MatrixMath::
operator*(const _Ty& first, const Scalar<_Ty, order>& second)
{
    return first * second.GetData();
}

template <typename _Ty, int M, int P, int N, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, M, N, order>
MatrixMath::
operator*(const MatrixMath::Matrix<_Ty, M, P, order>& first, const MatrixMath::Matrix<_Ty, P, N, order>& second)
{
    MatrixMath::Matrix<_Ty, M, N, order> result;

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            _Ty sum = 0;

            for (int k = 0; k < P; k++)
                sum += first.GetData(i, k) * second.GetData(k, j);

            result.setData(i, j, sum);
        }
    }

    return result;
}

template <typename _Ty, MatrixMath::StorageOrder order>
MatrixMath::MatrixQ<_Ty, 2, order>
MatrixMath::
operator*(const MatrixQ<_Ty, 2, order>& first, const MatrixQ<_Ty, 2, order>& second)
{
    const _Ty& a11 = first.GetData(0, 0);
    const _Ty& a12 = first.GetData(0, 1);
    const _Ty& a21 = first.GetData(1, 0);
    const _Ty& a22 = first.GetData(1, 1);
    const _Ty& b11 = second.GetData(0, 0);
    const _Ty& b12 = second.GetData(0, 1);
    const _Ty& b21 = second.GetData(1, 0);
    const _Ty& b22 = second.GetData(1, 1);

    const _Ty c11 = a11 * b11 + a12 * b21;
    const _Ty c12 = a11 * b12 + a12 * b22;
    const _Ty c21 = a21 * b11 + a22 * b21;
    const _Ty c22 = a21 * b12 + a22 * b22;

    if constexpr (order == MatrixMath::StorageOrder::ROW_MAJOR)
        return MatrixQ<_Ty, 2, order>{ c11, c12, c21, c22 };
    else
        return MatrixQ<_Ty, 2, order>{ c11, c21, c12, c22 };
}

template <typename _Ty, MatrixMath::StorageOrder order>
MatrixMath::MatrixQ<_Ty, 3, order>
MatrixMath::
operator*(const MatrixQ<_Ty, 3, order>& first, const MatrixQ<_Ty, 3, order>& second)
{
    const _Ty& a11 = first.GetData(0, 0);
    const _Ty& a12 = first.GetData(0, 1);
    const _Ty& a13 = first.GetData(0, 2);
    const _Ty& a21 = first.GetData(1, 0);
    const _Ty& a22 = first.GetData(1, 1);
    const _Ty& a23 = first.GetData(1, 2);
    const _Ty& a31 = first.GetData(2, 0);
    const _Ty& a32 = first.GetData(2, 1);
    const _Ty& a33 = first.GetData(2, 2);
    const _Ty& b11 = second.GetData(0, 0);
    const _Ty& b12 = second.GetData(0, 1);
    const _Ty& b13 = second.GetData(0, 2);
    const _Ty& b21 = second.GetData(1, 0);
    const _Ty& b22 = second.GetData(1, 1);
    const _Ty& b23 = second.GetData(1, 2);
    const _Ty& b31 = second.GetData(2, 0);
    const _Ty& b32 = second.GetData(2, 1);
    const _Ty& b33 = second.GetData(2, 2);

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

template <typename _Ty, MatrixMath::StorageOrder order>
MatrixMath::MatrixQ<_Ty, 4, order>
MatrixMath::
operator*(const MatrixQ<_Ty, 4, order>& first, const MatrixQ<_Ty, 4, order>& second)
{
    const _Ty& a11 = first.GetData(0, 0);
    const _Ty& a12 = first.GetData(0, 1);
    const _Ty& a13 = first.GetData(0, 2);
    const _Ty& a14 = first.GetData(0, 3);
    const _Ty& a21 = first.GetData(1, 0);
    const _Ty& a22 = first.GetData(1, 1);
    const _Ty& a23 = first.GetData(1, 2);
    const _Ty& a24 = first.GetData(1, 3);
    const _Ty& a31 = first.GetData(2, 0);
    const _Ty& a32 = first.GetData(2, 1);
    const _Ty& a33 = first.GetData(2, 2);
    const _Ty& a34 = first.GetData(2, 3);
    const _Ty& a41 = first.GetData(3, 0);
    const _Ty& a42 = first.GetData(3, 1);
    const _Ty& a43 = first.GetData(3, 2);
    const _Ty& a44 = first.GetData(3, 3);
    const _Ty& b11 = second.GetData(0, 0);
    const _Ty& b12 = second.GetData(0, 1);
    const _Ty& b13 = second.GetData(0, 2);
    const _Ty& b14 = second.GetData(0, 3);
    const _Ty& b21 = second.GetData(1, 0);
    const _Ty& b22 = second.GetData(1, 1);
    const _Ty& b23 = second.GetData(1, 2);
    const _Ty& b24 = second.GetData(1, 3);
    const _Ty& b31 = second.GetData(2, 0);
    const _Ty& b32 = second.GetData(2, 1);
    const _Ty& b33 = second.GetData(2, 2);
    const _Ty& b34 = second.GetData(2, 3);
    const _Ty& b41 = second.GetData(3, 0);
    const _Ty& b42 = second.GetData(3, 1);
    const _Ty& b43 = second.GetData(3, 2);
    const _Ty& b44 = second.GetData(3, 3);

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

// Scalar de facto
template <typename _Ty, MatrixMath::StorageOrder order>
class MatrixMath::Matrix<_Ty, 1, 1, order>
{
private:
    _Ty data;

public:
    Matrix();
    Matrix(const _Ty&);
    virtual ~Matrix() {}

    void SetData(const _Ty& value);
    const _Ty& GetData() const;
    constexpr bool IsVector() const;
    constexpr bool IsSquare() const;
    constexpr int GetWidth() const;
    constexpr int GetHeight() const;
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
    bool operator==(const _Ty&) const;
    _Ty Transpose() const;
    const std::string ToString() const;
};

template <typename _Ty, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, 1, 1, order>::
Matrix()
    : data{ 0 }
{
}

template <typename _Ty, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, 1, 1, order>::
Matrix(const _Ty& value)
    : data{ value }
{
}

template <typename _Ty, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
SetData(const _Ty& value)
{
    this->data = value;
}

template <typename _Ty, MatrixMath::StorageOrder order>
const _Ty&
MatrixMath::Matrix<_Ty, 1, 1, order>::
GetData() const
{
    return this->data;
}

template <typename _Ty, MatrixMath::StorageOrder order>
constexpr bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
IsVector() const
{
    return false;
}

template <typename _Ty, MatrixMath::StorageOrder order>
constexpr bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
IsSquare() const
{
    return false;
}

template <typename _Ty, MatrixMath::StorageOrder order>
constexpr int
MatrixMath::Matrix<_Ty, 1, 1, order>::
GetWidth() const
{
    return 1;
}

template <typename _Ty, MatrixMath::StorageOrder order>
constexpr int
MatrixMath::Matrix<_Ty, 1, 1, order>::
GetHeight() const
{
    return 1;
}

template <typename _Ty, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator+=(const Matrix& other)
{
    this->data += other.data;
}

template <typename _Ty, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator+=(const _Ty& other)
{
    this->data += other;
}

template <typename _Ty, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator-=(const Matrix& other)
{
    this->data -= other.data;
}

template <typename _Ty, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator-=(const _Ty& other)
{
    this->data -= other;
}

template <typename _Ty, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator*=(const Matrix& multiplier)
{
    this->data *= multiplier.data;
}

template <typename _Ty, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator*=(const _Ty& multiplier)
{
    this->data *= multiplier;
}

template <typename _Ty, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator/=(const Matrix& divider)
{
    this->data /= divider.data;
}

template <typename _Ty, MatrixMath::StorageOrder order>
void
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator/=(const _Ty& divider)
{
    this->data /= divider;
}

template <typename _Ty, MatrixMath::StorageOrder order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator+(const Matrix& other) const
{
    return this->data + other.data;
}

template <typename _Ty, MatrixMath::StorageOrder order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator+(const _Ty& other) const
{
    return this->data + other;
}

template <typename _Ty, MatrixMath::StorageOrder order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator-(const Matrix& other) const
{
    return this->data - other.data;
}

template <typename _Ty, MatrixMath::StorageOrder order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator-(const _Ty& other) const
{
    return this->data - other;
}

template <typename _Ty, MatrixMath::StorageOrder order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator/(const _Ty& divider) const
{
    return this->data / divider;
}

template <typename _Ty, MatrixMath::StorageOrder order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator/(const Matrix& divider) const
{
    return this->data / divider.data;
}

template <typename _Ty, MatrixMath::StorageOrder order>
bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator==(const Matrix& other) const
{
    return this->data == other.data;
}

template <typename _Ty, MatrixMath::StorageOrder order>
bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator==(const _Ty& other) const
{
    return this->data == other;
}

template <typename _Ty, MatrixMath::StorageOrder order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
Transpose() const
{
    return this->data;
}

template<typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator*(const Matrix<_Ty, Height, Width, order>& first, const Scalar<_Ty, order>& second)
{
    Matrix<_Ty, Height, Width, order> result(first);
    result *= second.GetData();
    return result;
}

template<typename _Ty, int Height, int Width, MatrixMath::StorageOrder order>
MatrixMath::Matrix<_Ty, Height, Width, order>
MatrixMath::
operator*(const Scalar<_Ty, order>& first, const Matrix<_Ty, Height, Width, order>& second)
{
    return second * first;
}
