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

#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <fstream>
#include <sstream>
#include <charconv>

#include "tidop/core/base/Path.h"
#include "tidop/core/base/Exception.h"

namespace tl
{

namespace detail
{

// Split a line by delimiter, preserving empty fields.
inline std::vector<std::string> split_line(const std::string &line, char delimiter)
{
    std::vector<std::string> fields;
    std::string token;
    std::istringstream ss(line);
    while (std::getline(ss, token, delimiter)) {
        fields.push_back(token);
    }
    // Handle trailing delimiter (empty field at end)
    if (!line.empty() && line.back() == delimiter) {
        fields.emplace_back();
    }
    return fields;
}

// Convert a string token to a value of type T.

/// TODO: Reemplazar por convertStringTo
template <typename T>
T convert_token(const std::string &token)
{
    if constexpr (std::is_integral_v<T>) {
        T value{};
        auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), value);
        TL_ASSERT(ec == std::errc(), "Failed to convert token to integral: {}", token);
        return value;
    } else if constexpr (std::is_floating_point_v<T>) {
        double tmp{};
        auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), tmp);
        TL_ASSERT(ec == std::errc(), "Failed to convert token to floating point: {}", token);
        return static_cast<T>(tmp);
    } else {
        return token; // Assume std::string or compatible type.
    }
}

// Load CSV/TSV file into column vectors.
// Columns types are given by template parameter pack Columns.
// Returns a tuple of vectors in the same order as Columns.
template <typename... Columns>
std::tuple<std::vector<Columns>...> load_file(const std::string &path,
    char delimiter,
    bool has_header,
    const std::vector<std::string> &user_col_names)
{
    std::ifstream infile(path);
    TL_ASSERT(infile.is_open(), "Cannot open file: {}", path);

    std::vector<std::string> col_names = user_col_names;
    std::string line;
    if (has_header) {
        TL_ASSERT(std::getline(infile, line), "File is empty, cannot read header.");
        col_names = detail::split_line(line, delimiter);
    }
    // Prepare containers for each column.
    std::tuple<std::vector<Columns>...> columns_data;
    size_t expected_fields = sizeof...(Columns);
    TL_ASSERT(!col_names.empty() && col_names.size() == expected_fields, "Header column count does not match template columns.");

    size_t row_index = 0;
    while (std::getline(infile, line)) {
        auto fields = detail::split_line(line, delimiter);
        TL_ASSERT(fields.size() == expected_fields, "Row {} has {} fields; expected {}", std::to_string(row_index), std::to_string(fields.size()), std::to_string(expected_fields));
        // Fill each column.
        std::size_t idx = 0;
        (void)std::initializer_list<int>{
            ((std::get<std::vector<Columns>>(columns_data).push_back(detail::convert_token<Columns>(fields[idx++])), 0))...
        };
        ++row_index;
    }
    return columns_data;
}

}

/*! \addtogroup MathBase
 *  \{
 */

/*!
 * \class DataFrame
 * \brief Estructura de datos tabular orientada a columnas, similar a Pandas.
 *
 * Utiliza características de C++17 y C++20 para una implementación eficiente
 * y segura de tipos mediante variadic templates y fold expressions.
 *
 * \tparam Columns Tipos de datos de cada columna.
 */
template <typename... Columns>
class DataFrame
{
public:
    /*!
     * \brief DataFrame a partir de filas de datos.
     * \param[in] columns Nombres de las columnas
     * \param[in] data Datos estructurados por filas (tuplas)
     *
     * <h4>Ejemplo</h4>
     * \code
     *   DataFrame<int, double, std::string> data_frame({"col1", "col2", "col3"},
     *                                                  { {1, 1.1, "text1"},
     *                                                    {2, 2.2, "text2"},
     *                                                    {3, 3.3, "text3"} });
     *   std::cout << data_frame << std::endl;
     * \endcode
     */
    DataFrame(const std::vector<std::string> &columns,
              std::initializer_list<std::tuple<Columns...>> data)
        : mCols(columns)
    {
        for (const auto& row : data) {
            append_row(row, std::index_sequence_for<Columns...>{});
        }
    }

    /*!
     * \brief DataFrame a partir de vectores (columnas).
     * \param[in] columns Nombres de las columnas
     * \param[in] data Resto de argumentos: vectores que representan las columnas.
     *
     * <h4>Ejemplo</h4>
     * \code
     *   std::vector<std::string> columns{"col1", "col2", "col3"};
     *   std::vector<int> v1{1, 2, 3};
     *   std::vector<double> v2{ 1.1, 2.2, 3.3 };
     *   std::vector<std::string> v3{"text1", "text2", "text3"};
     *   DataFrame<int, double, std::string> data_frame(columns, v1, v2, v3);
     *   std::cout << data_frame << std::endl;
     * \endcode
     */
    DataFrame(const std::vector<std::string> &columns,
              const std::vector<Columns>&... data)
        : mCols(columns), mData(data...)
    {
        check_sizes();
    }

    /*!
     * \brief DataFrame a partir de initializer_list de columnas.
     * \param[in] columns Nombres de las columnas
     * \param[in] data Listas de inicialización para cada columna.
     *
     * <h4>Ejemplo</h4>
     * \code
     *   DataFrame<int, double, std::string> data_frame({ "col1", "col2", "col3" },
     *                                                  { 1, 2, 3 },
     *                                                  { 1.1, 2.2, 3.3 },
     *                                                  { "text1", "text2", "text3" });
     *   std::cout << data_frame << std::endl;
     * \endcode
     */
    DataFrame(const std::vector<std::string> &columns,
              std::initializer_list<Columns>... data)
        : mCols(columns), mData(data...)
    {
        check_sizes();
    }

    /*!
     * \brief DataFrame a partir de pares {Nombre, initializer_list}.
     * \param[in] data Pares que contienen el nombre de la columna y sus datos.
     *
     * <h4>Ejemplo</h4>
     * \code
     *   DataFrame<int, double, std::string> data_frame({"col1", {1, 2, 3}},
     *                                                  {"col2", {1.1, 2.2, 3.3}},
     *                                                  {"col3", {"text1", "text2", "text3"}});
     *   std::cout << data_frame << std::endl;
     * \endcode
     */
    DataFrame(std::pair<std::string, std::initializer_list<Columns>>... data)
        : mCols{data.first...}, mData(data.second...)
    {
        check_sizes();
    }

    ~DataFrame() = default;
    
    // -----------------------------------------------------------------
    // Static factories for loading from delimited files
    // -----------------------------------------------------------------
    /** 
     * @brief Load a DataFrame from a delimited text file (CSV/TSV).
     * @param path Path to the file.
     * @param delimiter Field delimiter (default ',').
     * @param has_header True if first line contains column names.
     * @param col_names Column names to use when has_header is false.
     * @throws DataFrameParseError on I/O or format errors.
     */
    static DataFrame fromFile(const Path &path,
                               char delimiter = ',',
                               bool has_header = true,
                               const std::vector<std::string>& col_names = {}) {
        std::ifstream file(path.toString());
        TL_ASSERT(file.is_open(), "Cannot open file: {}", path.toString());
        
        std::vector<std::string> column_names = col_names;
        std::string line;
        if (has_header) {
            TL_ASSERT(std::getline(file, line), "Empty file, cannot read header.");
            column_names = detail::split_line(line, delimiter);
        }
        // Load column data (without rereading header)
        auto column_vectors = detail::load_file<Columns...>(path.toString(),
                                                       delimiter,
                                                       has_header,
                                                       column_names);
        return DataFrame(column_names,
                         std::get<std::vector<Columns>>(column_vectors)...);
    }

    /** 
     * @brief Placeholder for type‑inferred loading (not implemented).
     */
    static DataFrame from_file_infer(const std::string& /*path*/,
                                    char /*delimiter*/ = ',',
                                    bool /*has_header*/ = true) {
        TL_THROW_EXCEPTION("from_file_infer not implemented – explicit types required.");
    }

    /*!
     * \brief Número de columnas
     * \return El número total de columnas.
     */
    size_t cols() const 
    { 
        return sizeof...(Columns); 
    }

    /*!
     * \brief Número de filas
     * \return El número total de filas.
     */
    size_t rows() const 
    {
        if constexpr (sizeof...(Columns) > 0) {
            return std::get<0>(mData).size();
        }
        return 0;
    }

    /*!
     * \brief Acceso a los nombres de columna
     * \param[in] columnId Indice de columna
     * \return Nombre de la columna
     */
    std::string &column(size_t columnId) 
    { 
        return mCols.at(columnId); 
    }
    
    const std::string &column(size_t columnId) const 
    { 
        return mCols.at(columnId); 
    }

    /*!
     * \brief Listado con los nombres de las columnas
     * \return Un vector con los nombres.
     */
    std::vector<std::string> columns() const 
    { 
        return mCols; 
    }

    /*!
     * \brief Obtener una fila (registro) en un índice específico
     * \param idx Índice de la fila.
     * \return Una tupla con los valores de la fila en ese índice.
     */
    std::tuple<Columns...> reg(size_t idx) const 
    {
        if (idx >= rows()) {
            throw std::out_of_range("Index out of bounds for DataFrame row");
        }
        return get_row_impl(idx, std::index_sequence_for<Columns...>{});
    }

    /*!
     * \brief Acceso directo al vector de una columna por su índice en la plantilla.
     */
    template <size_t I>
    std::vector<std::tuple_element_t<I, std::tuple<Columns...>>>& getColumnData() 
    { 
        return std::get<I>(mData); 
    }

    template <size_t I>
    const std::vector<std::tuple_element_t<I, std::tuple<Columns...>>>& getColumnData() const 
    { 
        return std::get<I>(mData); 
    }

    /*!
     * \brief Imprime el DataFrame por salida estándar.
     */
    friend std::ostream &operator<<(std::ostream &os, const DataFrame &dataframe)
    {
        os << "   ";
        for (size_t c = 0; c < dataframe.cols(); c++) {
            os << std::left << std::setw(10) << dataframe.column(c);
        }
        os << "\n";

        for (size_t r = 0; r < dataframe.rows(); r++) {
            os << std::left << std::setw(3) << r + 1;
            auto reg = dataframe.reg(r);
            std::apply([&os](const auto&... args) {
                ((os << std::left << std::setw(10) << args), ...);
            }, reg);
            os << "\n";
        }

        os << std::flush;
        return os;
    }

private:
    /*!
     * \brief Verifica que todas las columnas tengan el mismo tamaño (número de filas).
     */
    void check_sizes() const 
    {
        if constexpr (sizeof...(Columns) > 0) {
            size_t expected_size = std::get<0>(mData).size();
            auto check = [&](const auto& vec) {
                if (vec.size() != expected_size) {
                    throw std::invalid_argument("DataFrame: All columns must have the same size.");
                }
            };
            std::apply([&](const auto&... args) { (check(args), ...); }, mData);
        }
    }

    template<std::size_t... Is>
    std::tuple<Columns...> get_row_impl(size_t idx, std::index_sequence<Is...>) const 
    {
        return std::make_tuple(std::get<Is>(mData)[idx]...);
    }

    template<std::size_t... Is>
    void append_row(const std::tuple<Columns...>& row, std::index_sequence<Is...>) 
    {
        (std::get<Is>(mData).push_back(std::get<Is>(row)), ...);
    }

private:
    std::vector<std::string> mCols;
    std::tuple<std::vector<Columns>...> mData;
};




// -----------------------------------------------------------------
// Dynamic schema support (when a single Schema type with static member `Columns` is provided)
// -----------------------------------------------------------------


//struct DynamicSchema
//{
//    static constexpr std::size_t Id = 1;                 // ejemplo: id del esquema
//    static constexpr std::size_t Columns = 0;            // 0 → “desconocido” (lo determinará el CSV)
//};

//
//// Helper to detect a schema type that defines `static constexpr size_t Columns`.
//namespace tl {
//    template <class, class = void>
//    struct has_columns : std::false_type {};
//    template <class T>
//    struct has_columns<T, std::void_t<decltype(T::Columns)>> : std::true_type {};
//}
//
//// Partial specialization for a single template argument that satisfies `has_columns`.
//// This specialization treats the DataFrame as a dynamic, column‑oriented container of strings.
//// It is selected only when the provided type has a static member `Columns`.
//
//template <class Schema,
//          std::enable_if_t<tl::has_columns<Schema>::value, int> = 0>
//class DataFrame<Schema>
//{
//public:
//    // Row stores each cell as a string (raw text from CSV).
//    using Row = std::vector<std::string>;
//
//    DataFrame() = default;
//    explicit DataFrame(const std::string &csv_path) { load(csv_path); }
//
//    // -----------------------------------------------------------------
//    // API pública (compatible con la versión estática cuando sea posible)
//    // -----------------------------------------------------------------
//    const std::vector<Row> &rows() const noexcept { return data_; }
//    std::size_t column_count() const noexcept { return ncols_; }
//
//    // -----------------------------------------------------------------
//    // Carga de CSV (delimitador ',' por defecto)
//    // -----------------------------------------------------------------
//    void load(const std::string &csv_path)
//    {
//        std::ifstream file(csv_path);
//        if (!file) {
//            throw std::runtime_error("Cannot open file: " + csv_path);
//        }
//
//        std::string line;
//        bool first = true;
//        while (std::getline(file, line)) {
//            std::istringstream ss(line);
//            std::string cell;
//            Row row;
//            while (std::getline(ss, cell, ',')) {
//                row.emplace_back(std::move(cell));
//            }
//            // Si la línea termina en delimitador, añadimos celda vacía
//            if (!line.empty() && line.back() == ',') {
//                row.emplace_back();
//            }
//
//            if (first) {
//                first = false;
//                if constexpr (Schema::Columns == 0) {
//                    ncols_ = row.size();
//                } else {
//                    ncols_ = Schema::Columns;
//                }
//            }
//            data_.emplace_back(std::move(row));
//        }
//    }
//
//    // -----------------------------------------------------------------
//    // Acceso a una celda (devuelve std::string_view para evitar copias)
//    // -----------------------------------------------------------------
//    std::string_view get(std::size_t row, std::size_t col) const {
//        if (row >= data_.size() || col >= ncols_) {
//            throw std::out_of_range("Index out of range");
//        }
//        return data_[row][col];
//    }
//
//private:
//    std::vector<Row> data_;      // filas leídas del CSV
//    std::size_t       ncols_{0}; // número de columnas determinado en tiempo de carga
//};

// End of dynamic‑schema specialization

/*! \} */

} // namespace tl
