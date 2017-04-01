#ifndef I3D_VECT_IO_H
#define I3D_VECT_IO_H

#include <memory>

#include "core/config.h"
#include "core/defs.h"
//#ifdef HAVE_OPENCV
//#include "opencv2/core/core.hpp"
//#endif // HAVE_OPENCV

#ifdef HAVE_GDAL
I3D_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
I3D_DEFAULT_WARNINGS
#endif // HAVE_GDAL


namespace I3D
{

enum class Mode
{
  Read,
  Update,
  Create
};

enum class Status
{
  OPEN_OK,
  OPEN_FAIL,
  SAVE_OK,
  SUCCESS,
  FAILURE
};

class VrtVector
{

protected:

  /*!
   * \brief Nombre del fichero
   */
  std::string mName;

public:
  VrtVector();
  ~VrtVector();

  /*!
   * \brief Cierra el fichero imagen
   */
  virtual void close() = 0;
  virtual int open(const char *file, Mode mode = Mode::Read) = 0;
  virtual int create() = 0;
  virtual void read();
  virtual int write();

private:

};

VrtVector::VrtVector()
{
}

VrtVector::~VrtVector()
{
}

#ifdef HAVE_GDAL

class GdalVector : public VrtVector
{

protected:
    
  /*!
   * \brief Dataset de GDAL
   */
  GDALDataset *pDataset;

  /*!
   * \brief Driver GDAL
   */
  GDALDriver *driver;

public:

  GdalVector() {}
  virtual ~GdalVector() {}

  /*!
   * \brief Cierra el fichero vectorial
   */
  void close() override;

  /*!
   * \brief Abre un fichero vectorial
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  int open(const char *file, Mode mode = Mode::Read) override;

  /*!
   * \brief Crea una fichero vectorial
   * \return
   */
  int create() override;

  void read() override;

  int write() override;

  /*!
   * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
   * Si la extensión no se correspondo con un driver disponible devuelve nulo.
   * \param ext Extensión del archivo
   * \return Nombre del Driver de GDAL
   */
  static const char *getDriverFromExt(const char *ext);

private:

  void update();

};

#endif // HAVE_GDAL

/*!
 * \brief Clase contenedor y de gestión de gráficos vectoriales
 *
 * Esta clase permite crear, abrir y guardar un fichero vectorial.
 */
class VectorGraphics
{
protected:

  /*!
   * \brief Nombre del fichero
   */
  std::string mName;

  std::unique_ptr<VrtVector> mImageFormat;

public:
  VectorGraphics();
  ~VectorGraphics();

  /*!
   * \brief Cierra el archivo
   */
  void close();

  /*!
   * \brief Abre un archivo vectorial
   * \param file Nombre del fichero
   * \param mode Modo de apertura del fichero
   * \return Status
   * \see Mode
   */
  virtual Status open(const char *file, Mode mode = Mode::Read);

private:

};







//
//class VectFile
//{
//public:
//  enum class Status
//  {
//    OPEN_OK,
//    OPEN_ERROR
//  };
//
//private:
//  std::string mFile;
//  GDALDataset  *poDataset;
//
//public:
//
//  VectFile(const char *file) : mFile(file) { }
//
//  ~VectFile();
//
//  Status open();
//  
//  bool read( );
//  bool write( );
//
//  WindowI getWindow();
//
//private:
//  const char* GetOGRDriverName(const char *ext ) {
//    const char *format;
//    if      ( strcmpi( ext, ".dxf" ) == 0 )  format = "DXF";
//    else if ( strcmpi( ext, ".dwg" ) == 0 )  format = "DWG";
//    else if ( strcmpi( ext, ".dgn" ) == 0 )  format = "DGN";
//    else if ( strcmpi( ext, ".shp" ) == 0 )  format = "ESRI Shapefile";
//    else if ( strcmpi( ext, ".gml" ) == 0 )  format = "GML";
//    else if ( strcmpi( ext, ".kml" ) == 0 )  format = "LIBKML";
//    else if ( strcmpi( ext, ".kmz" ) == 0 )  format = "LIBKML";
//    else if ( strcmpi( ext, ".json") == 0 )  format = "GeoJSON";
//    else if ( strcmpi( ext, ".osm" ) == 0 )  format = "OSM";
//    else                                     format = 0;
//    return( format );
//  }
//};

} // End namespace I3D



#endif // I3D_VECT_IO_H
