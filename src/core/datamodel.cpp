#include "core/datamodel.h"

namespace I3D
{

//TODO: falta todo por hacer...

/* ---------------------------------------------------------------------------------- */

TableRegister::TableRegister()
{
}

TableRegister::~TableRegister()
{
}


/* ---------------------------------------------------------------------------------- */


TableField::~TableField()
{
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

std::string DataTable::getName() const
{
  return mName;
}

std::shared_ptr<TableRegister> DataTable::getRegister(int index)
{
  iterator it = mRegister.begin();
  std::advance(it, index);
  return *it;
}

void DataTable::setName(const std::string &name)
{
  mName = name;
}

/* ---------------------------------------------------------------------------------- */

DataModel::DataModel()
{

}

DataModel::~DataModel()
{
}

void DataModel::createTable(std::string tableName, std::list<std::shared_ptr<TableField>> tableFields) 
{
  mDataTables.push_back(std::make_shared<DataTable>(tableName, tableFields));
}

void DataModel::addTable(std::shared_ptr<DataTable> table)
{
  mDataTables.push_back(table);
}

/* ---------------------------------------------------------------------------------- */


} // End namespace I3D
