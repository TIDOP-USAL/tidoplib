#ifndef TL_VECTOR_WRITER_H
#define TL_VECTOR_WRITER_H

#include <memory>
#include <list>
#include <string>

#include "config_tl.h"

#include "tidop/core/defs.h"
//#include "tidop/geospatial/crs.h"

namespace tl
{

namespace graph
{
class GLayer;
}

class TL_EXPORT VectorWriter
{

public:

  VectorWriter(const std::string &fileName);
  virtual ~VectorWriter() = default;

  /*!
   * \brief Abre el fichero
   */
  virtual void open() = 0;

  /*!
   * \brief Comprueba si el fichero se ha cargado correctamente
   */
  virtual bool isOpen() = 0;

  /*!
   * \brief Cierra el fichero
   */
  virtual void close() = 0;

  virtual void create() = 0;

  virtual void write(const graph::GLayer &layer) = 0;

  //virtual void setCRS(const geospatial::Crs &crs) = 0;
  virtual void setCRS(const std::string &epsgCode) = 0;

protected:

  std::string mFileName;

};


/*!
 * \brief Factoria de clases para la escritura de diferentes formatos vectoriales
 */
class TL_EXPORT VectorWriterFactory
{
public:

private:

  VectorWriterFactory() {}

public:

  static std::unique_ptr<VectorWriter> createWriter(const std::string &fileName);
};


} // End namespace tl


#endif // TL_VECTOR_WRITER_H
