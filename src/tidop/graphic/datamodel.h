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

#include <list>
#include <vector>
#include <memory>
#include <string>

#include "tidop/core/base/Defs.h"

namespace tl
{

/*! \addtogroup Graphic
 *  \{
 */

/*!
 * \class TableField
 * \brief Represents a field (column) definition in a table schema.
 *
 * A `TableField` stores metadata about a table column, including its name,
 * attributes type and maximum size. It defines the structure of a `TableRegister`.
 */
class TL_EXPORT TableField
{

public:

    /*!
     * \enum Type
     * \brief Supported field attributes types.
     */
    enum class Type
    {
        INT,       /*!< 32-bit integer */
        INT64,     /*!< 64-bit integer */
        DOUBLE,    /*!< Double-precision floating point */
        STRING     /*!< UTF-8 encoded string */
        //....
    };

private:

    std::string mName;
    Type mType;
    int mSize;

public:

    //TableField() = default;

    /*!
     * \brief Constructor
     * \param[in] name Field name
     * \param[in] type Field type
     * \param[in] size Maximum field size (in characters or bytes)
     */
    TableField(const std::string &name, Type type, int size);

    ~TableField();

    /*!
     * \brief Returns the field name
     */
    auto name() const->std::string;

    /*!
     * \brief Returns the field type
     */
    auto type() const->Type;

    /*!
     * \brief Returns the field size
     */
    auto size() const -> int;


};


/*!
 * \class RegisterValue
 * \brief Represents a single value in a table record.
 *
 * A `RegisterValue` holds a string-formatted value associated with a specific `TableField`.
 * The actual value may be stored in string form, regardless of its declared type.
 */
class TL_EXPORT RegisterValue
{

public:

    /*!
     * \brief Constructor with field reference.
     * \param[in] field Associated table field.
     */
    RegisterValue(const TableField &field);

    /*!
     * \brief Constructor with initial value.
     * \param[in] field Associated table field.
     * \param[in] value String-formatted value.
     */
    RegisterValue(const TableField &field,
                  std::string value);

    ~RegisterValue();

    /*!
     * \brief Returns the string value.
     */
    auto value() const -> std::string;

    /*!
     * \brief Sets the string value.
     * \param[in] value New value to assign.
     */
    void setValue(const std::string &value);

private:

    TableField mField;
    //Por ahora cargo una cadena con el valor
    std::string mValue;
};




/*!
 * \class TableRegister
 * \brief Represents a single record (row) in a attributes table.
 *
 * A `TableRegister` stores one value per field, forming a complete table row.
 * Values are internally managed as `RegisterValue` objects.
 */
class TL_EXPORT TableRegister
{

protected:

    std::vector<RegisterValue> mRegisterValues;

public:

    TableRegister() = default;

    /*!
     * \brief Constructs a new record with the given table schema.
     * \param[in] fields Table fields defining the register structure.
     */
    TableRegister(const std::vector<TableField> &fields);

    /*!
     * \brief Copy constructor
     * \param[in] _register Object to copy
     */
    TableRegister(const TableRegister &_register);

    //TableRegister(std::initializer_list<std::shared_ptr<TableRegisterField>> registerFields);

    ~TableRegister();

    /*!
     * \brief Returns the value at the given index.
     * \param[in] idx Field index.
     * \return Value as string.
     */
    auto value(size_t idx) const -> std::string;

    /*!
     * \brief Sets the value at the given index.
     * \param[in] idx Field index.
     * \param[in] field Value as string.
     */
    void setValue(size_t idx, const std::string &field);

    /*!
     * \brief Returns the number of fields in the register.
     */
    auto size() const -> size_t;

};


/*!
 * \class DataTable
 * \brief Represents a table with schema and attributes records.
 *
 * A `DataTable` contains a set of `TableField` definitions (columns)
 * and a list of `TableRegister` entries (rows). It supports iteration
 * over records and register creation based on the field definitions.
 */
class TL_EXPORT DataTable
{
public:

    typedef std::vector<TableRegister>::iterator iterator;

private:

    std::string mTableName;
    std::vector<TableField> mTableFields;
    std::vector<TableRegister> mRegister;

public:

    /*!
     * \brief Constructs a table with name and fields.
     * \param[in] tableName Table name.
     * \param[in] tableField Vector of field definitions.
     */
    DataTable(const std::string &tableName,
              const std::vector<TableField> &tableField);

    ~DataTable() = default;

    /*!
     * \brief Returns an iterator to the beginning of the table records.
     */
    auto begin() -> iterator;

    /*!
     * \brief Returns an iterator to the end of the table records.
     */
    auto end() -> iterator;

    /*!
     * \brief Returns the table name.
     */
    auto name() const -> std::string;

    /*!
     * \brief Creates a new empty register using the table schema.
     * \param[in] index Optional index (unused).
     * \return New `TableRegister` object.
     */
    auto createRegister(int index) const -> TableRegister;

    /*!
     * \brief Returns the list of fields that define the table schema.
     */
    auto fields() const -> std::vector<TableField>;

    /*!
     * \brief Sets the table name.
     * \param[in] name New table name.
     */
    void setName(const char *name);

    /*!
     * \brief Returns the number of records in the table.
     */
    auto size() const -> size_t;

};


/*!
 * \class DataModel
 * \brief Container and manager for multiple attributes tables.
 *
 * The `DataModel` manages a collection of named `DataTable` objects.
 * It allows for creation and registration of new tables in a structured
 * attributes model, often associated with geometries or layers.
 */
class TL_EXPORT DataModel
{

public:

    DataModel() = default;
    ~DataModel() = default;

    /*!
     * \brief Creates and registers a new table in the model.
     * \param[in] tableName Name of the table.
     * \param[in] fields Table schema definition.
     * \see TableField
     */
    void createTable(const std::string &tableName,
                     const std::vector<TableField> &fields);

    /*!
     * \brief Adds an existing table to the model.
     * \param[in] table Table to add.
     */
    void addTable(const DataTable& table);

private:

    std::vector<DataTable> mDataTables;
};

/*! \} */ // End Graphic

} // End namespace tl
