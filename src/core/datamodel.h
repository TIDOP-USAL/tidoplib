#ifndef I3D_DATA_MODEL_H
#define I3D_DATA_MODEL_H

#include <list>
#include <vector>
#include <memory>

#include "core/defs.h"

namespace I3D
{


/*!
 * \brief Campo de la tabla
 *
 */
class I3D_EXPORT TableHeaderField
{
public:

  enum class Type
  {
    INT,
    DOUBLE,
    FLOAT,
    STRING
  };

private:

  /*!
   * \brief Nombre del campo
   */
  std::string mName;

  /*!
   * \brief Tipo del campo
   */
  Type mType;

public:

  /*!
   * \brief Constructor
   * \param name Nombre del campo
   */
  TableHeaderField(const std::string &name, Type type);

  /*!
   * \brief Destructor
   */
  ~TableHeaderField();

  /*!
   * \brief Devuelve el nombre del campo
   */
  const char *getName();

  /*!
   * \brief Devuelve el tipo del campo
   */
  Type getType();
};


/*!
 * \brief Cabecera de la tabla
 */
class I3D_EXPORT TableHeader
{
private:

  /*!
   * \brief Campos de la tabla
   */
  std::vector<std::shared_ptr<TableHeaderField>> mTableFields;

public:

  /*!
   * \brief Constructora por defecto
   */
  TableHeader();

  /*!
   * \brief Constructor de copia
   * \param[in] tableHeader
   */
  TableHeader(const TableHeader &tableHeader);

  /*!
   * \brief Constructor de lista
   * \param[in] tableHeader
   */
  TableHeader(std::initializer_list<std::shared_ptr<TableHeaderField>> tableHeader);

  /*!
   * \brief Destructora
   */
  ~TableHeader();

  void addField(std::shared_ptr<TableHeaderField> field);

  /*!
   * \brief Limpia la lista de procesos
   */
  void clear();

  size_t getFieldCount();

  TableHeaderField *getTableFieldName(int idx);

private:

};


class TableRegisterField
{
public:
  TableRegisterField();
  ~TableRegisterField();

private:

};




/*!
 * \brief Clase que representa un registro de una tabla
 */
class I3D_EXPORT TableRegister
{

protected:
  
  /*!
   * \brief Campos de la tabla
   */
  std::vector<std::shared_ptr<TableRegisterField>> mRegisterValues;

public:

  /*!
   * \brief Constructora por defecto
   */
  TableRegister();

  /*!
   * \brief Constructor de copia
   * \param[in] tableRegister
   */
  TableRegister(const TableRegister &_register);
  
  /*!
   * \brief Constructor de lista
   * \param[in] registerFields
   */
  TableRegister(std::initializer_list<std::shared_ptr<TableRegisterField>> registerFields);

  /*!
   * \brief Destructora
   */
  ~TableRegister();

  const char *getValue(int idx);
};


/*!
 * \brief Tabla de datos
 */
class I3D_EXPORT DataTable /*: public std::iterator<std::bidirectional_iterator_tag, int>*/
{
public:

  /*!
   * \brief Iterador sobre los registros de la tabla
   */
  typedef std::list<std::shared_ptr<TableRegister>>::iterator iterator;

private:

  /*!
   * \brief Nombre de la tabla
   */
  std::string mTableName;

  /*!
   * \brief Campos de la tabla
   */
  std::shared_ptr<TableHeader> mTableHeader;

  /*!
   * \brief Registros de la tabla
   */
  std::list<std::shared_ptr<TableRegister>> mRegister;
  
  //size_t index;

public:

  DataTable() 
    : mTableName(""), mTableHeader(0) { }

  DataTable(const DataTable &dataTable) 
    : mTableName(dataTable.mTableName), mTableHeader(dataTable.mTableHeader) {}
  DataTable(const std::string &tableName, std::shared_ptr<TableHeader> tableHeader)
    : mTableName(tableName), mTableHeader(tableHeader) { }

  ~DataTable() {}

  ///*!
  // * \brief read
  // */
  //void read();

  ///*!
  // * \brief write
  // */
  //void write();


  /*!
   * \brief Iterador al primer registro
   */
  iterator begin();

  /*!
   * \brief Iterador al último registro
   */
  iterator end();

  /*!
   * \brief Añade un registro al final
   * \param[in] _register Registro que se añade
   */
  void addRegister(std::shared_ptr<TableRegister> _register);

  /*!
   * \brief Borra un registro
   * \param[in] index Indice del registro que se borra
   */
  void deleteRegister(int index);

  /*!
   * \brief Nombre de la tabla
   * \return Nombre de la tabla
   */
  const char *getName() const;

  /*!
   * \brief Devuelve el registro
   * \param[in] index Indice del registro
   * \return Registro
   */
  std::shared_ptr<TableRegister> getRegister(int index);

  TableHeader *getTableHeader();

  /*!
   * \brief Establece el nombre de la tabla
   * \param[in] name Nombre de la tabla
   */
  void setName(const char *name);

  /*!
   * \brief Establece la cabecera de la tabla
   * \param[in] tableHeader Cabecera de la tabla
   */
  void setTableHeader(std::shared_ptr<TableHeader> tableHeader);
};


/*!
 * \brief The DataModel class
 */
class I3D_EXPORT DataModel
{

private:

  std::list<std::shared_ptr<DataTable>> mDataTables;

public:

  /*!
   * \brief Constructor por defecto
   */
  DataModel();

  /*!
   * \brief Destructor
   */
  ~DataModel();

  /*!
   * \brief create
   */
  void create();

  /*!
   * \brief Crea una tabla nueva en el modelo de datos
   * \param[in] tableName Nombre de la tabla
   * \param[in] tableFields Campos de la tabla
   * \see TableField
   */
  void createTable(const std::string &tableName, std::shared_ptr<TableHeader> tableHeader);

  /*!
   * \brief Añade una tabla al modelo de datos
   * \param[in] table Tabla que se añade
   */
  void addTable(std::shared_ptr<DataTable> table);

  /*!
   * \brief drop
   */
  void drop();

  /*!
   * \brief open
   */
  void open(std::string file);

  /*!
   * \brief read
   */
  void read();

  /*!
   * \brief write
   */
  void write();

  /*!
   * \brief save
   */
  void save();
};




} // End namespace I3D

#endif // I3D_CONSOLE_H
