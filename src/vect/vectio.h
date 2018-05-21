#ifndef TL_VECT_IO_H
#define TL_VECT_IO_H

#include <memory>
#include <list>

#include "config_tl.h"
#include "core/defs.h"

//#ifdef HAVE_OPENCV
//#include "opencv2/core/core.hpp"
//#endif // HAVE_OPENCV

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL


#include "core/utils.h" 

class OGRFeature;
class OGRPoint;
class OGRLineString;
class OGRPolygon;
class OGRMultiPoint;
class OGRMultiLineString;
class OGRMultiPolygon;
class OGRStyleMgr;
class OGRStylePen;
class OGRStyleBrush;
class OGRStyleSymbol;
class OGRStyleLabel;

namespace TL
{

class VectorGraphics;

namespace graph
{
  class StylePen;
  class StyleBrush;
  class StyleSymbol;
  class StyleLabel;
  class GLayer;
  class GraphicEntity;
  class GPoint;
  class GLineString;
  class GPolygon;
  class GMultiPoint;
  class GMultiLineString;
  class GMultiPolygon;
  class GraphicStyle;
  class GData;
}

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

class TL_EXPORT VrtVector : public File
{

protected:

  ///*!
  // * \brief Nombre del fichero
  // */
  //std::string mName;

  /*!
   * \brief Número de capas
   */
  //int mLayersCount;

public:

  /*!
   * \brief Constructora
   */
  VrtVector();

  /*!
   * \brief Destructora
   */
  ~VrtVector() {}

  /*!
   * \brief Cierra el fichero imagen
   */
  //virtual void close() = 0;
  //virtual int open(const char *file, Mode mode = Mode::Read) = 0;
  //virtual int open(const std::string &file, Mode mode = Mode::Read) = 0;
  virtual Status create() = 0;
  //virtual void read(VectorGraphics *vector) = 0;

  ///TODO: renombrar como readLayer
  virtual void read(int id, graph::GLayer *layer) = 0;
  virtual void read(const char *name, graph::GLayer *layer) = 0;
  virtual void read(const std::string &name, graph::GLayer *layer) = 0;

  virtual int getLayersCount() const = 0;

  virtual Status createLayer(const char *layerName) = 0;
  virtual Status createLayer(const std::string &layerName) = 0;
  virtual Status createLayer(const graph::GLayer &layer) = 0;

  //virtual Status write(VectorGraphics *vector) = 0;
  virtual Status writeLayer(int id, const graph::GLayer &layer) = 0;
  virtual Status writeLayer(const char *name, const graph::GLayer &layer) = 0;
  virtual Status writeLayer(const std::string &name, const graph::GLayer &layer) = 0;
  virtual Status writePoint(int id, const graph::GPoint &gPoint) = 0;
  virtual Status writePoint(const char *name, const graph::GPoint &gPoint) = 0;
  virtual Status writePoint(const std::string &name, const graph::GPoint &gPoint) = 0;
  virtual Status writeLineString(int id, const graph::GLineString &gLineString) = 0;
  virtual Status writeLineString(const char *name, const graph::GLineString &gLineString) = 0;
  virtual Status writeLineString(const std::string &name, const graph::GLineString &gLineString) = 0;
  virtual Status writePolygon(int id, const graph::GPolygon &gPolygon) = 0;
  virtual Status writePolygon(const char *name, const graph::GPolygon &gPolygon) = 0;
  virtual Status writePolygon(const std::string &name, const graph::GPolygon &gPolygon) = 0;
  virtual Status writeMultiPoint(int id, const graph::GMultiPoint *gMultiPoint) = 0;
  virtual Status writeMultiPoint(const char *name, const graph::GMultiPoint *gMultiPoint) = 0;
  virtual Status writeMultiPoint(const std::string &name, const graph::GMultiPoint *gMultiPoint) = 0;
  virtual Status writeMultiLineString(int id, const graph::GMultiLineString *gMultiLineString) = 0;
  virtual Status writeMultiLineString(const char *name, const graph::GMultiLineString *gMultiLineString) = 0;
  virtual Status writeMultiLineString(const std::string &name, const graph::GMultiLineString *gMultiLineString) = 0;
  virtual Status writeMultiPolygon(int id,  const graph::GMultiPolygon *gMultiPolygon) = 0;
  virtual Status writeMultiPolygon(const char *name, const graph::GMultiPolygon *gMultiPolygon) = 0;
  virtual Status writeMultiPolygon(const std::string &name, const graph::GMultiPolygon *gMultiPolygon) = 0;
  

  //virtual Status writeStyles(OGRStyleMgr *ogrStyle, const graph::GraphicStyle *gStyle) = 0;
  //virtual Status writeStylePen(OGRStylePen *ogrStylePen, const graph::GraphicStyle *gStyle) = 0;
  //virtual Status writeStyleBrush(OGRStyleBrush *ogrStyleBrush, const graph::GraphicStyle *gStyle) = 0;
  //virtual Status writeStyleSymbol(OGRStyleSymbol *ogrStyleSymbol, const graph::GraphicStyle *gStyle) = 0;
  //virtual Status writeStyleLabel(OGRStyleLabel *ogrStyleLabel, const graph::GraphicStyle *gStyle) = 0;

private:

};


#ifdef HAVE_GDAL

class TL_EXPORT GdalVector : public VrtVector
{

protected:
    
  /*!
   * \brief Dataset de GDAL
   */
  GDALDataset *pDataset;

  OGRFeature *pFeature;

  /*!
   * \brief Driver GDAL
   */
  GDALDriver *pDriver;

  const char *mDriverName;

public:

  GdalVector();
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
  Status open(const char *file, Mode mode = Mode::Read) override;
  Status open(const std::string &file, Mode mode = Mode::Read) override;

  /*!
   * \brief Crea una fichero vectorial
   * \return
   */
  Status create() override;
  
  int getLayersCount() const override;

  //void read(VectorGraphics *vector) override;
  void read(int id, graph::GLayer *layer) override;
  void read(const char *name, graph::GLayer *layer) override;
  void read(const std::string &name, graph::GLayer *layer) override;
  

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const char *fileOut) override;

  Status createLayer(const char *layerName) override;
  Status createLayer(const std::string &layerName) override;
  Status createLayer(const graph::GLayer &layer) override;

  /*!
   * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
   * Si la extensión no se correspondo con un driver disponible devuelve nulo.
   * \param ext Extensión del archivo
   * \return Nombre del Driver de GDAL
   */
  static const char *getDriverFromExt(const char *ext);

  //Status write(VectorGraphics *vector) override;
  Status writeLayer(int id, const graph::GLayer &layer) override;
  Status writeLayer(const char *name, const graph::GLayer &layer) override;
  Status writeLayer(const std::string &name, const graph::GLayer &layer) override;
  Status writePoint(int id, const graph::GPoint &gPoint) override;
  Status writePoint(const char *name, const graph::GPoint &gPoint) override;
  Status writePoint(const std::string &name, const graph::GPoint &gPoint) override;
  Status writeLineString(int id, const graph::GLineString &gLineString) override;
  Status writeLineString(const char *name, const graph::GLineString &gLineString) override;
  Status writeLineString(const std::string &name, const graph::GLineString &gLineString) override;
  Status writePolygon(int id, const graph::GPolygon &gPolygon) override;
  Status writePolygon(const char *name, const graph::GPolygon &gPolygon) override;
  Status writePolygon(const std::string &name, const graph::GPolygon &gPolygon) override;
  Status writeMultiPoint(int id, const graph::GMultiPoint *gMultiPoint) override;
  Status writeMultiPoint(const char *name, const graph::GMultiPoint *gMultiPoint) override;
  Status writeMultiPoint(const std::string &name, const graph::GMultiPoint *gMultiPoint) override;
  Status writeMultiLineString(int id, const graph::GMultiLineString *gMultiLineString) override;
  Status writeMultiLineString(const char *name, const graph::GMultiLineString *gMultiLineString) override;
  Status writeMultiLineString(const std::string &name, const graph::GMultiLineString *gMultiLineString) override;
  Status writeMultiPolygon(int id,  const graph::GMultiPolygon *gMultiPolygon) override;
  Status writeMultiPolygon(const char *name, const graph::GMultiPolygon *gMultiPolygon) override;
  Status writeMultiPolygon(const std::string &name, const graph::GMultiPolygon *gMultiPolygon) override;

private:

  void read(OGRLayer *pLayer, graph::GLayer *layer);
  void readEntity(OGRGeometry *ogrGeometry, graph::GraphicEntity *gEntity);
  void readPoint(OGRPoint *ogrPoint, graph::GPoint *gPoint);
  void readLineString(OGRLineString *ogrLineString, graph::GLineString *gLineString);
  void readPolygon(OGRPolygon *ogrPolygon, graph::GPolygon *gPolygon);
  void readMultiPoint(OGRMultiPoint *ogrMultiPoint, graph::GMultiPoint *gMultiPoint);
  void readMultiLineString(OGRMultiLineString *ogrMultiLineString, graph::GMultiLineString *gMultiLineString);
  void readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon, graph::GMultiPolygon *gMultiPolygon);
  void readStyles(OGRStyleMgr *ogrStyle, graph::GraphicStyle *gStyle);
  void readStylePen(OGRStylePen *ogrStylePen, graph::GraphicStyle *gStyle);
  void readStyleBrush(OGRStyleBrush *ogrStyleBrush, graph::GraphicStyle *gStyle);
  void readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol, graph::GraphicStyle *gStyle);
  void readStyleLabel(OGRStyleLabel *ogrStyleLabel, graph::GraphicStyle *gStyle);
  //void readData();

  Status writeLayer(OGRLayer *pLayer, const graph::GLayer &layer);
  Status writePoint(OGRLayer *pLayer, const graph::GPoint &gPoint);
  Status writeLineString(OGRLayer *pLayer, const graph::GLineString &gPoint);
  Status writePolygon(OGRLayer *pLayer, const graph::GPolygon &gPolygon);
  Status writeMultiPoint(OGRLayer *pLayer, const graph::GMultiPoint *gMultiPoint);
  Status writeMultiLineString(OGRLayer *pLayer, const graph::GMultiLineString *gMultiLineString);
  Status writeMultiPolygon(OGRLayer *pLayer, const graph::GMultiPolygon *gMultiPolygon);

  void update();

};

#endif // HAVE_GDAL

/*!
 * \brief Clase contenedor y de gestión de gráficos vectoriales
 *
 * Esta clase permite crear, abrir y guardar un fichero vectorial.
 */
class TL_EXPORT VectorGraphics : public File
{
protected:
  
  std::list<std::shared_ptr<graph::GLayer>> mLayers;

  std::unique_ptr<VrtVector> mVectorFormat;

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
  Status open(const char *file, Mode mode = Mode::Read) override;
  Status open(const std::string &file, Mode mode = Mode::Read) override;

  Status create();
  Status createCopy(const char *fileOut) override;
  Status read();
  Status read(VectorGraphics *vector);
  Status read(int layerId, graph::GLayer *layer);
  Status read(const char *layerName, graph::GLayer *layer);
  Status read(const std::string &layerName, graph::GLayer *layer);

  //Status write(VectorGraphics *vector);

  int getLayersCount() const;

  Status createLayer(const char *layerName);
  Status createLayer(const std::string &layerName);
  Status createLayer(const graph::GLayer &layer);

  Status writeLayer(int id, const graph::GLayer &layer);
  Status writeLayer(const char *layerName, const graph::GLayer &layer);
  Status writeLayer(const std::string &layerName, const graph::GLayer &layer);

private:

  void update();
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

} // End namespace TL



#endif // TL_VECT_IO_H
