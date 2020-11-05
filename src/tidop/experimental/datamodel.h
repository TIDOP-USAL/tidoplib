#ifndef TL_DATA_MODEL_H
#define TL_DATA_MODEL_H

#include "config_tl.h"

#include <list>
#include <vector>
#include <memory>

#include "tidop/core/defs.h"

namespace tl
{


/*!
 * \brief Campo de la tabla
 *
 */
class TL_EXPORT TableHeaderField
{
public:

  enum class Type
  {
    INT,
    DOUBLE,
    FLOAT,
    STRING
    //....
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

  //TODO: Añadir tamaño??
public:

  /*!
   * \brief Constructor
   * \param[in] name Nombre del campo
   * \param[in] type Tipo
   */
  TableHeaderField(const std::string &name, Type type);

  /*!
   * \brief Destructor
   */
  ~TableHeaderField();

  /*!
   * \brief Devuelve el nombre del campo
   */
  const char *getName() const;

  /*!
   * \brief Devuelve el tipo del campo
   */
  Type getType() const;
};


/*!
 * \brief Cabecera de la tabla
 */
class TL_EXPORT TableHeader
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

  size_t getFieldCount() const;

  const TableHeaderField *getTableHeaderField(int idx) const;

private:

};


//class TL_EXPORT TableRegisterField
//{
//private:
//
//  //Por ahora cargo una cadena con el valor
//  std::string mValue;
//
//public:
//  TableRegisterField(const std::string &value);
//  ~TableRegisterField();
//
//  std::string  getValue() const;
//  void setValue(const std::string &value);
//private:
//
//};




/*!
 * \brief Clase que representa un registro de una tabla
 */
class TL_EXPORT TableRegister
{

protected:
  
  /*!
   * \brief Campos de la tabla
   */
  //std::vector<std::shared_ptr<TableRegisterField>> mRegisterValues;
  std::vector<std::string> mRegisterValues;

public:

  /*!
   * \brief Constructora por defecto
   */
  TableRegister();
  TableRegister(int size);

  /*!
   * \brief Constructor de copia
   * \param[in] _register Objeto que se copia
   */
  TableRegister(const TableRegister &_register);
  
  //TableRegister(std::initializer_list<std::shared_ptr<TableRegisterField>> registerFields);

  /*!
   * \brief Destructora
   */
  ~TableRegister();

  const char *getValue(int idx);

  void setField(int idx, const std::string &field);

  void setSize(int size);

};


/*!
 * \brief Tabla de datos
 */
class TL_EXPORT DataTable /*: public std::iterator<std::bidirectional_iterator_tag, int>*/
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

  const TableHeader *getTableHeader() const;
  
  size_t getFieldCount() const;
  
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

  size_t size();
};


/*!
 * \brief The DataModel class
 */
class TL_EXPORT DataModel
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
   * \param[in] tableHeader Cabecera de la tabla
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




} // End namespace TL

#endif // TL_CONSOLE_H
