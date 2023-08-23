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

TableField::~TableField()
{
}

std::string TableField::name() const
{
    return mName;
}

TableField::Type TableField::type() const
{
    return mType;
}

int TableField::size() const
{
    return mSize;
}









RegisterValue::RegisterValue(const std::shared_ptr<TableField> &field)
  : mField(field),
    mValue("")
{
}

RegisterValue::RegisterValue(const std::shared_ptr<TableField> &field,
                             const std::string &value)
  : mField(field),
    mValue(value)
{
}

RegisterValue::~RegisterValue()
{
}

std::string  RegisterValue::value() const
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

TableRegister::~TableRegister()
{
}

std::string TableRegister::value(size_t idx) const
{
    if (idx < mRegisterValues.size())
        return mRegisterValues[idx].value();
    else return std::string();
}

void TableRegister::setValue(size_t idx, const std::string &field)
{
    if (idx < mRegisterValues.size())
        mRegisterValues[idx].setValue(field);
}

size_t TableRegister::size() const
{
    return mRegisterValues.size();
}






DataTable::DataTable(const std::string &tableName,
                     const std::vector<std::shared_ptr<TableField>> &TableField)
  : mTableName(tableName),
    mTableFields(TableField)
{
}

DataTable::iterator DataTable::begin()
{
    return mRegister.begin();
}

DataTable::iterator DataTable::end()
{
    return mRegister.end();
}

//void DataTable::addRegister(std::shared_ptr<TableRegister> _register)
//{
//  mRegister.push_back(_register);
//}
//
//void DataTable::deleteRegister(int index)
//{
//  iterator it = mRegister.begin();
//  std::advance(it, index);
//  mRegister.erase(it);
//}

std::string DataTable::name() const
{
    return mTableName;
}

//std::shared_ptr<TableRegister> DataTable::getRegister(int index)
//{
//  iterator it = mRegister.begin();
//  std::advance(it, index);
//  return *it;
//}

TL_TODO("¿Mejor como estatica?")
std::shared_ptr<TableRegister> DataTable::createRegister(int index)
{
    ///TODO: Completar
    unusedParameter(index);
    return std::shared_ptr<TableRegister>(new TableRegister(mTableFields));
}

std::vector<std::shared_ptr<TableField>> DataTable::fields() const
{
    return mTableFields;
}
//
//size_t DataTable::getFieldCount() const
//{
//  return mTableHeader->getFieldCount();
//}

void DataTable::setName(const char *name)
{
    mTableName = name;
}

//void DataTable::setTableHeader(std::shared_ptr<TableHeader> tableHeader)
//{
//  mTableHeader = tableHeader;
//}

size_t DataTable::size()
{
    return mRegister.size();
}






DataModel::DataModel()
{

}

DataModel::~DataModel()
{
}

void DataModel::createTable(const std::string &tableName,
                            const std::vector<std::shared_ptr<TableField>> &fields)
{
    mDataTables.push_back(std::make_shared<DataTable>(tableName, fields));
}

void DataModel::addTable(std::shared_ptr<DataTable> table)
{
    mDataTables.push_back(table);
}



} // End namespace tl
