#ifndef TL_VECTOR_READER_H
#define TL_VECTOR_READER_H

#include <memory>
#include <list>
#include <string>

#include "config_tl.h"

#include "tidop/core/defs.h"

namespace tl
{

namespace graph
{
class GLayer;
}


class TL_EXPORT VectorReader
{

public:

	VectorReader(const std::string &fileName);
	virtual ~VectorReader() = default;

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

  virtual int layersCount() const = 0;
  virtual std::shared_ptr<graph::GLayer> read(int layerId) = 0;
  virtual std::shared_ptr<graph::GLayer> read(const std::string &layerName) = 0;

protected:

  std::string mFileName;

};


/*!
 * \brief Factoría de clases para la lectura de formatos vectoriales
 */
class TL_EXPORT VectorReaderFactory
{

private:

  VectorReaderFactory() {}

public:

  static std::unique_ptr<VectorReader> createReader(const std::string &fileName);
};



} // End namespace tl


#endif // TL_VECTOR_READER_H
