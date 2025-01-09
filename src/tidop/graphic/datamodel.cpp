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

#include "tidop/graphic/datamodel.h"
#include "tidop/core/base/common.h"

namespace tl
{



TableField::TableField(const std::string &name,
                       Type type,
                       int size)
  : mName(name),
    mType(type),
    mSize(size)
{
}

TableField::~TableField() = default;

auto TableField::name() const -> std::string
{
    return mName;
}

auto TableField::type() const -> TableField::Type
{
    return mType;
}

auto TableField::size() const -> int
{
    return mSize;
}









RegisterValue::RegisterValue(const std::shared_ptr<TableField> &field)
  : mField(field)
{
}

RegisterValue::RegisterValue(const std::shared_ptr<TableField> &field,
                             std::string value)
  : mField(field),
    mValue(std::move(value))
{
}

RegisterValue::~RegisterValue() = default;

auto RegisterValue::value() const -> std::string
{
    return mValue;
}

void RegisterValue::setValue(const std::string &value)
{
    TL_TODO("Usar TableField para comprobar que el valor introducido es correcto")
        mValue = value;
}






TableRegister::TableRegister(const std::vector<std::shared_ptr<TableField>> &fields)
{
    size_t size = fields.size();

    for (size_t i = 0; i < size; i++) {
        mRegisterValues.push_back(RegisterValue(fields[i]));
    }
}

TableRegister::TableRegister(const TableRegister &_register)
  : mRegisterValues(_register.mRegisterValues)
{
}

TableRegister::~TableRegister() = default;

auto TableRegister::value(size_t idx) const -> std::string
{
    if (idx < mRegisterValues.size())
        return mRegisterValues[idx].value();

    return std::string{};
}

void TableRegister::setValue(size_t idx, const std::string &field)
{
    if (idx < mRegisterValues.size())
        mRegisterValues[idx].setValue(field);
}

auto TableRegister::size() const -> size_t
{
    return mRegisterValues.size();
}






DataTable::DataTable(const std::string &tableName,
                     const std::vector<std::shared_ptr<TableField>> &tableField)
  : mTableName(tableName),
    mTableFields(tableField)
{
}

auto DataTable::begin() -> iterator
{
    return mRegister.begin();
}

auto DataTable::end() -> iterator
{
    return mRegister.end();
}

auto DataTable::name() const -> std::string
{
    return mTableName;
}

auto DataTable::createRegister(int index) const -> std::shared_ptr<TableRegister>
{
    ///TODO: Completar
    unusedParameter(index);
    return std::make_shared<TableRegister>(mTableFields);
}

auto DataTable::fields() const -> std::vector<std::shared_ptr<TableField>>
{
    return mTableFields;
}


void DataTable::setName(const char *name)
{
    mTableName = name;
}

auto DataTable::size() const -> size_t
{
    return mRegister.size();
}






DataModel::DataModel()
{
}

DataModel::~DataModel() = default;

void DataModel::createTable(const std::string &tableName,
                            const std::vector<std::shared_ptr<TableField>> &fields)
{
    mDataTables.push_back(std::make_shared<DataTable>(tableName, fields));
}

void DataModel::addTable(const std::shared_ptr<DataTable> &table)
{
    mDataTables.push_back(table);
}



} // End namespace tl
