/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/vector/detail/Assing.h"

namespace tl
{

/* Iterator */	

template<typename T>
class IteratorCols
{

public:

    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

private:

    pointer colPtr;
    size_t colSize;

public:

    IteratorCols(pointer ptr, size_t colSize);
    ~IteratorCols() = default;

    auto operator*() const -> reference;
    auto operator->() -> pointer;
    auto operator++() -> IteratorCols&;
    auto operator++(int) -> IteratorCols;

    bool operator == (const IteratorCols &itCol);
    bool operator != (const IteratorCols &itCol);

};



template<typename T, size_t Size = DynamicData>
class MatrixCol
  : public VectorBase<MatrixCol<T, Size>>
{

private:

    T *matrixData;
    size_t matrixCol;
    size_t matrixRows;
    size_t matrixCols;

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    
    using iterator = IteratorCols<T>;
    using const_iterator = IteratorCols<const T>;

public:

    MatrixCol(T *data, size_t col, size_t rows, size_t cols);

    template<VectorExpr Expr>
    auto operator=(const Expr &expr) -> MatrixCol &
    {
        detail::assign_col(*this, expr);
        return *this;
        //TL_ASSERT(expr.size() == size(), "Column size mismatch");

        //if (expr.aliases(matrixData)) {

        //    Vector<T> tmp(expr);

        //    for (size_t i = 0; i < size(); ++i) {
        //        (*this)[i] = tmp[i];
        //    }

        //} else {

        //    for (size_t i = 0; i < size(); ++i) {
        //        (*this)[i] = expr[i];
        //    }
        //}

        //return *this;
    }

    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;
    auto size() const TL_NOEXCEPT -> size_t;
    
    void fill(T value);

    auto operator[](size_t row) const -> const_reference;
    auto operator[](size_t row) -> reference;
/*    auto operator=(const Vector<T> &vector) -> MatrixCol&;
    template<typename T2, size_t _size2>
    auto operator = (const Vector<T2, _size2> &vector) -> MatrixCol&;*/   
    
    //explicit operator Vector<T>();

    auto aliases(const void *ptr) const -> bool
    {
        return matrixData == ptr;
    }
};



/*------------------------------------------------------------------------*/
/* IteratorRows implementation                                            */
/*------------------------------------------------------------------------*/



template<typename T>
IteratorCols<T>::IteratorCols(pointer ptr, size_t colSize)
  : colPtr(ptr),
    colSize(colSize)
{
}

template<typename T>
auto IteratorCols<T>::operator*() const -> reference
{
    return *colPtr;
}

template<typename T>
auto IteratorCols<T>::operator->() -> pointer
{
    return colPtr;
}

template<typename T>
auto IteratorCols<T>::operator++() -> IteratorCols&
{
    colPtr += colSize;
    return *this;
}

template<typename T>
auto IteratorCols<T>::operator++(int) -> IteratorCols
{
    IteratorCols it = *this;
    ++(*this);
    return it;
}

template<typename T>
bool IteratorCols<T>::operator == (const IteratorCols<T> &itCol)
{
    return this->colPtr == itCol.colPtr;
}

template<typename T>
bool IteratorCols<T>::operator != (const IteratorCols<T> &itCol)
{
    return this->colPtr != itCol.colPtr;
}



/*------------------------------------------------------------------------*/
/* MatrixCol implementation                                               */
/*------------------------------------------------------------------------*/

template<typename T, size_t Size>
MatrixCol<T, Size>::MatrixCol(T *data, size_t col, size_t rows, size_t cols)
  : matrixData(data),
    matrixCol(col),
    matrixRows(rows),
    matrixCols(cols)
{
}

template<typename T, size_t Size>
auto MatrixCol<T, Size>::begin() TL_NOEXCEPT -> iterator
{
    return iterator(&matrixData[matrixCol], matrixCols);
}

template<typename T, size_t Size>
auto MatrixCol<T, Size>::begin() const TL_NOEXCEPT -> const_iterator
{
    return iterator(&matrixData[matrixCol], matrixCols);
}

template<typename T, size_t Size>
auto MatrixCol<T, Size>::end() TL_NOEXCEPT -> iterator
{
    return iterator(&matrixData[matrixCol] + matrixRows * matrixCols, matrixCols);
}

template<typename T, size_t Size>
auto MatrixCol<T, Size>::end() const TL_NOEXCEPT -> const_iterator
{
    return iterator(&matrixData[matrixCol] + matrixRows * matrixCols, matrixCols);
}

template<typename T, size_t Size>
auto MatrixCol<T, Size>::size() const TL_NOEXCEPT -> size_t
{
    return matrixRows;
}

template<typename T, size_t Size>
void MatrixCol<T, Size>::fill(T value)
{
    std::fill(begin(), end(), value);
}

template<typename T, size_t Size>
auto MatrixCol<T, Size>::operator[](size_t row) const -> const_reference
{
    return matrixData[row * matrixCols + matrixCol];
}

template<typename T, size_t Size>
auto MatrixCol<T, Size>::operator[](size_t row) -> reference
{
    return matrixData[row * matrixCols + matrixCol];
}

//template<typename T, size_t Size>
//auto MatrixCol<T, Size>::operator=(T value) -> void
//{
//    std::fill(begin(), end(), value);
//}

//template<typename T, size_t Size>
//auto MatrixCol<T, Size>::operator=(const Vector<T> &vector) -> MatrixCol&
//{
//    TL_ASSERT(vector.size() == size(), "Invalid vector size");
//
//    for(size_t i = 0; i < size(); i++)
//        (*this)[i] = vector[i];
//
//    return *this;
//}
//
//template<typename T, size_t Size>
//template<typename T2, size_t _size2>
//auto MatrixCol<T, Size>::operator = (const Vector<T2, _size2> &vector) -> MatrixCol&
//{
//    TL_ASSERT(this->size() == vector.size(), "A size != B size");
//
//    for(size_t i = 0; i < this->size(); i++) {
//        (*this)[i] = static_cast<T>(vector[i]);
//    }
//
//    return *this;
//}

//template<typename T, size_t Size>
//MatrixCol<T, Size>::operator Vector<T>()
//{
//    Vector<T> vector(this->size());
//
//    for(size_t i = 0; i < this->size(); i++) {
//        vector[i] = (*this)[i];
//    }
//
//    return vector;
//}

} // End namespace tl