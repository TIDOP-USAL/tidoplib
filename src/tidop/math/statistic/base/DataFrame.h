/**************************************************************************
 *                                                                        *
 * Copyright (C) 2026 by Tidop Research Group                             *
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

/*! \file DataFrame.h
 * \brief In‑memory tabular data structure (data frame).
 *
 * This file defines the `DataFrame` class, which stores columns of homogeneous
 * data types (similar to a spreadsheet or a database table). Each column has a
 * name and contains a sequence of values of a single type. Columns can be added
 * dynamically, and data can be accessed as `std::span` for efficient read‑only
 * access. The class is move‑only (copy is disabled) to avoid expensive copies.
 *
 * \ingroup Data
 * \see tl::Series, tl::Column
 */

#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <span>
#include <memory>

#include "tidop/core/base/Path.h"
#include "tidop/core/base/Exception.h"
#include "tidop/core/base/Concepts.h"
#include "tidop/core/base/Type.h"
#include "tidop/math/statistic/base/Series.h"

namespace tl
{

class DataFrame;

namespace detail
{

class Column
{

public:

    virtual ~Column() = default;
    virtual size_t size() const noexcept = 0;
    virtual Type type() const noexcept = 0;
    virtual const void *data() const noexcept = 0;
};

template<typename T>
class TypedColumn
  : public Column
{

private:

    std::vector<T> mData;

public:

    TypedColumn(std::vector<T> &&data)
        : mData(std::move(data))
    {
    }

    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t override { return mData.size(); }

    [[nodiscard]]
    auto type() const noexcept -> Type override 
    {
        return TypeTraits<T>::id_type;
    }

    [[nodiscard]]
    auto data() const noexcept -> const void *override
    {
        return mData.data();
    }

};

} // namespace detail


struct ColumnInfo
{
    std::string name;
    Type type;
    size_t size;
};


class RowView
{

private:

    const DataFrame &mDf;
    size_t mRowIndex;

public:

    RowView(const DataFrame &df, size_t rowIndex)
      : mDf(df),
        mRowIndex(rowIndex)
    {
    }

    // Acceso por nombre de columna: row["Precios"]
    template <typename T>
    auto get(std::string_view col_name) const -> const T &;

    // Acceso por índice numérico de columna: row[0]
    template <typename T>
    auto get(size_t col_index) const -> const T &;

    auto index() const noexcept -> size_t { return mRowIndex; }
};

class RowIterator
{

private:

    const DataFrame &mDf;
    size_t mCurrentRow;

public:

    using iterator_category = std::forward_iterator_tag;
    using value_type = RowView;
    using difference_type = std::ptrdiff_t;
    using pointer = RowView *;
    using reference = RowView; // Devuelve por valor porque RowView es una vista ligera

    RowIterator(const DataFrame &df, size_t row)
      : mDf(df),
        mCurrentRow(row)
    {
    }

    auto operator*() const -> RowView 
    {
        return RowView(mDf, mCurrentRow); 
    }

    auto operator++() -> RowIterator &
    {
        ++mCurrentRow;
        return *this;
    }

    auto operator++(int) -> RowIterator
    {
        RowIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    auto operator==(const RowIterator &other) const -> bool
    {
        return mCurrentRow == other.mCurrentRow;
    }
};


// Objeto ayudante para soportar la sintaxis range-based for: df.rows()
class RowRange
{

private:

    const DataFrame &mDf;

public:

    RowRange(const DataFrame &df);
    auto begin() const -> RowIterator;
    auto end() const -> RowIterator;
};




/*!
 * \class DataFrame
 * \brief Tabular data structure with named columns of possibly different types.
 *
 * A `DataFrame` stores a collection of columns. Each column has a unique name
 * and contains a sequence of values of a single type. Columns are added using
 * `insert()`, and data can be retrieved as a `std::span<const T>` using the
 * `column()` method.
 *
 * The class is move‑only (copy constructor and copy assignment are deleted)
 * to prevent accidental expensive copies. All columns have the same length
 * (number of rows), which is determined when the first column is inserted.
 * Subsequent columns must match that length.
 *
 * ### Example
 * \code
 * DataFrame df;
 * std::vector<int> ages = {25, 30, 35};
 * df.insert("age", ages);
 *
 * std::vector<double> scores = {85.5, 92.0, 78.3};
 * df.insert("score", scores);
 *
 * auto age_col = df.column<int>("age");
 * for (int a : age_col) std::cout << a << "\n";
 * \endcode
 */
class DataFrame
{

private:

    std::vector<std::string> mColNames;                           /*!< Column names in insertion order. */
    std::vector<std::unique_ptr<detail::Column>> mColumns;        /*!< Pointers to typed column objects. */
    std::unordered_map<std::string, size_t> mColIndex;            /*!< Name → index map. */

public:

    DataFrame() = default;
    DataFrame(DataFrame &&) noexcept = default;
    DataFrame &operator=(DataFrame &&) noexcept = default;

    DataFrame(const DataFrame &) = delete;
    DataFrame &operator=(const DataFrame &) = delete;

    ~DataFrame() = default;

    /*!
     * \brief Inserts a new column into the data frame.
     * \tparam R A type satisfying `ColumnInput` (an input range).
     * \param[in] name The name of the column (must be unique).
     * \param[in] range The range of values to fill the column.
     * \throws `tl::Exception` if a column with the same name already exists,
     *         or if the range length does not match the existing number of rows.
     *
     * The column’s type is deduced from the range’s value type (`std::ranges::range_value_t<R>`).
     * If the data frame is empty, the length of the first column determines the number of rows.
     * Subsequent columns must have exactly the same number of rows.
     */
    template<typename R>
        requires(std::ranges::input_range<R>)
    void insert(std::string_view name, R &&range);

    /*!
     * \brief Retrieves a column as a read‑only span.
     * \tparam T The expected element type of the column.
     * \param[in] name The column name.
     * \return A `std::span<const T>` containing the column data.
     * \throws `tl::Exception` if the column does not exist or if the type `T`
     *         does not match the stored column type.
     *
     * The span provides contiguous, read‑only access to the column elements.
     * The data remains owned by the `DataFrame`; the span is valid as long as
     * the `DataFrame` is not moved or destroyed.
     */
    template <typename T>
    [[nodiscard]]
    auto column(std::string_view name) const -> std::span<const T>;

    template <typename T>
    [[nodiscard]]
    auto column(size_t index) const -> std::span<const T>;

    /*!
     * \brief Returns the number of columns.
     * \return Number of columns.
     */
    [[nodiscard]]
    auto cols() const noexcept -> size_t;

    /*!
     * \brief Returns the number of rows (length of each column).
     * \return Number of rows (0 if no columns).
     */
    [[nodiscard]]
    auto size() const noexcept -> size_t;

    /*!
     * \brief Returns the list of column names in insertion order.
     * \return Const reference to a vector of column names.
     */
    [[nodiscard]]
    auto columns() const noexcept -> const std::vector<std::string> &;

    [[nodiscard]]
    auto row(size_t i) const -> RowView;

    // Permite la sintaxis: for (auto row : df.rows())
    [[nodiscard]]
    auto rows() const -> RowRange;


    auto columnType(size_t index) const -> Type
    {
        TL_ASSERT(index < mColumns.size(), "Column index {} out of bounds", index);

        return mColumns[index]->type();
    }

    auto columnType(std::string_view name) const -> Type
    {
        auto it = mColIndex.find(std::string(name));
        return this->columnType(it->second);
    }

    auto info() const -> std::vector<ColumnInfo>
    {
        std::vector<ColumnInfo> result;
        result.reserve(cols());

        for (size_t i = 0; i < cols(); ++i) {
            result.push_back({
                mColNames[i],
                mColumns[i]->type(),
                mColumns[i]->size()
                });
        }

        return result;
    }
};


/*! \} */

template<typename R>
    requires(std::ranges::input_range<R>)
inline void DataFrame::insert(std::string_view name, R &&range)
{
    using T = std::ranges::range_value_t<R>;

    std::string col_name(name);

    TL_ASSERT(!mColIndex.contains(col_name), "DataFrame: Column '{}' already exists", name);

    std::vector<T> data;
    if constexpr (std::ranges::sized_range<R>) {
        data.reserve(std::ranges::size(range));
    }

    for (auto &&v : range)
        data.emplace_back(static_cast<T>(v));

    if (!mColumns.empty()) {
        TL_ASSERT(data.size() == size(), "DataFrame: Length mismatch. Expected {}, got {}", size(), data.size());
    }

    mColNames.emplace_back(col_name);
    mColumns.push_back(std::make_unique<detail::TypedColumn<T>>(std::move(data)));
    mColIndex[col_name] = mColumns.size() - 1;
}

template<typename T>
inline auto DataFrame::column(std::string_view name) const -> std::span<const T>
{
    auto it = mColIndex.find(std::string(name));

    TL_ASSERT(it != mColIndex.end(), "Column not found");

    return column<T>(it->second);
}

template<typename T>
inline auto DataFrame::column(size_t index) const -> std::span<const T>
{
    TL_ASSERT(index < mColumns.size(), "Column index {} out of bounds", index);

    const auto &col = mColumns[index];

    TL_ASSERT(col->type() == TypeTraits<T>::id_type, "Type mismatch");

    return std::span<const T>(static_cast<const T *>(col->data()), col->size());
}

inline auto DataFrame::cols() const noexcept -> size_t
{
    return mColumns.size();
}

inline auto DataFrame::size() const noexcept -> size_t
{
    return mColumns.empty() ? 0 : mColumns[0]->size();
}

inline auto DataFrame::columns() const noexcept -> const std::vector<std::string> &
{
    return mColNames;
}

inline auto DataFrame::row(size_t i) const -> RowView
{
    TL_ASSERT(i < size(), "DataFrame: Row index {} out of bounds", i);
    return RowView(*this, i);
}

inline auto DataFrame::rows() const -> RowRange
{
    return RowRange(*this);
}


template<typename T>
inline auto RowView::get(std::string_view col_name) const -> const T &
{
    auto span = mDf.column<T>(col_name);
    TL_ASSERT(mRowIndex < span.size(), "RowView: Index out of bounds");
    return span[mRowIndex];
}

template<typename T>
inline auto RowView::get(size_t col_index) const -> const T &
{
    auto span = mDf.column<T>(col_index);
    TL_ASSERT(mRowIndex < span.size(), "RowView: Index out of bounds");
    return span[mRowIndex];
}


RowRange::RowRange(const DataFrame &df) 
  : mDf(df)
{}

inline auto RowRange::begin() const -> RowIterator 
{ 
    return RowIterator(mDf, 0);
}

inline auto RowRange::end() const -> RowIterator
{ 
    return RowIterator(mDf, mDf.size());
}



//
//inline auto operator<<(std::ostream &os, const DataFrame &df) -> std::ostream &
//{
//    os << "DataFrame\n";
//    os << "Rows: " << df.size() << '\n';
//    os << "Columns: " << df.cols() << '\n';
//
//    os << "\nName\tType\tSize\n";
//
//    for (const auto &col : df.info()) {
//        os << col.name
//            << '\t'
//            << typeToString(col.type)
//            << '\t'
//            << col.size
//            << '\n';
//    }
//
//    return os;
//}


} // namespace tl
