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

#include "tidop/core/defs.h"

namespace tl
{



/*!
 * \brief Campo de la tabla
 *
 */
class TL_EXPORT TableField
{
public:

    enum class Type
    {
        INT,
        INT64,
        DOUBLE,
        STRING
        //....
    };

private:

    std::string mName;
    Type mType;
    int mSize;

public:

    /*!
     * \brief Constructor
     * \param[in] name Field name
     * \param[in] type Type
     * \param[in] size Size
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



class TL_EXPORT RegisterValue
{

public:

    RegisterValue(const std::shared_ptr<TableField> &field);
    RegisterValue(const std::shared_ptr<TableField> &field,
                  std::string value);
    ~RegisterValue();

    auto value() const -> std::string;
    void setValue(const std::string &value);

private:

    std::shared_ptr<TableField> mField;
    //Por ahora cargo una cadena con el valor
    std::string mValue;
};




/*!
 * \brief Class representing a record of a table
 */
class TL_EXPORT TableRegister
{

protected:

    std::vector<RegisterValue> mRegisterValues;

public:

    TableRegister(const std::vector<std::shared_ptr<TableField>> &fields);

    /*!
     * \brief Copy constructor
     * \param[in] _register Object to copy
     */
    TableRegister(const TableRegister &_register);

    //TableRegister(std::initializer_list<std::shared_ptr<TableRegisterField>> registerFields);

    ~TableRegister();

    auto value(size_t idx) const -> std::string;
    void setValue(size_t idx, const std::string &field);

    auto size() const -> size_t;

};


/*!
 * \brief Tabla de datos
 */
class TL_EXPORT DataTable
{
public:

    typedef std::list<std::shared_ptr<TableRegister>>::iterator iterator;

private:

    std::string mTableName;
    std::vector<std::shared_ptr<TableField>> mTableFields;
    std::list<std::shared_ptr<TableRegister>> mRegister;

public:

    DataTable(const std::string &tableName,
              const std::vector<std::shared_ptr<TableField>> &tableField);
    ~DataTable() = default;

    auto begin() -> iterator;
    auto end() -> iterator;

    /*!
     * \brief Table name
     * \return Table name
     */
    auto name() const -> std::string;

    auto createRegister(int index) const -> std::shared_ptr<TableRegister>;

    auto fields() const -> std::vector<std::shared_ptr<TableField>>;

    /*!
     * \brief Sets the table name
     * \param[in] name Table name
     */
    void setName(const char *name);

    auto size() const -> size_t;

};


/*!
 * \brief The DataModel class
 */
class TL_EXPORT DataModel
{

public:

    DataModel();
    ~DataModel();
    /*!
     * \brief Creates a new table in the data model
     * \param[in] tableName Table name
     * \param[in] fields Table fields
     * \see TableField
     */
    void createTable(const std::string &tableName,
                     const std::vector<std::shared_ptr<TableField>> &fields);

    /*!
     * \brief Adds a table to the data model
     * \param[in] table Table to be added
     */
    void addTable(const std::shared_ptr<DataTable>& table);

private:

    std::list<std::shared_ptr<DataTable>> mDataTables;

};


} // End namespace tl
