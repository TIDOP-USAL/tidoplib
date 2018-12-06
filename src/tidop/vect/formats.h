#ifndef TL_GRAPHIC_VECT_FORMATS_H
#define TL_GRAPHIC_VECT_FORMATS_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <vector>

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal_priv.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#include "tidop/core/utils.h"

namespace TL
{


/*!
 * \brief Opciones del formato
 */
class TL_EXPORT VectorOptions : public FileOptions
{

public:
  
  enum class Format
  {
    SHP,
    DXF,
    DGN
  };

protected:

  Format mFormat;

public:

  VectorOptions(Format format);
  virtual ~VectorOptions();

  Format getFormat();

  virtual const char *getOptions() = 0;
};

/*!
 * \brief Clase que gestiona las opciones del formato Shape
 */
class TL_EXPORT ShapeOptions : public VectorOptions
{

public:

  /*!
   * \brief Modos de ajuste de geometr�a
   */
  enum class ADJUST_GEOM_TYPE : uint8_t
  { 
    NO,
    FIRST_SHAPE,
    ALL_SHAPES
  };

protected:

  /*!
   * \brief Codificaci�n
   */
  std::string mEncoding;

  /*!
   * \brief Fecha de modificaci�n para escribir en el encabezado DBF con el formato a�o-mes-d�a. 
   * Si no se especifica, se usa la fecha actual.
   */
  std::string mDbfDateLastUpdate;

  /*!
   * \brief 
   * Por defecto es false
   */
  bool bAdjustType;

  /*!
   * \brief Ajuste del tipo de geometr�a
   * Define c�mo se calcula el tipo de geometr�a de capa, en particular para distinguir shapefiles que tienen formas con valores significativos en la dimensi�n M desde los que los valores M se establecen en el valor nodata
   * El valor por defecto es FIRST_SHAPE
   * \see ADJUST_GEOM_TYPE
   */
  ADJUST_GEOM_TYPE mAdjustGeomType;
  
  /*!
   * \brief
   */
  bool bAutoRepack;

  /*!
   * \brief 
   */
  bool bDbfEofChar;

public:

  ShapeOptions();
  ~ShapeOptions();

  const char *getOptions() override;

  void enableAdjustType(bool value = true);
  void enableAutoRepac(bool value = true);
  void enableDbfEofChar(bool value = true);

  std::string getEncoding() const;
  void setEncoding(const std::string &encoding);

  std::string getDbfDateLastUpdate() const;
  void setDbfDateLastUpdate(const std::string &date);

  ADJUST_GEOM_TYPE getAdjustGeomType() const;
  void setAdjustGeomType(ADJUST_GEOM_TYPE type);

};


}



#endif // TL_GRAPHIC_VECT_FORMATS_H
