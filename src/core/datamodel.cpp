#include "core/datamodel.h"

namespace I3D
{

//TODO: falta todo por hacer...

/* ---------------------------------------------------------------------------------- */

TableHeaderField::TableHeaderField(const std::string &name, Type type) 
  : mName(name),
    mType(type)
{
}

TableHeaderField::~TableHeaderField()
{
}

const char *TableHeaderField::getName()
{
  return mName.c_str();
}

TableHeaderField::Type TableHeaderField::getType()
{
  return mType;
}

/* ---------------------------------------------------------------------------------- */

TableHeader::TableHeader()
  : mTableFields(0)
{
}

TableHeader::TableHeader(const TableHeader &tableHeader) 
  : mTableFields(tableHeader.mTableFields) 
{
}

TableHeader::TableHeader(std::initializer_list<std::shared_ptr<TableHeaderField>> tableHeader) 
  : mTableFields(tableHeader) 
{
}

TableHeader::~TableHeader()
{
}

void TableHeader::addField(std::shared_ptr<TableHeaderField> field)
{
  mTableFields.push_back(field);
}

void TableHeader::clear() 
{ 
  mTableFields.clear(); 
}

size_t TableHeader::getFieldCount()
{
  return mTableFields.size();
}

TableHeaderField *TableHeader::getTableFieldName(int idx)
{
  return mTableFields[idx].get();
}

/* ---------------------------------------------------------------------------------- */

TableRegisterField::TableRegisterField()
{}

TableRegisterField::~TableRegisterField()
{}

/* ---------------------------------------------------------------------------------- */


TableRegister::TableRegister()
  : mRegisterValues(0)
{
}

TableRegister::TableRegister(const TableRegister &_register)
  : mRegisterValues(_register.mRegisterValues)
{
}

TableRegister::TableRegister(std::initializer_list<std::shared_ptr<TableRegisterField>> registerFields)
  : mRegisterValues(registerFields)
{

}

TableRegister::~TableRegister()
{
}

const char *TableRegister::getValue(int idx)
{
  //TODO: Aqui hay que ver como devolver el tipo adecuado
  return NULL;
}

/* ---------------------------------------------------------------------------------- */


DataTable::iterator DataTable::begin()
{
  return mRegister.begin();
}

DataTable::iterator DataTable::end() 
{
  return mRegister.end();
}

void DataTable::addRegister(std::shared_ptr<TableRegister> _register)
{
  mRegister.push_back(_register);
}

void DataTable::deleteRegister(int index)
{
  iterator it = mRegister.begin();
  std::advance(it, index);
  mRegister.erase(it);
}

const char *DataTable::getName() const
{
  return mTableName.c_str();
}

std::shared_ptr<TableRegister> DataTable::getRegister(int index)
{
  iterator it = mRegister.begin();
  std::advance(it, index);
  return *it;
}

TableHeader *DataTable::getTableHeader()
{
  return mTableHeader.get();
}

void DataTable::setName(const char *name)
{
  mTableName = name;
}

void DataTable::setTableHeader(std::shared_ptr<TableHeader> tableHeader)
{
  mTableHeader = tableHeader;
}


/* ---------------------------------------------------------------------------------- */

DataModel::DataModel()
{

}

DataModel::~DataModel()
{
}

void DataModel::createTable(const std::string &tableName, std::shared_ptr<TableHeader> tableHeader) 
{
  mDataTables.push_back(std::make_shared<DataTable>(tableName, tableHeader));
}

void DataModel::addTable(std::shared_ptr<DataTable> table)
{
  mDataTables.push_back(table);
}

/* ---------------------------------------------------------------------------------- */


} // End namespace I3D
