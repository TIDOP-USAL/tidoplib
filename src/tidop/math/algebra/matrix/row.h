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

#include "tidop/math/base/data.h"
#include "tidop/math/algebra/vector.h"


namespace tl
{
	
/// \cond

namespace internal
{

/* Iterator */	
 
template<typename T>
class IteratorRows
{

public:

    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

private:

    pointer rowPtr;

public:

    explicit IteratorRows(pointer ptr);
    ~IteratorRows() = default;

    auto operator*() const -> reference;
    auto operator->() -> pointer;
    auto operator++() -> IteratorRows&;
    auto operator++(int) -> IteratorRows;

    bool operator== (const IteratorRows &itRow);
    bool operator!= (const IteratorRows &itRow);

}; 



template<typename T, size_t _size_ = DynamicData>
class MatrixRow
  : public VectorBase<MatrixRow<T, _size_>>
{
private:

    T *matrixData;
    size_t matrixRow;
    size_t matrixCols;

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    using iterator = IteratorRows<T>;
    using const_iterator = IteratorRows<const T>;

public:

    MatrixRow(T *data, size_t row, size_t cols);
    //~MatrixRow() = default;

    auto begin() TL_NOEXCEPT->iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT->iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;
    auto size() const TL_NOEXCEPT -> size_t;

    auto operator[](size_t column) const -> const_reference;
    auto operator[](size_t column) -> reference;
    void operator=(T value);
    auto operator=(const Vector<T> &vector) -> MatrixRow&;
    template<typename T2, size_t _size2>
    auto operator = (const Vector<T2, _size2> &vector) -> MatrixRow&;

    operator Vector<T>();

};


/*------------------------------------------------------------------------*/
/* IteratorRows implementation                                            */
/*------------------------------------------------------------------------*/

template<typename T>
IteratorRows<T>::IteratorRows(pointer ptr)
  : rowPtr(ptr)
{
}

template<typename T>
auto IteratorRows<T>::operator*() const -> reference
{
    return *rowPtr;
}

template<typename T>
auto IteratorRows<T>::operator->() -> pointer
{
    return rowPtr;
}

template<typename T>
auto IteratorRows<T>::operator++() -> IteratorRows&
{
    ++rowPtr;
    return *this;
}

template<typename T>
auto IteratorRows<T>::operator++(int) -> IteratorRows
{
    IteratorRows it = *this;
    ++(*this);
    return it;
}

template<typename T>
bool IteratorRows<T>::operator == (const IteratorRows<T> &itRow)
{
    return this->rowPtr == itRow.rowPtr;
}

template<typename T>
bool IteratorRows<T>::operator != (const IteratorRows<T> &itRow)
{
    return this->rowPtr != itRow.rowPtr;
}


/*------------------------------------------------------------------------*/
/* MatrixRow implementation                                               */
/*------------------------------------------------------------------------*/

template<typename T, size_t _size_>
MatrixRow<T, _size_>::MatrixRow(T *data, size_t row, size_t cols)
  : matrixData(data),
    matrixRow(row),
    matrixCols(cols)
{
    this->properties.enable(MatrixRow<T, _size_>::Properties::contiguous_memory);
}

template<typename T, size_t _size_>
auto MatrixRow<T, _size_>::begin() TL_NOEXCEPT -> iterator
{
    return iterator(&matrixData[matrixRow * matrixCols]);
}

template<typename T, size_t _size_>
auto MatrixRow<T, _size_>::begin() const TL_NOEXCEPT -> const_iterator
{
    return iterator(&matrixData[matrixRow * matrixCols]);
}

template<typename T, size_t _size_>
auto MatrixRow<T, _size_>::end() TL_NOEXCEPT -> iterator
{
    return iterator(&matrixData[matrixRow * matrixCols] + matrixCols);
}

template<typename T, size_t _size_>
auto MatrixRow<T, _size_>::end() const TL_NOEXCEPT -> const_iterator
{
  return iterator(&matrixData[matrixRow * matrixCols] + matrixCols);
}

template<typename T, size_t _size_>
auto MatrixRow<T, _size_>::size() const TL_NOEXCEPT -> size_t
{
    return matrixCols;
}

template<typename T, size_t _size_>
auto MatrixRow<T, _size_>::operator[](size_t column) const -> const_reference
{
    return matrixData[matrixRow * matrixCols + column];
}

template<typename T, size_t _size_>
auto MatrixRow<T, _size_>::operator[](size_t column) -> reference
{
    return matrixData[matrixRow * matrixCols + column];
}

template<typename T, size_t _size_>
auto MatrixRow<T, _size_>::operator=(T value) -> void
{
    std::fill(begin(), end(), value);
}

template<typename T, size_t _size_>
auto MatrixRow<T, _size_>::operator=(const Vector<T> &vector) -> MatrixRow&
{
    TL_ASSERT(vector.size() == size(), "Invalid vector size");
    
    for(size_t i = 0; i < size(); i++)
        (*this)[i] = vector[i];
    
    return *this;
}

template<typename T, size_t _size_>
template<typename T2, size_t _size2>
auto MatrixRow<T, _size_>::operator=(const Vector<T2, _size2> &vector) -> MatrixRow &
{
    TL_ASSERT(this->size() == vector.size(), "A size != B size");

    for(size_t i = 0; i < this->size(); i++) {
        (*this)(i) = static_cast<T>(vector(i));
    }

    return *this;
}

template<typename T, size_t _size_>
MatrixRow<T, _size_>::operator Vector<T>()
{
    Vector<T> vector(this->size());

    for(size_t i = 0; i < this->size(); i++) {
        vector[i] = (*this)[i];
    }

    return vector;
}



} // namespace internal

/// \endcond

} // End namespace tl