#ifndef TL_VECT_IO_H
#define TL_VECT_IO_H

#include <memory>
#include <list>

#include "config_tl.h"
#include "tidop/core/defs.h"


#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal_priv.h"
#include "cpl_conv.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL


#include "tidop/core/utils.h"
#include "tidop/geospatial/crs.h"

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
class VectorOptions;

namespace graph
{
class GraphicEntity;
class GPoint;
class GLineString;
class GPolygon;
class GMultiPoint;
class GMultiLineString;
class GMultiPolygon;
class GLayer;
class GPoint3D;
class GLineString3D;
class GPolygon3D;
class GMultiPoint3D;
class GMultiLineString3D;
class GMultiPolygon3D;
class StylePen;
class StyleBrush;
class StyleSymbol;
class StyleLabel;
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

class TL_EXPORT VrtVector
  : public File
{

public:

  /*!
   * \brief Constructora
   */
  VrtVector();

  /*!
   * \brief Destructora
   */
  ~VrtVector() override {}

  virtual Status create() = 0;

  ///TODO: renombrar como readLayer
  virtual void read(int id, graph::GLayer *layer) = 0;
  virtual void read(const std::string &name, graph::GLayer *layer) = 0;

  virtual int layersCount() const = 0;

  virtual Status createLayer(const std::string &layerName) = 0;
  virtual Status createLayer(const graph::GLayer &layer) = 0;

  virtual Status writeLayer(int id, const graph::GLayer &layer) = 0;
  virtual Status writeLayer(const std::string &name, const graph::GLayer &layer) = 0;
  virtual Status writePoint(int id, const std::shared_ptr<graph::GPoint> &gPoint) = 0;
  virtual Status writePoint(const std::string &name, const std::shared_ptr<graph::GPoint> &gPoint) = 0;
  virtual Status writeLineString(int id, const std::shared_ptr<graph::GLineString> &gLineString) = 0;
  virtual Status writeLineString(const std::string &name, const std::shared_ptr<graph::GLineString> &gLineString) = 0;
  virtual Status writePolygon(int id, const std::shared_ptr<graph::GPolygon> &gPolygon) = 0;
  virtual Status writePolygon(const std::string &name, const std::shared_ptr<graph::GPolygon> &gPolygon) = 0;
  virtual Status writeMultiPoint(int id, const std::shared_ptr<graph::GMultiPoint> &gMultiPoint) = 0;
  virtual Status writeMultiPoint(const std::string &name, const std::shared_ptr<graph::GMultiPoint> &gMultiPoint) = 0;
  virtual Status writeMultiLineString(int id, const std::shared_ptr<graph::GMultiLineString> &gMultiLineString) = 0;
  virtual Status writeMultiLineString(const std::string &name, const std::shared_ptr<graph::GMultiLineString> &gMultiLineString) = 0;
  virtual Status writeMultiPolygon(int id,  const std::shared_ptr<graph::GMultiPolygon> &gMultiPolygon) = 0;
  virtual Status writeMultiPolygon(const std::string &name, const std::shared_ptr<graph::GMultiPolygon> &gMultiPolygon) = 0;
  

  //virtual Status writeStyles(OGRStyleMgr *ogrStyle, const graph::GraphicStyle *gStyle) = 0;
  //virtual Status writeStylePen(OGRStylePen *ogrStylePen, const graph::GraphicStyle *gStyle) = 0;
  //virtual Status writeStyleBrush(OGRStyleBrush *ogrStyleBrush, const graph::GraphicStyle *gStyle) = 0;
  //virtual Status writeStyleSymbol(OGRStyleSymbol *ogrStyleSymbol, const graph::GraphicStyle *gStyle) = 0;
  //virtual Status writeStyleLabel(OGRStyleLabel *ogrStyleLabel, const graph::GraphicStyle *gStyle) = 0;

private:

};


#ifdef HAVE_GDAL

/*!
 * \brief Clase para la lectura/escritura de ficheros vectoriales con GDAL
 */
class TL_EXPORT GdalVector
  : public VrtVector
{

protected:
    
  /*!
   * \brief Dataset de GDAL
   */
  GDALDataset *pDataset;

  /*!
   * \brief Driver GDAL
   */
  GDALDriver *pDriver;

  /*!
   * \brief Sistema de referencia
   */
  OGRSpatialReference mSpatialReference;

  /*!
   * \brief Nombre del driver
   */
  const char *mDriverName;


public:

  /*!
   * \brief Constructora
   */
  GdalVector();

  /*!
   * \brief Destructora
   */
  virtual ~GdalVector() override;

  /*!
   * \brief Cierra el fichero vectorial
   */
  void close() override;

  /*!
   * \brief Abre un fichero vectorial especificando las opciones del formato
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \param[in] options Opciones del formato
   * \return Error
   * \see Mode
   */
  Status open(const std::string &file, Mode mode = Mode::Update, FileOptions *options = nullptr) override;

  /*!
   * \brief Crea una fichero vectorial
   * \return Error
   */
  Status create() override;
  
  /*!
   * \brief Devuelve el número de capas
   * \return Número de capas
   */
  int layersCount() const override;

  /*!
   * \brief Lectura de una capa
   * \param[in] id Identificador de la capa
   * \param[out] layer Objeto GLayer que corresponde a la capa
   */
  void read(int id, graph::GLayer *layer) override;

  /*!
   * \brief read
   * \param[in] name Nombre de la capa
   * \param[out] layer Objeto GLayer que corresponde a la capa
   */
  void read(const std::string &name, graph::GLayer *layer) override;
  

  /*!
   * \brief Guarda una copia con otro nonbre
   * \param[in] fileOut Nombre del fichero destino
   * \return Error
   */
  Status createCopy(const std::string &fileOut) override;

  /*!
   * \brief Crea una capa nueva
   * \param[in] layerName Nombre de la capa
   * \return Error
   */
  Status createLayer(const std::string &layerName) override;

  /*!
   * \brief Crea una capa nueva
   * \param[in] layer Capa tipo GLayer
   * \return Error
   */
  Status createLayer(const graph::GLayer &layer) override;

  /*!
   * \brief Devuelve el nombre del driver de GDAL correspondiente a una extensión de archivo
   * Si la extensión no se correspondo con un driver disponible devuelve nulo.
   * \param ext Extensión del archivo
   * \return Nombre del Driver de GDAL
   */
  static const char *driverFromExt(const char *ext);

  /*!
   * \brief Escribe una capa
   * \param[in] id Identificador de la capa
   * \param[in] layer Capa
   * \return
   */
  Status writeLayer(int id, const graph::GLayer &layer) override;

  /*!
   * \brief Escribe una capa
   * \param[in] name Nombre de la capa
   * \param[in] layer Capa
   * \return
   */
  Status writeLayer(const std::string &name, const graph::GLayer &layer) override;

  /*!
   * \brief Escribe un punto en una capa
   * \param[in] id Identificador de la capa
   * \param[in] gPoint Punto
   * \return Error
   */
  Status writePoint(int id, const std::shared_ptr<graph::GPoint> &gPoint) override;

  /*!
   * \brief Escribe un punto en una capa
   * \param[in] name Nombre de la capa
   * \param[in] gPoint Punto
   * \return Error
   */
  Status writePoint(const std::string &name, const std::shared_ptr<graph::GPoint> &gPoint) override;

  /*!
   * \brief Escribe una polilinea en una capa
   * \param[in] id Identificador de la capa
   * \param[in] gLineString Polilinea
   * \return Error
   */
  Status writeLineString(int id, const std::shared_ptr<graph::GLineString> &gLineString) override;

  /*!
   * \brief Escribe una polilinea en una capa
   * \param[in] name Nombre de la capa
   * \param[in] gLineString Polilinea
   * \return Error
   */
  Status writeLineString(const std::string &name, const std::shared_ptr<graph::GLineString> &gLineString) override;

  /*!
   * \brief Escribe un poligono en una capa
   * \param[in] id Identificador de la capa
   * \param[in] gPolygon Poligono
   * \return Error
   */
  Status writePolygon(int id, const std::shared_ptr<graph::GPolygon> &gPolygon) override;

  /*!
   * \brief Escribe un poligono en una capa
   * \param[in] name Identificador de la capa
   * \param[in] gPolygon Poligono
   * \return Error
   */
  Status writePolygon(const std::string &name, const std::shared_ptr<graph::GPolygon> &gPolygon) override;

  /*!
   * \brief Escribe una entidad multi-punto en una capa
   * \param[in] id Identificador de la capa
   * \param[in] gMultiPoint Multi-punto
   * \return Error
   */
  Status writeMultiPoint(int id, const std::shared_ptr<graph::GMultiPoint> &gMultiPoint) override;

  /*!
   * \brief Escribe una entidad multi-polilinea en una capa
   * \param[in] name Identificador de la capa
   * \param[in] gMultiPoint Multi-polilinea
   * \return Error
   */
  Status writeMultiPoint(const std::string &name, const std::shared_ptr<graph::GMultiPoint> &gMultiPoint) override;

  /*!
   * \brief Escribe una entidad multi-polilinea en una capa
   * \param[in] id Identificador de la capa
   * \param[in] gMultiLineString Multi-polilinea
   * \return Error
   */
  Status writeMultiLineString(int id, const std::shared_ptr<graph::GMultiLineString> &gMultiLineString) override;

  /*!
   * \brief Escribe una entidad multi-polilinea en una capa
   * \param[in] name Nombre de la capa
   * \param[in] gMultiLineString Multi-polilinea
   * \return Error
   */
  Status writeMultiLineString(const std::string &name, const std::shared_ptr<graph::GMultiLineString> &gMultiLineString) override;

  /*!
   * \brief Escribe una entidad multi-poligono en una capa
   * \param[in] id Identificador de la capa
   * \param[in] gMultiPolygon Multi-poligono
   * \return Error
   */
  Status writeMultiPolygon(int id,  const std::shared_ptr<graph::GMultiPolygon> &gMultiPolygon) override;

  /*!
   * \brief Escribe una entidad multi-poligono en una capa
   * \param[in] name Nombre de la capa
   * \param[in] gMultiPolygon Multi-poligono
   * \return Error
   */
  Status writeMultiPolygon(const std::string &name, const std::shared_ptr<graph::GMultiPolygon> &gMultiPolygon) override;

private:

  void read(OGRLayer *pLayer, graph::GLayer *layer);
  std::shared_ptr<graph::GraphicEntity> readEntity(OGRGeometry *ogrGeometry);
  std::shared_ptr<graph::GPoint> readPoint(OGRPoint *ogrPoint);
  std::shared_ptr<graph::GPoint3D> readPoint3D(OGRPoint *ogrPoint);
  std::shared_ptr<graph::GLineString> readLineString(OGRLineString *ogrLineString);
  std::shared_ptr<graph::GLineString3D> readLineString3D(OGRLineString *ogrLineString);
  std::shared_ptr<graph::GPolygon> readPolygon(OGRPolygon *ogrPolygon);
  std::shared_ptr<graph::GPolygon3D> readPolygon3D(OGRPolygon *ogrPolygon);
  std::shared_ptr<graph::GMultiPoint> readMultiPoint(OGRMultiPoint *ogrMultiPoint);
  std::shared_ptr<graph::GMultiPoint3D> readMultiPoint3D(OGRMultiPoint *ogrMultiPoint);
  std::shared_ptr<graph::GMultiLineString> readMultiLineString(OGRMultiLineString *ogrMultiLineString);
  std::shared_ptr<graph::GMultiLineString3D> readMultiLineString3D(OGRMultiLineString *ogrMultiLineString);
  std::shared_ptr<graph::GMultiPolygon> readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon);
  std::shared_ptr<graph::GMultiPolygon3D> readMultiPolygon3D(OGRMultiPolygon *ogrMultiPolygon);
  void readStyles(OGRStyleMgr *ogrStyle, std::shared_ptr<graph::GraphicEntity> &gStyle);
  std::shared_ptr<graph::StylePen> readStylePen(OGRStylePen *ogrStylePen);
  std::shared_ptr<graph::StyleBrush> readStyleBrush(OGRStyleBrush *ogrStyleBrush);
  std::shared_ptr<graph::StyleSymbol> readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol);
  std::shared_ptr<graph::StyleLabel> readStyleLabel(OGRStyleLabel *ogrStyleLabel);

  //void readData();

  Status writeLayer(OGRLayer *pLayer, const graph::GLayer &layer);
  Status writePoint(OGRFeature *ogrFeature, const std::shared_ptr<graph::GPoint> &gPoint);
  Status writePoint(OGRFeature *ogrFeature, const std::shared_ptr<graph::GPoint3D> &gPoint3D);
  Status writeLineString(OGRFeature *ogrFeature, const std::shared_ptr<graph::GLineString> &gLineString);
  Status writeLineString(OGRFeature *ogrFeature, const std::shared_ptr<graph::GLineString3D> &gLineString);
  Status writePolygon(OGRFeature *ogrFeature, const std::shared_ptr<graph::GPolygon> &gPolygon);
  Status writePolygon(OGRFeature *ogrFeature, const std::shared_ptr<graph::GPolygon3D> &gPolygon3D);
  Status writeMultiPoint(OGRFeature *ogrFeature, const std::shared_ptr<graph::GMultiPoint> &gMultiPoint);
  Status writeMultiPoint(OGRFeature *ogrFeature, const std::shared_ptr<graph::GMultiPoint3D> &gMultiPoint3D);
  Status writeMultiLineString(OGRFeature *ogrFeature, const std::shared_ptr<graph::GMultiLineString> &gMultiLineString);
  Status writeMultiLineString(OGRFeature *ogrFeature, const std::shared_ptr<graph::GMultiLineString3D> &gMultiLineString3D);
  Status writeMultiPolygon(OGRFeature *ogrFeature, const std::shared_ptr<graph::GMultiPolygon> &gMultiPolygon);
  Status writeMultiPolygon(OGRFeature *ogrFeature, const std::shared_ptr<graph::GMultiPolygon3D> &gMultiPolygon3D);

  void update();

};

#endif // HAVE_GDAL

///TODO: Separar en una clase vector y otra clase VectorHandler. 
/// La clase VectorHandler seria una virtual y se extenderia para los ficheros que se lean con GDAL 
/// o de otra forma.

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

  /*!
   * \brief Constructora
   */
  VectorGraphics();

  /*!
   * \brief Destructora
   */
  ~VectorGraphics() override;

  /*!
   * \brief Cierra el archivo
   */
  void close() override;

  /*!
   * \brief Abre un fichero vectorial especificando las opciones del formato
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura   
   * \param[in] options Opciones del formato
   * \return Error
   * \see Mode
   */
  Status open(const std::string &file, Mode mode = Mode::Update, FileOptions *options = nullptr) override;

  /*!
   * \brief Crea un fichero
   * \return Error
   */
  Status create();

  /*!
   * \brief Crea una copia de un fichero
   * \param[in] fileOut Fichero de salida
   * \return Error
   */
  Status createCopy(const std::string &fileOut) override;

  /*!
   * \brief Lectura
   * \return Error
   */
  //Status read();

  /*!
   * \brief Lee una capa del archivo
   * \param[in] layerId Identificador de la capa
   * \param[out] layer Capa
   * \return Error
   */
  Status read(int layerId, graph::GLayer *layer);

  /*!
   * \brief Lee una capa del archivo
   * \param[in] layerName Nombre de la capa
   * \param[out] layer Capa
   * \return Error
   */
  Status read(const std::string &layerName, graph::GLayer *layer);

  /*!
   * \brief Devuelve el número de capas
   * \return Número de capas
   */
  int layersCount() const;

  /*!
   * \brief Crea una capa vacia
   * \param[in] layerName Nombre de la capa
   * \return Error
   */
  Status createLayer(const std::string &layerName);

  /*!
   * \brief Crea una capa a partir de un objeto GLayer
   * \param[in] layer Capa (objero GLayer)
   * \return Error
   */
  Status createLayer(const graph::GLayer &layer);

  /*!
   * \brief Escribe una capa
   * \param[in] id Identificador de la capa
   * \param[in] layer Capa
   * \return Error
   */
  Status writeLayer(int id, const graph::GLayer &layer);

  /*!
   * \brief Escribe una capa
   * \param[in] layerName Nombre capa
   * \param[in] layer Capa
   * \return Error
   */
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
