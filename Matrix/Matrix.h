#pragma once

#include <algorithm>
#include <array>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace MetaMath
{
    template <int LHS, int RHS>
    constexpr static bool LargerThan = LHS > RHS;

    template <int LHS, int RHS>
    constexpr static bool Equal = LHS == RHS;

    template <int Number>
    constexpr static bool IsOdd = (Number & 1) == 1;

    template <int Number>
    constexpr static bool IsEven = (Number & 1) == 0;
}

namespace MatrixMath
{
    struct StorageOrder
    {
        struct RowMajor;
        struct ColumnMajor;

        template <typename order>
        struct CofactorOrder;
    };

    struct StorageOrder::RowMajor : StorageOrder
    {
        inline static int convert2index(const int& Height, const int& Width,
            const int& row, const int& column, const bool& isTransposed);
        inline static constexpr bool IsRowMajor();
        inline static constexpr bool IsColumnMajor();
    };

    struct StorageOrder::ColumnMajor : StorageOrder
    {
        inline static int convert2index(const int& Height, const int& Width,
            const int& row, const int& column, const bool& isTransposed);
        inline static constexpr bool IsRowMajor();
        inline static constexpr bool IsColumnMajor();
    };

    // DOES NOTHING
    struct AbstractCofactor {};

    template <typename order>
    struct StorageOrder::CofactorOrder
        : AbstractCofactor
        , order
    {
        //
        // Height:
        //      the height of the parent matrix of the cofactor
        // Width:
        //      the width of the parent matrix of the cofactor
        // row:
        //      the relative coordinate in the cofactor
        // column:
        //      the relative coordinate in the cofactor
        // RowSrc, RowDst, ColSrc, ColDst:
        //      the absolute coordinate in the parent matrix of the cofactor
        //
        inline static int convert2index(const int& Height, const int& Width,
            const int& row, const int& column, const bool& isTransposed,
            int RowSrc, int RowDst, int ColSrc, int ColDst);
        inline static int convert2index(const int& Height, const int& Width,
            const int& row, const int& column, const bool& isTransposed);
    };

    // INTERFACE OF ALL KINDS OF MATRICES
    // Use this interface as the base of
    // all the "de facto" matrices, vectors, etc.
    template <typename _Ty>
    class IMatrix
    {
        virtual inline void SetElement(const int& index, const _Ty& value) = 0;
        virtual inline void SetElement(const int& row, const int& column, const _Ty& value) = 0;
        virtual inline const _Ty& GetElement(const int& index) const = 0;
        virtual inline const _Ty& GetElement(const int& row, const int& column) const = 0;
        virtual inline _Ty& GetElement(const int& index) = 0;
        virtual inline _Ty& GetElement(const int& row, const int& column) = 0;
    };

    template <typename _Ty, int Height, int Width, typename order>
    class ProtoMatrix
    {
        static_assert(Width > 0, "Template argument 'Width' has negative value!");
        static_assert(Height > 0, "Template argument 'Height' has negative value!");
        static_assert(std::is_base_of_v<StorageOrder, order>, "Template argument 'order' is invalid type!");

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
    private:
        std::shared_ptr<std::array<_Ty, Width * Height>> pData; // 'Width * Height' here cannot be replaced by 'Size',
                                                                // otherwise a compiler error (C2244) will be thrown at compile time
                                                                // if the project is compiled with Microsoft VC++
        bool isTransposed;

    protected:
        ProtoMatrixData();
        ProtoMatrixData(const ProtoMatrixData& other);
        ProtoMatrixData(const ProtoMatrixData&& other);
        ProtoMatrixData(const std::initializer_list<_Ty>& init);
        ~ProtoMatrixData();

        inline const std::shared_ptr<std::array<_Ty, Width * Height>>& GetDataPointer() const;
        inline std::shared_ptr<std::array<_Ty, Width * Height>>& GetDataPointer();

    public:
        void Transpose();
        bool IsTransposed() const;
        inline const std::array<_Ty, Width * Height>& GetData() const;
        inline std::array<_Ty, Width * Height>& GetData();

#ifdef _DEBUG
        std::string name;
#endif
    };

    template <typename _Ty, int Height, int Width, typename order = StorageOrder::RowMajor>
    class Matrix
        : public ProtoMatrixData<_Ty, Height, Width, order>
        , public IMatrix<_Ty>
    {
    public:
        using DataType = ProtoMatrixData<_Ty, Height, Width, order>;
        using Transposed = Matrix<_Ty, Width, Height, order>;

        Matrix();
        Matrix(const Matrix& other);
        Matrix(const Matrix&& other);
        Matrix(const std::initializer_list<_Ty>& init);

        // Access data

        inline void SetElement(const int& index, const _Ty& value);
        inline const _Ty& GetElement(const int& index) const;
        inline void SetElement(const int& row, const int& column, const _Ty& value);
        inline const _Ty& GetElement(const int& row, const int& column) const;
        inline _Ty& GetElement(const int& index);
        inline _Ty& GetElement(const int& row, const int& column);

        // Always output a string representing the matrix in row-major order
        const std::string ToString() const;

        template <int RowSrc, int RowDst, int ColSrc, int ColDst>
        class SubMatrix;

        template <int RowSrc, int RowDst, int ColSrc, int ColDst>
        SubMatrix<RowSrc, RowDst, ColSrc, ColDst> GetSubMatrix();

        template <int Row, int Column>
        class Refactor;
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

    // Valid only if the arguments are matrix de facto
    template <typename _Ty, int Height, int Width, typename order,
        std::enable_if_t<MetaMath::LargerThan<Height, 1> && MetaMath::LargerThan<Width, 1>, int> = 0>
    bool operator==(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs);

    // Valid only if the arguments are vector de facto
    template <typename _Ty, int Height, int Width, typename order,
        std::enable_if_t<MetaMath::LargerThan<Height, 1> && MetaMath::Equal<Width, 1>
        || MetaMath::LargerThan<Width, 1> && MetaMath::Equal<Height, 1>, int> = 0>
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

    // Advanced algorithms

    template <typename _Ty, int N, typename order>
    class Determinant;

    // Storage order utility

    template <typename NewOrder, typename _Ty, int Height, int Width, typename OldOrder>
    Matrix<_Ty, Height, Width, NewOrder> ChangeOrder(const Matrix<_Ty, Height, Width, OldOrder>& other);

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
ProtoMatrixData()                                   // default ctor
    : pData{ std::make_unique<std::array<_Ty, Width * Height>>() } // initialize std::array pointer with nullptr
    , isTransposed{ false }
{
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
ProtoMatrixData(const ProtoMatrixData& other)       // copy ctor
    : ProtoMatrixData()
{
    std::copy(other.pData->begin(), other.pData->end(), this->pData->begin());
    this->isTransposed = other.isTransposed;
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
ProtoMatrixData(const ProtoMatrixData&& other)      // move ctor
    : pData{ other.pData }
    , isTransposed{ other.isTransposed }
{
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
ProtoMatrixData(const std::initializer_list<_Ty>& init)
    : ProtoMatrixData() // allocate the destination array/buffer of copy operation
{
    const _Ty* src{ init.begin() };
    const _Ty* dst{ init.end() };
    // prevent buffer overflow attack
    const _Ty* end{ src + std::min<ptrdiff_t>(dst - src, Width * Height) };
    std::copy(src, end, this->pData->begin());
    this->isTransposed = false;
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
~ProtoMatrixData()
{
#ifdef _DEBUG
    std::cout
        << "Deleting ProtoMatrixData{name: "
        << this->name
        << "} ..."
        << std::endl;
#endif
}

template <typename _Ty, int Height, int Width, typename order>
inline
const std::shared_ptr<std::array<_Ty, Width * Height>>&
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
GetDataPointer() const
{
    return this->pData;
}

template <typename _Ty, int Height, int Width, typename order>
inline
std::shared_ptr<std::array<_Ty, Width * Height>>&
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
GetDataPointer()
{
    return this->pData;
}

template <typename _Ty, int Height, int Width, typename order>
void
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
Transpose()
{
    this->isTransposed = !this->isTransposed;
}

template <typename _Ty, int Height, int Width, typename order>
bool
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
IsTransposed() const
{
    return this->isTransposed;
}

template <typename _Ty, int Height, int Width, typename order>
inline
const std::array<_Ty, Width * Height>&
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
GetData() const
{
    return *(GetDataPointer());
}

template <typename _Ty, int Height, int Width, typename order>
inline
std::array<_Ty, Width * Height>&
MatrixMath::ProtoMatrixData<_Ty, Height, Width, order>::
GetData()
{
    return *(GetDataPointer());
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix()
    : DataType()
{
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix(const Matrix& other)
    : DataType(other)
{
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix(const Matrix&& other)
    : DataType(std::move(other))
{
}

template <typename _Ty, int Height, int Width, typename order>
MatrixMath::Matrix<_Ty, Height, Width, order>::
Matrix(const std::initializer_list<_Ty>& init)
    : DataType(init)
{
}

template <typename _Ty, int Height, int Width, typename order>
inline void
MatrixMath::Matrix<_Ty, Height, Width, order>::
SetElement(const int& index, const _Ty& value)
{
    auto& data{ this->GetData() };
    data[index] = value;
}

template <typename _Ty, int Height, int Width, typename order>
inline const _Ty&
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetElement(const int& index) const
{
    auto& data{ this->GetData() };
    return data[index];
}

template <typename _Ty, int Height, int Width, typename order>
inline void
MatrixMath::Matrix<_Ty, Height, Width, order>::
SetElement(const int& row, const int& column, const _Ty& value)
{
    const int index{ order::convert2index(Height, Width, row, column, this->IsTransposed()) };
    this->SetElement(index, value);
}

template <typename _Ty, int Height, int Width, typename order>
inline const _Ty&
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetElement(const int& row, const int& column) const
{
    const int index{ order::convert2index(Height, Width, row, column, this->IsTransposed()) };
    return this->GetElement(index);
}

template <typename _Ty, int Height, int Width, typename order>
inline _Ty&
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetElement(const int& index)
{
    auto& data{ this->GetData() };
    return data[index];
}

template <typename _Ty, int Height, int Width, typename order>
inline _Ty&
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetElement(const int& row, const int& column)
{
    const int index{ order::convert2index(Height, Width, row, column, this->IsTransposed()) };
    return this->GetElement(index);
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

template <typename _Ty, int Height, int Width, typename order>
template <int RowSrc, int RowDst, int ColSrc, int ColDst>
class MatrixMath::Matrix<_Ty, Height, Width, order>::SubMatrix
    : public ProtoMatrix<_Ty, RowDst - RowSrc, ColDst - ColSrc, order>
    , public IMatrix<_Ty>
{
    static_assert(RowSrc >= 0, "Invalid argument: RowSrc < 0!");
    static_assert(RowDst <= Height, "Invalid argument: RowDst > Height!");
    static_assert(ColSrc >= 0, "Invalid argument: ColSrc < 0!");
    static_assert(ColDst <= Width, "Invalid argument: ColDst > Width!");
    static_assert(RowSrc < RowDst, "Invalid argument: RowSrc >= RowDst!");
    static_assert(ColSrc < ColDst, "Invalid argument: ColSrc >= ColDst!");

public:
    using ParentType = Matrix<_Ty, Height, Width, order>;
    using OrderType = StorageOrder::CofactorOrder<order>;

private:
    ParentType& parent;

    inline static int convert2index(const int& row, const int& column, const bool& isTransposed)
    {
        const int index{ OrderType::convert2index(Height, Width,
            row, column, isTransposed,
            RowSrc, RowDst, ColSrc, ColDst) };
        return index;
    }

    inline static int convert2index(const int& index, const bool& isTransposed)
    {
        int row{ 0 }, column{ 0 };
        if (OrderType::IsRowMajor() && isTransposed
            || OrderType::IsColumnMajor() && !isTransposed)
        {
            // index = row + column * Height
            row = index % Height;
            column = index / Height;
        }
        else
        {
            // index = column + row * Width
            row = index / Width;
            column = index % Width;
        }
        return SubMatrix::convert2index(row, column, isTransposed);
    }

    inline int convert2index(const int& row, const int& column) const
    {
        return SubMatrix::convert2index(row, column, parent.IsTransposed());
    }

    inline int convert2index(const int& index) const
    {
        return SubMatrix::convert2index(index, parent.IsTransposed());
    }

public:
    SubMatrix(ParentType& parent)
        : parent{ parent }
    {
    }

    const ParentType& GetParent() const
    {
        return parent;
    }

    ParentType& GetParent()
    {
        return parent;
    }

    inline bool IsTransposed() const
    {
        return parent.IsTransposed();
    }

    // Access data

    inline void SetElement(const int& index, const _Ty& value)
    {
        this->parent.SetElement(this->convert2index(index), value);
    }

    inline const _Ty& GetElement(const int& index) const
    {
        return this->parent.GetElement(this->convert2index(index));
    }

    inline void SetElement(const int& row, const int& column, const _Ty& value)
    {
        this->parent.SetElement(this->convert2index(row, column), value);
    }

    inline const _Ty& GetElement(const int& row, const int& column) const
    {
        return this->parent.GetElement(this->convert2index(row, column));
    }

    inline _Ty& GetElement(const int& index)
    {
        return this->parent.GetElement(this->convert2index(index));
    }

    inline _Ty& GetElement(const int& row, const int& column)
    {
        return this->parent.GetElement(this->convert2index(row, column));
    }

    const std::string ToString() const
    {
        std::stringstream ss;

        for (int row = 0; row < this->GetHeight(); row++)
        {
            ss << "| ";
            for (int col = 0; col < this->GetWidth(); col++)
            {
                ss << this->GetElement(row, col)
                    << " ";
            }
            ss << "|"
                << std::endl;
        }

        return ss.str();
    }
};

template <typename _Ty, int Height, int Width, typename order>
template <int RowSrc, int RowDst, int ColSrc, int ColDst>
MatrixMath::Matrix<_Ty, Height, Width, order>::SubMatrix<RowSrc, RowDst, ColSrc, ColDst>
MatrixMath::Matrix<_Ty, Height, Width, order>::
GetSubMatrix()
{
    return SubMatrix<RowSrc, RowDst, ColSrc, ColDst>(*this);
}


template <typename _Ty, int Height, int Width, typename order>
template <int Row, int Column>
class MatrixMath::Matrix<_Ty, Height, Width, order>::Refactor
    : public ProtoMatrix<_Ty, Height - 1, Width - 1, order>
    , public IMatrix<_Ty>
{
    static_assert(Row >= 0, "Invalid template argument: Row < 0!");
    static_assert(Row < Height, "Invalid template argument: Row >= Height!");
    static_assert(Column >= 0, "Invalid template argument: Column < 0!");
    static_assert(Column < Width, "Invalid template argument: Column < Width!");

public:
    using ParentType = Matrix<_Ty, Height, Width, order>;

private:
    ParentType& parent;
    std::vector<std::unique_ptr<IMatrix<_Ty>>> subMatrices;

public:
    Refactor(ParentType& parent)
        : parent{ parent }
    {
    }

    const ParentType& GetParent() const
    {
        return parent;
    }

    ParentType& GetParent()
    {
        return parent;
    }

    inline bool IsTransposed() const
    {
        return parent.IsTransposed();
    }

};


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

template <typename _Ty, int Height, int Width, typename order,
    std::enable_if_t<MetaMath::LargerThan<Height, 1> && MetaMath::LargerThan<Width, 1>, int>>
bool
MatrixMath::
operator==(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs)
{
    if (&lhs == &rhs) return true;
    auto& lhsd{ lhs.GetData() };
    auto& rhsd{ rhs.GetData() };
    if ((&lhsd == &rhsd || lhsd == rhsd) && lhs.IsTransposed() == rhs.IsTransposed())
        return true;
    for (int row = 0; row < Height; row++)
    {
        for (int col = 0; col < Width; col++)
        {
            if (lhs.GetElement(row, col) != rhs.GetElement(row, col))
                return false;
        }
    }
    return true;
}

template <typename _Ty, int Height, int Width, typename order,
    std::enable_if_t<MetaMath::LargerThan<Height, 1> && MetaMath::Equal<Width, 1>
    || MetaMath::LargerThan<Width, 1> && MetaMath::Equal<Height, 1>, int>>
bool
MatrixMath::
operator==(const Matrix<_Ty, Height, Width, order>& lhs, const Matrix<_Ty, Height, Width, order>& rhs)
{
    if (&lhs == &rhs) return true;
    auto& lhsd{ lhs.GetData() };
    auto& rhsd{ rhs.GetData() };
    if ((&lhsd == &rhsd || lhsd == rhsd) && lhs.IsTransposed() == rhs.IsTransposed())
        return true;
    for (int index = 0; index < Width * Height; index++)
    {
        if (lhs.GetElement(index) != rhs.GetElement(index))
            return false;
    }
    return true;
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

    if constexpr (order::IsRowMajor())
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

    if constexpr (order::IsRowMajor())
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

    if constexpr (order::IsRowMajor())
        return MatrixQ<_Ty, 4, order>{ c11, c12, c13, c14, c21, c22, c23, c24, c31, c32, c33, c34, c41, c42, c43, c44 };
    else
        return MatrixQ<_Ty, 4, order>{ c11, c21, c31, c41, c12, c22, c32, c42, c13, c23, c33, c43, c14, c24, c34, c44 };
}

// Vector de facto
template <typename _Ty, int N, typename order>
class MatrixMath::Matrix<_Ty, N, 1, order>
    : public MatrixMath::ProtoMatrixData<_Ty, N, 1, order>
    , public IMatrix<_Ty>
{
public:
    using DataType = ProtoMatrixData<_Ty, N, 1, order>;
    using Transposed = Matrix<_Ty, 1, N, order>;

    Matrix();
    Matrix(const Matrix& other);
    Matrix(const Matrix&& other);
    Matrix(const std::initializer_list<_Ty>& init);

    inline void SetElement(const int& index, const _Ty& value);
    inline void SetElement(const int& row, const int& column, const _Ty& value);
    inline const _Ty& GetElement(const int& index) const;
    inline const _Ty& GetElement(const int& row, const int& column) const;
    inline _Ty& GetElement(const int& index);
    inline _Ty& GetElement(const int& row, const int& column);

    const std::string ToString() const;
};

template <typename _Ty, int N, typename order>
MatrixMath::Matrix<_Ty, N, 1, order>::
Matrix()
    : DataType()
{
}

template <typename _Ty, int N, typename order>
MatrixMath::Matrix<_Ty, N, 1, order>::
Matrix(const Matrix& other)
    : DataType(other)
{
}

template <typename _Ty, int N, typename order>
MatrixMath::Matrix<_Ty, N, 1, order>::
Matrix(const Matrix&& other)
    : DataType(std::move(other))
{
}

template <typename _Ty, int N, typename order>
MatrixMath::Matrix<_Ty, N, 1, order>::
Matrix(const std::initializer_list<_Ty>& init)
    : DataType(init)
{
}

template <typename _Ty, int N, typename order>
inline void
MatrixMath::Matrix<_Ty, N, 1, order>::
SetElement(const int& index, const _Ty& value)
{
    auto& data{ this->GetData() };
    data[index] = value;
}

template <typename _Ty, int N, typename order>
inline void
MatrixMath::Matrix<_Ty, N, 1, order>::
SetElement(const int& row, const int& column, const _Ty& value)
{
    this->SetElement(row, value);
}

template <typename _Ty, int N, typename order>
inline const _Ty&
MatrixMath::Matrix<_Ty, N, 1, order>::
GetElement(const int& index) const
{
    auto& data{ this->GetData() };
    return data[index];
}

template <typename _Ty, int N, typename order>
inline const _Ty&
MatrixMath::Matrix<_Ty, N, 1, order>::
GetElement(const int& row, const int& column) const
{
    return this->GetElement(row);
}

template <typename _Ty, int N, typename order>
inline _Ty&
MatrixMath::Matrix<_Ty, N, 1, order>::
GetElement(const int& index)
{
    auto& data{ this->GetData() };
    return data[index];
}

template <typename _Ty, int N, typename order>
inline _Ty&
MatrixMath::Matrix<_Ty, N, 1, order>::
GetElement(const int& row, const int& column)
{
    return this->GetElement(row);
}

template <typename _Ty, int N, typename order>
const std::string
MatrixMath::Matrix<_Ty, N, 1, order>::
ToString() const
{
    auto& data{ this->GetData() };
    std::stringstream ss;

    ss << "| ";
    for (const auto& x : data)
    {
        ss << x << " ";
    }
    ss << "|T";

    return ss.str();
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

    void SetElement(const _Ty& value);
    const _Ty& GetElement() const;
    const _Ty& GetData() const;
    _Ty& GetData();
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
    _Ty operator*(const Matrix&) const;
    _Ty operator*(const _Ty&) const;
    _Ty operator/(const Matrix&) const;
    _Ty operator/(const _Ty&) const;
    bool operator==(const Matrix&) const;
    bool operator!=(const Matrix&) const;
    template <typename NumberType> bool operator==(const NumberType&) const;
    template <typename NumberType> bool operator!=(const NumberType&) const;
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
SetElement(const _Ty& value)
{
    this->data = value;
}

template <typename _Ty, typename order>
const _Ty&
MatrixMath::Matrix<_Ty, 1, 1, order>::
GetElement() const
{
    return this->data;
}

template <typename _Ty, typename order>
const _Ty&
MatrixMath::Matrix<_Ty, 1, 1, order>::
GetData() const
{
    return this->data;
}

template <typename _Ty, typename order>
_Ty&
MatrixMath::Matrix<_Ty, 1, 1, order>::
GetData()
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
operator*(const Matrix& other) const
{
    return this->data * other.data;
}


template <typename _Ty, typename order>
_Ty
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator*(const _Ty& other) const
{
    return this->data * other;
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
template <typename NumberType>
bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator==(const NumberType& other) const
{
    return this->data == other;
}

template <typename _Ty, typename order>
template <typename NumberType>
bool
MatrixMath::Matrix<_Ty, 1, 1, order>::
operator!=(const NumberType& other) const
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


inline int
MatrixMath::StorageOrder::RowMajor::
convert2index(const int& Height, const int& Width,
    const int& row, const int& column, const bool& isTransposed)
{
    if (isTransposed)   return row + column * Height;
    else                return column + row * Width;
}

inline constexpr bool
MatrixMath::StorageOrder::RowMajor::
IsRowMajor()
{
    return true;
}

inline constexpr bool
MatrixMath::StorageOrder::RowMajor::
IsColumnMajor()
{
    return false;
}

inline int
MatrixMath::StorageOrder::ColumnMajor::
convert2index(const int& Height, const int& Width,
    const int& row, const int& column, const bool& isTransposed)
{
    if (isTransposed)   return column + row * Width;
    else                return row + column * Height;
}

inline constexpr bool
MatrixMath::StorageOrder::ColumnMajor::
IsRowMajor()
{
    return false;
}

inline constexpr bool
MatrixMath::StorageOrder::ColumnMajor::
IsColumnMajor()
{
    return true;
}

template <typename order>
inline int
MatrixMath::StorageOrder::CofactorOrder<order>::
convert2index(const int& Height, const int& Width,
    const int& row, const int& column, const bool& isTransposed,
    int RowSrc, int RowDst, int ColSrc, int ColDst)
{
    return order::convert2index(Height, Width, row + RowSrc, column + ColSrc, isTransposed);
}

template <typename order>
inline int
MatrixMath::StorageOrder::CofactorOrder<order>::
convert2index(const int& Height, const int& Width,
    const int& row, const int& column, const bool& isTransposed)
{
    return convert2index(Height, Width, row, column, isTransposed,
        0, Height, 0, Width);
}

namespace detail
{
    template <int N>
    struct Factorial
    {
        static_assert(N >= 0, "Invalid template argument: N < 0!");
        constexpr static int value{ N * Factorial<N - 1>::value };
    };
    template <> struct Factorial<0> { constexpr static int value{ 1 }; };

    // Permutation generation in lexicographic order
    // @see: https://en.wikipedia.org/wiki/Permutation#Algorithms_to_generate_permutations
    template <int Size>
    struct PermutationGenerator
    {
        struct MyArray
        {
            std::array<int, Size> data;
            int inverse;

#pragma warning(push)
#pragma warning(disable: 26495)
            constexpr MyArray()
                : inverse{ 0 }
            {
                for (int i = 0; i < Size; i++)
                    data[i] = i;
            }
#pragma warning(pop)

            constexpr void swap(const int& lhs, const int& rhs)
            {
                std::swap(data[lhs], data[rhs]);
                ++inverse;
            }

            constexpr void sort(const int& src, const int& dst)
            {
                const int len{ dst - src };
                for (int i = 0; i < len; i++)
                {
                    for (int j = i; j > 0 && data[src + j] < data[src + j - 1]; j--)
                    {
                        this->swap(src + j, src + j - 1);
                    }
                }
            }

            constexpr int& operator[](const int& index)
            {
                return data[index];
            }

            constexpr auto begin() -> decltype(data.begin())
            {
                return data.begin();
            }

            constexpr auto end() -> decltype(data.end())
            {
                return data.end();
            }
        };

        using ArrayType = MyArray;
        constexpr static int Count{ Factorial<Size>::value };
        using ResultType = std::array<ArrayType, Count>;

        constexpr static ResultType generate(ArrayType& seq)
        {
            ResultType result;

            for (int p = 0; p < Factorial<Size>::value; p++)
            {
                ArrayType& target{ result[p] };
                std::copy(seq.begin(), seq.end(), target.begin());
                target.inverse = seq.inverse;

#pragma warning(push)
#pragma warning(disable: 26451)
                int i{ Size - 2 };
                while (i >= 0 && seq[i] >= seq[i + 1]) --i;
                if (i == -1) break;
                int j{ i + 1 };
                for (int k = j + 1; k <= Size - 1; k++)
                    if (seq[k] > seq[i] && seq[k] < seq[j])
                        j = k;
                seq.swap(i, j);
                //int* src{ &seq[i + 1] };
                //int* dst{ &seq[0] + Size };
                //std::sort(src, dst);
                seq.sort(i + 1, Size);
#pragma warning(pop)
            }

            return result;
        }

        constexpr static ResultType generate()
        {
            ArrayType sequence;
            ResultType result{ generate(sequence) };
            /*
            {
                for (auto& p : result)
                {
                    for (auto& i : p)
                    {
                        std::cout << i << ", ";
                    }
                    std::cout << " -- " << p.inverse << std::endl;
                }
                std::cout << std::endl;
            }
            */
            return result;
        }
    };

}

template <typename _Ty, int N, typename order>
class MatrixMath::Determinant
{
private:
    _Ty result;

public:
    Determinant(const Matrix<_Ty, N, N, order>& square);

    operator _Ty() const;
};

template <typename _Ty, int N, typename order>
MatrixMath::Determinant<_Ty, N, order>::
Determinant(const Matrix<_Ty, N, N, order>& square)
    : result{ 0 }
{
    for (auto& p : detail::PermutationGenerator<N>::generate())
    {
        _Ty cache{ (p.inverse & 0x1) ? -1 : 1 };
        //std::cout << std::setw(2) << cache;
        for (int i = 0; i < N; i++)
        {
            const _Ty& element{ square.GetElement(i, p[i]) };
            cache *= element;
            //std::cout << " * (" << i << ", " << p[i] << ": " << element << ")";
        }
        result += cache;
        //std::cout << " = " << cache << std::endl;
    }
}

template <typename _Ty, int N, typename order>
MatrixMath::Determinant<_Ty, N, order>::
operator _Ty() const
{
    return result;
}


template <typename NewOrder, typename _Ty, int Height, int Width, typename OldOrder>
MatrixMath::Matrix<_Ty, Height, Width, NewOrder>
MatrixMath::
ChangeOrder(const Matrix<_Ty, Height, Width, OldOrder>& other)
{
    static_assert(std::is_base_of_v<StorageOrder, OldOrder>
        && std::is_base_of_v<StorageOrder, NewOrder>,
        "Invalid template argument: Not a derived of StorageOrder!");

    static_assert(!std::is_base_of_v<AbstractCofactor, NewOrder>,
        "Invalid template argument: It is not allowed to change into CofactorOrder!");

    static_assert(!std::is_base_of_v<AbstractCofactor, OldOrder>,
        "Invalid template argument: It is not allowed to change from CofactorOrder!");

    if constexpr (std::is_same_v<NewOrder, OldOrder>)
        return other;

    using NewType = Matrix<_Ty, Height, Width, NewOrder>;

    NewType result;
    for (int row = 0; row < Height; row++)
        for (int col = 0; col < Width; col++)
            result.SetElement(row, col,
                other.GetElement(row, col));
    return result;
}
