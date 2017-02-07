#ifndef I3D_DATA_MODEL_H
#define I3D_DATA_MODEL_H

#include <list>
#include <memory>

#include "core/defs.h"

namespace I3D
{

class I3D_EXPORT TableRegister
{
public:
  TableRegister();
  ~TableRegister();

private:

};

/*!
 * \brief Campo de la tabla
 *
 * Clase virtual pura que se extendera para cada tipo de dato que se
 * necesite
 */
class I3D_EXPORT TableField
{

private:

  /*!
   * \brief Nombre del campo
   */
  std::string mName;

public:

  /*!
   * \brief Constructor
   * \param name Nombre del campo
   */
  TableField(const std::string name) : mName(mName) {}

  /*!
   * \brief Destructor
   */
  ~TableField();

};

/*!
 * \brief Campo de tabla de tipo entero
 */
class I3D_EXPORT TableFieldInt : public TableField
{
public:

  TableFieldInt(const std::string name) : TableField(name)
  {
  }

  ~TableFieldInt()
  {
  }

private:

};

/*!
 * \brief Campo de datos de tipo double
 */
class I3D_EXPORT TableFieldDouble : public TableField
{
public:
  TableFieldDouble(const std::string name) : TableField(name)
  {
  }

  ~TableFieldDouble()
  {
  }

private:

};

/*!
 * \brief Campo de datos de tipo float
 */
class I3D_EXPORT TableFieldFloat : public TableField
{
public:
  TableFieldFloat(const std::string name) : TableField(name)
  {
  }

  ~TableFieldFloat()
  {
  }

private:

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
  std::string mName;

  /*!
   * \brief Campos de la tabla
   */
  std::list<std::shared_ptr<TableField>> mTableFields;

  /*!
   * \brief Registros de la tabla
   */
  std::list<std::shared_ptr<TableRegister>> mRegister;
  
  //size_t index;

public:

  DataTable(const std::string tableName, const std::list<std::shared_ptr<TableField>> tableFields)
    : mName(tableName), mTableFields(tableFields) { }

  ~DataTable() {}

  /*!
   * \brief read
   */
  void read();

  /*!
   * \brief write
   */
  void write();


  /*!
   * \brief Iterador al primer punto
   */
  iterator begin();

  /*!
   * \brief Iterador al último punto
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
  std::string getName() const;

  /*!
   * \brief Devuelve el registro
   * \param[in] index Indice del registro
   * \return Registro
   */
  std::shared_ptr<TableRegister> getRegister(int index);

  /*!
   * \brief Establece el nombre de la tabla
   * \param[in] name Nombre de la tabla
   */
  void setName(const std::string &name);
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
  void createTable(std::string tableName, std::list<std::shared_ptr<TableField>> tableFields);

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
