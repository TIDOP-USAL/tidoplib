#include "vectio.h"

#include "tidop/core/utils.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogrsf_frmts.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/entities/linestring.h"
#include "tidop/graphic/entities/polygon.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/styles.h"
#include "tidop/graphic/color.h"
#include "tidop/graphic/font.h"

// filesystem
#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

namespace tl
{

using namespace graph;
using namespace geometry;


VrtVector::VrtVector()
  : File()/*,
    mLayersCount(0)*/
{}

//int VrtVector::getLayersCount() const
//{
//  return mLayersCount;
//}

#ifdef HAVE_GDAL
GdalVector::GdalVector()
  : VrtVector(),
    pDataset(nullptr),
    pDriver(nullptr),
    mSpatialReference(),
    mDriverName("")
{
  GDALAllRegister();
  //RegisterGdal::init();
}

GdalVector::~GdalVector()
{
  if (pDataset) GDALClose(pDataset), pDataset = nullptr;

}

void GdalVector::close()
{
  if (pDataset) GDALClose(pDataset), pDataset = nullptr;
  mFile = "";
  //mLayersCount = 0;
}

GdalVector::Status GdalVector::open(const char *file, Mode mode, FileOptions *options)
{
  //TODO: Se puede sacar a una clase de mas bajo nivel esta secci?n ...
  close();

  mFile = file;
//  char ext[TL_MAX_EXT];
//  if (getFileExtension(file, ext, TL_MAX_EXT) != 0) return Status::OPEN_FAIL;
  std::string ext = fs::extension(mFile);

  mDriverName = getDriverFromExt(ext.c_str());
  if (mDriverName == nullptr) return Status::OPEN_FAIL;

  // No parece que se necesite
  //GDALAccess gdal_access;
  //switch (mode) {
  //case TKL::Mode::Read:
  //  gdal_access = GA_ReadOnly;
  //  break;
  //case TKL::Mode::Update:
  //  gdal_access = GA_Update;
  //  break;
  //case TKL::Mode::Create:
  //  gdal_access = GA_Update;
  //  break;
  //default:
  //  gdal_access = GA_ReadOnly;
  //  break;
  //}
  //....

  if (mode == Mode::Create) {

    pDriver = GetGDALDriverManager()->GetDriverByName(mDriverName);
    if (pDriver == nullptr) return Status::OPEN_FAIL;

    // Se crea el directorio si no existe
    char dir[TL_MAX_PATH];
    if ( getFileDriveDir(file, dir, TL_MAX_PATH) == 0 )
      if ( createDir(dir) == -1) return Status::OPEN_FAIL;
    return Status::OPEN_OK;
  } else {

    pDataset = (GDALDataset*)GDALOpenEx( file, GDAL_OF_VECTOR, nullptr, nullptr/*options->getOptions()*/, nullptr ); //GDALOpen( file, gdal_access);
    if (pDataset == nullptr) {
      return Status::OPEN_FAIL;
    } else {
      pDriver = pDataset->GetDriver();
      update();
      return Status::OPEN_OK;
    }

  }
}

GdalVector::Status GdalVector::open(const std::string &file, Mode mode, FileOptions *options)
{
  return open(file.c_str(), mode, options);
}

///TODO: ver opciones de creaci?n: 2D, 3D, ...
GdalVector::Status  GdalVector::create()
{
  if (pDriver == nullptr) {
    msgError("Utilice el modo Create para abrir el archivo");
    return Status::FAILURE;
  }

  if (pDataset) GDALClose(pDataset), pDataset = nullptr;
  pDataset = pDriver->Create(mFile.c_str(), 0, 0, 0, GDT_Unknown, nullptr);

  if (pDataset == nullptr) {
    msgError("Creation of output file failed.");
    return Status::FAILURE;
  }

  update();

  return Status::SUCCESS;
}

int GdalVector::getLayersCount() const
{
  return pDataset->GetLayerCount();
}

//void GdalVector::read(VectorGraphics *vector)
//{
//  OGRLayer  *pLayer;
//}

void GdalVector::read(int id, GLayer *layer)
{
  read(pDataset->GetLayer(id), layer);
}

void GdalVector::read(const char *name, GLayer *layer)
{
  read(pDataset->GetLayerByName(name), layer);
}

void GdalVector::read(const std::string &name, GLayer *layer)
{
  read(pDataset->GetLayerByName(name.c_str()), layer);
}

GdalVector::Status GdalVector::createCopy(const char *fileOut)
{
  return Status::FAILURE;
}

GdalVector::Status GdalVector::createLayer(const char *layerName)
{
  char **encoding = nullptr;
  encoding = CSLAddString(encoding,"ENCODING=UTF-8");

  OGRLayer *layer = nullptr;
  if (strcmp(mDriverName, "DXF") == 0 || strcmp(mDriverName, "DGN") == 0) {
    if (pDataset->GetLayerCount() == 0) {
      /// S?lo soportan la creaci?n de una capa con lo cual se a?adir? siempre a la capa "0"
      layer = pDataset->CreateLayer("0", &mSpatialReference /*poOutputSRS*/, (OGRwkbGeometryType)wkbUnknown, encoding);
    }
  } else if (strcmp(mDriverName, "SHP") == 0) {
    layer = pDataset->CreateLayer(layerName, &mSpatialReference, (OGRwkbGeometryType)wkbUnknown, encoding);
  } else {
    layer = pDataset->CreateLayer(layerName, &mSpatialReference, (OGRwkbGeometryType)wkbUnknown, encoding);
  }

  if (layer == nullptr) {
    msgError("Layer creation failed.");
    return Status::FAILURE;
  } else {

    return Status::SUCCESS;
  }
}

GdalVector::Status GdalVector::createLayer(const std::string &layerName)
{
  return createLayer(layerName.c_str());
}

GdalVector::Status GdalVector::createLayer(const graph::GLayer &layer)
{
  return createLayer(layer.getName());
}

const char *GdalVector::getDriverFromExt(const char *ext)
{
  const char *format;
  if      (boost::iequals( ext, ".dxf" ))  format = "DXF";
  else if (boost::iequals( ext, ".dwg" ))  format = "DWG";
  else if (boost::iequals( ext, ".dgn" ))  format = "DGN";
  else if (boost::iequals( ext, ".shp" ))  format = "ESRI Shapefile";
  else if (boost::iequals( ext, ".gml" ))  format = "GML";
  else if (boost::iequals( ext, ".kml" ))  format = "LIBKML";
  else if (boost::iequals( ext, ".kmz" ))  format = "LIBKML";
  else if (boost::iequals( ext, ".json"))  format = "GeoJSON";
  else if (boost::iequals( ext, ".osm" ))  format = "OSM";
  else format = nullptr;
  return format;
}

//GdalVector::Status GdalVector::write(VectorGraphics *vector)
//{
//  return Status::FAILURE;
//}

GdalVector::Status GdalVector::writeLayer(int id, const graph::GLayer &layer)
{
  return writeLayer(pDataset->GetLayer(id), layer);
}

GdalVector::Status GdalVector::writeLayer(const char *name, const graph::GLayer &layer)
{
  return writeLayer(pDataset->GetLayerByName(name), layer);
}

GdalVector::Status GdalVector::writeLayer(const std::string &name, const graph::GLayer &layer)
{
  return writeLayer(pDataset->GetLayerByName(name.c_str()), layer);
}

GdalVector::Status GdalVector::writePoint(int id, const std::shared_ptr<GPoint> &gPoint)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayer(id)->GetLayerDefn());
  return writePoint(ogrFeature, gPoint);
}

GdalVector::Status GdalVector::writePoint(const char *name, const std::shared_ptr<GPoint> &gPoint)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name)->GetLayerDefn());
  return writePoint(ogrFeature, gPoint);
}

GdalVector::Status GdalVector::writePoint(const std::string &name, const std::shared_ptr<GPoint> &gPoint)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name.c_str())->GetLayerDefn());
  return writePoint(ogrFeature, gPoint);
}

GdalVector::Status GdalVector::writeLineString(int id, const std::shared_ptr<GLineString> &gLineString)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayer(id)->GetLayerDefn());
  return writeLineString(ogrFeature, gLineString);
}

GdalVector::Status GdalVector::writeLineString(const char *name, const std::shared_ptr<GLineString> &gLineString)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name)->GetLayerDefn());
  return writeLineString(ogrFeature, gLineString);
}

GdalVector::Status GdalVector::writeLineString(const std::string &name, const std::shared_ptr<GLineString> &gLineString)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name.c_str())->GetLayerDefn());
  return writeLineString(ogrFeature, gLineString);
}

GdalVector::Status GdalVector::writePolygon(int id, const std::shared_ptr<GPolygon> &gPolygon)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayer(id)->GetLayerDefn());
  return writePolygon(ogrFeature, gPolygon);
}

GdalVector::Status GdalVector::writePolygon(const char *name, const std::shared_ptr<GPolygon> &gPolygon)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name)->GetLayerDefn());
  return writePolygon(ogrFeature, gPolygon);
}

GdalVector::Status GdalVector::writePolygon(const std::string &name, const std::shared_ptr<GPolygon> &gPolygon)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name.c_str())->GetLayerDefn());
  return writePolygon(ogrFeature, gPolygon);
}

GdalVector::Status GdalVector::writeMultiPoint(int id, const std::shared_ptr<GMultiPoint> &gMultiPoint)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayer(id)->GetLayerDefn());
  return writeMultiPoint(ogrFeature, gMultiPoint);
}

GdalVector::Status GdalVector::writeMultiPoint(const char *name, const std::shared_ptr<GMultiPoint> &gMultiPoint)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name)->GetLayerDefn());
  return writeMultiPoint(ogrFeature, gMultiPoint);
}

GdalVector::Status GdalVector::writeMultiPoint(const std::string &name, const std::shared_ptr<GMultiPoint> &gMultiPoint)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name.c_str())->GetLayerDefn());
  return writeMultiPoint(ogrFeature, gMultiPoint);
}

GdalVector::Status GdalVector::writeMultiLineString(int id, const std::shared_ptr<GMultiLineString> &gMultiLineString)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayer(id)->GetLayerDefn());
  return writeMultiLineString(ogrFeature, gMultiLineString);
}

GdalVector::Status GdalVector::writeMultiLineString(const char *name, const std::shared_ptr<GMultiLineString> &gMultiLineString)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name)->GetLayerDefn());
  return writeMultiLineString(ogrFeature, gMultiLineString);
}

GdalVector::Status GdalVector::writeMultiLineString(const std::string &name, const std::shared_ptr<GMultiLineString> &gMultiLineString)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name.c_str())->GetLayerDefn());
  return writeMultiLineString(ogrFeature, gMultiLineString);
}

GdalVector::Status GdalVector::writeMultiPolygon(int id,  const std::shared_ptr<GMultiPolygon> &gMultiPolygon)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayer(id)->GetLayerDefn());
  return writeMultiPolygon(ogrFeature, gMultiPolygon);
}

GdalVector::Status GdalVector::writeMultiPolygon(const char *name, const std::shared_ptr<GMultiPolygon> &gMultiPolygon)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name)->GetLayerDefn());
  return writeMultiPolygon(ogrFeature, gMultiPolygon);
}

GdalVector::Status GdalVector::writeMultiPolygon(const std::string &name, const std::shared_ptr<GMultiPolygon> &gMultiPolygon)
{
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pDataset->GetLayerByName(name.c_str())->GetLayerDefn());
  return writeMultiPolygon(ogrFeature, gMultiPolygon);
}

/* private */
void GdalVector::read(OGRLayer *pLayer, GLayer *layer)
{
  OGRFeature *ogrFeature;
  while ( (ogrFeature = pLayer->GetNextFeature()) != nullptr ) {

    const char *layerName = nullptr;
    if (strcmp(mDriverName , "DXF") == 0) {
      layerName = ogrFeature->GetFieldAsString(0);
    } else if ( strcmp(mDriverName , "DGN" ) == 0 ) {
      layerName = ogrFeature->GetFieldAsString(1);
    } else {
      layerName = pLayer->GetName();
    }

    layer->setName(layerName);
    if (OGRGeometry *pGeometry = ogrFeature->GetGeometryRef()) {
      std::shared_ptr<GraphicEntity> entity;
      OGRStyleMgr *ogrStyleMgr = nullptr;
      try {
        readEntity(pGeometry, entity);
        ogrStyleMgr = new OGRStyleMgr();
        ogrStyleMgr->GetStyleString(ogrFeature);
        //readStyles(ogrStyleMgr, std::dynamic_pointer_cast<GraphicStyle>(entity));
        readStyles(ogrStyleMgr, entity);
        //readData()
        layer->push_back(entity);

      } catch (std::exception &e) {
        msgError(e.what());
      }
      if (ogrStyleMgr) delete ogrStyleMgr, ogrStyleMgr = nullptr;

          //GVE_Base *gent;
          //gent = ReadOgrGeometry( poGeometry, ly );
          //if ( gent ) {
            //// Estilos
            //GVE_Style gs;
            //ReadOgrStyles( poStyleMgr, &gs );
            //gent->SetStyles( &gs );
            // Atributos
            //CSL_SetStr sat;
            //ReadOgrFeatureVal( poFeature, &sat );
            //gent->SetAttributes( &sat );

          //} else {
          //  // Salida a fichero log con errores.  GDAL en OpenCv lo env?a a cout
          //}
        }
    OGRFeature::DestroyFeature(ogrFeature);
  }
}

void GdalVector::readEntity(OGRGeometry *ogrGeometry, std::shared_ptr<GraphicEntity> &gEntity)
{
  OGRwkbGeometryType type;
  if ( wkbHasZ(ogrGeometry->getGeometryType()) )
    type = wkbFlatten(ogrGeometry->getGeometryType());
  else
    type = wkbSetZ(ogrGeometry->getGeometryType());

  OGRwkbGeometryType::wkbLineString25D;
  OGRwkbGeometryType::wkbLineStringM;
  OGRwkbGeometryType::wkbLineStringZM;
  int dim = ogrGeometry->getCoordinateDimension();
  switch ( type ) {
  case wkbUnknown:
    break;
  case wkbPoint:
    gEntity = std::make_shared<GPoint>();
    readPoint(static_cast<OGRPoint *>(ogrGeometry), gEntity);
    break;
  case wkbLineString:
    gEntity = std::make_shared<GLineString>();
    readLineString(static_cast<OGRLineString *>(ogrGeometry), gEntity);
    break;
  case wkbPolygon:
    gEntity = std::make_shared<GPolygon>();
    readPolygon(static_cast<OGRPolygon *>(ogrGeometry), gEntity);
    break;
  case wkbMultiPoint:
    gEntity = std::make_shared<GMultiPoint>();
    readMultiPoint(static_cast<OGRMultiPoint *>(ogrGeometry), gEntity);
    break;
  case wkbMultiLineString:
    gEntity = std::make_shared<GMultiLineString>();
    readMultiLineString(static_cast<OGRMultiLineString *>(ogrGeometry), gEntity);
    break;
  case wkbMultiPolygon:
    gEntity = std::make_shared<GMultiPolygon>();
    readMultiPolygon(static_cast<OGRMultiPolygon *>(ogrGeometry), gEntity);
    break;
  case wkbGeometryCollection:
    break;
#if  GDAL_VERSION_MAJOR >= 2
  case wkbCircularString:
    break;
  case wkbCompoundCurve:
    break;
  case wkbCurvePolygon:
    break;
  case wkbMultiCurve:
    break;
  case wkbMultiSurface:
    break;
#if GDAL_VERSION_MINOR >= 1
  case wkbCurve:
    break;
  case wkbSurface:
    break;
#endif
  case wkbCircularStringZ:
    break;
  case wkbCompoundCurveZ:
    break;
  case wkbCurvePolygonZ:
    break;
  case wkbMultiCurveZ:
    break;
  case wkbMultiSurfaceZ:
    break;
#if GDAL_VERSION_MINOR >= 1
  case wkbCurveZ:
    break;
  case wkbSurfaceZ:
    break;
  case wkbPointM:
    break;
  case wkbLineStringM:
    break;
  case wkbPolygonM:
    break;
  case wkbMultiPointM:
    break;
  case wkbMultiLineStringM:
    break;
  case wkbMultiPolygonM:
    break;
  case wkbGeometryCollectionM:
    break;
  case wkbCircularStringM:
    break;
  case wkbCompoundCurveM:
    break;
  case wkbCurvePolygonM:
    break;
  case wkbMultiCurveM:
    break;
  case wkbMultiSurfaceM:
    break;
  case wkbCurveM:
    break;
  case wkbSurfaceM:
    break;
  case wkbPointZM:
    break;
  case wkbLineStringZM:
    break;
  case wkbPolygonZM:
    break;
  case wkbMultiPointZM:
    break;
  case wkbMultiLineStringZM:
    break;
  case wkbMultiPolygonZM:
    break;
  case wkbGeometryCollectionZM:
    break;
  case wkbCircularStringZM:
    break;
  case wkbCompoundCurveZM:
    break;
  case wkbCurvePolygonZM:
    break;
  case wkbMultiCurveZM:
    break;
  case wkbMultiSurfaceZM:
    break;
  case wkbCurveZM:
    break;
  case wkbSurfaceZM:
    break;
#endif
#endif
  case wkbPoint25D:
    break;
  case wkbLineString25D:
    break;
  case wkbPolygon25D:
    gEntity = std::make_shared<GPolygon3D>();
    if (dim == 2)
      readPolygon(static_cast<OGRPolygon *>(ogrGeometry), gEntity);
    else
      readPolygon(static_cast<OGRPolygon *>(ogrGeometry), gEntity);
    break;
  case wkbMultiPoint25D:
    break;
  case wkbMultiLineString25D:
    break;
  case wkbMultiPolygon25D:
    break;
  case wkbGeometryCollection25D:
    break;
  default:
    break;
  }
}

void GdalVector::readPoint(OGRPoint *ogrPoint, std::shared_ptr<GraphicEntity> &gPoint)
{
  std::dynamic_pointer_cast<GPoint>(gPoint)->x = ogrPoint->getX();
  std::dynamic_pointer_cast<GPoint>(gPoint)->y = ogrPoint->getY();
}

void GdalVector::readLineString(OGRLineString *ogrLineString, std::shared_ptr<GraphicEntity> &gLineString)
{
  int n = ogrLineString->getNumPoints();
  std::shared_ptr<GLineString> lineString = std::dynamic_pointer_cast<GLineString>(gLineString);
  lineString->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    //gLineString->operator[](i) = Point<float>(ogrLineString->getX(i), ogrLineString->getY(i));
    (*lineString)[static_cast<size_t>(i)].x = ogrLineString->getX(i);
    (*lineString)[static_cast<size_t>(i)].y = ogrLineString->getY(i);
  }
}

void GdalVector::readPolygon(OGRPolygon *ogrPolygon, std::shared_ptr<GraphicEntity> &gPolygon)
{
  // Contorno exterior
  OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
  int n = ogrLinearRing->getNumPoints();
  std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(gPolygon);
  polygon->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    ///TODO: Es mas rapida la asignaci?n con movimiento
    (*polygon)[i] = PointD(ogrLinearRing->getX(i), ogrLinearRing->getY(i));
    /// o asignar directamente
    //(*gPolygon)[i].x = ogrLinearRing->getX(i);
    //(*gPolygon)[i].y = ogrLinearRing->getY(i);
  }
  //TODO: modificar la clase Polygon para permitir Islas
  //n = ogrPolygon->getNumInteriorRings();
  //for (int i = 0; i < n; i++) {
  //  ogrLinearRing = ogrPolygon->getInteriorRing(i);
  //  int nr = ogrLinearRing->getNumPoints();
  //  for (int j = 0; j < nr; j++) {
  //    gPolygon->addRing(PointD(ogrLinearRing->getX(j), ogrLinearRing->getY(j)));
  //  }
  //}
}

void GdalVector::readMultiPoint(OGRMultiPoint *ogrMultiPoint, std::shared_ptr<GraphicEntity> &gMultiPoint)
{
  int n = ogrMultiPoint->getNumGeometries();
  std::shared_ptr<GMultiPoint> multiPoint = std::dynamic_pointer_cast<GMultiPoint>(gMultiPoint);
  multiPoint->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    OGRPoint *ogrPoint = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(i));
    (*multiPoint)[static_cast<size_t>(i)].x = ogrPoint->getX();
    (*multiPoint)[static_cast<size_t>(i)].y = ogrPoint->getY();
  }
}

void GdalVector::readMultiLineString(OGRMultiLineString *ogrMultiLineString, std::shared_ptr<GraphicEntity> &gMultiLineString)
{
  size_t n = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
  std::shared_ptr<GMultiLineString> multiLineString = std::dynamic_pointer_cast<GMultiLineString>(gMultiLineString);
  multiLineString->resize(n);
  for (size_t i = 0; i < n; i++) {
    OGRLineString *ogrLineString = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
    size_t np = static_cast<size_t>(ogrLineString->getNumPoints());
    (*multiLineString)[i].resize(np);
    for (size_t j = 0; j < np; j++) {
      (*multiLineString)[i][j].x = ogrLineString->getX(static_cast<int>(j));
      (*multiLineString)[i][j].y = ogrLineString->getY(static_cast<int>(j));
    }
  }
}

void GdalVector::readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon, std::shared_ptr<GraphicEntity> &gMultiPolygon)
{
  size_t n = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
  std::shared_ptr<GMultiPolygon> multiPolygon = std::dynamic_pointer_cast<GMultiPolygon>(gMultiPolygon);
  multiPolygon->resize(n);
  for (size_t i = 0; i < n; i++) {
    OGRPolygon *ogrPolygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
    OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
    size_t np = static_cast<size_t>(ogrLinearRing->getNumPoints());
    (*multiPolygon)[i].resize(np);
    for (size_t j = 0; j < n; j++) {
      (*multiPolygon)[i][j].x = ogrLinearRing->getX(static_cast<int>(j));
      (*multiPolygon)[i][j].y = ogrLinearRing->getY(static_cast<int>(j));
    }
    int nir = ogrPolygon->getNumInteriorRings();

  }
}

//void GdalVector::readStyles(OGRStyleMgr *ogrStyle, std::shared_ptr<GraphicStyle> &gStyle)
void GdalVector::readStyles(OGRStyleMgr *ogrStyle, std::shared_ptr<GraphicEntity>  &gStyle)
{
  OGRStyleTool *ogrStyleTool = nullptr;
  for (int i = 0; i < ogrStyle->GetPartCount(); i++) {
    if ((ogrStyleTool = ogrStyle->GetPart(i)) != nullptr) {
      OGRSTClassId oci = ogrStyleTool->GetType();
      switch (oci) {
      case OGRSTCPen:
        readStylePen(dynamic_cast<OGRStylePen *>(ogrStyleTool), gStyle);
        break;
      case OGRSTCBrush:
        readStyleBrush(dynamic_cast<OGRStyleBrush *>(ogrStyleTool), gStyle);
        break;
      case OGRSTCSymbol:
        readStyleSymbol(dynamic_cast<OGRStyleSymbol *>(ogrStyleTool), gStyle);
        break;
      case OGRSTCLabel:
        readStyleLabel(dynamic_cast<OGRStyleLabel *>(ogrStyleTool), gStyle);
        break;
      case OGRSTCVector:

        break;
      default:
        break;
      }
    }
  }
}

void GdalVector::readStylePen(OGRStylePen *ogrStylePen, std::shared_ptr<GraphicEntity> &gStyle)
{
  GBool bDefault = false;
  std::shared_ptr<StylePen> stylePen = std::make_shared<StylePen>();

  /* Pen Color */
  const char *hexColor = ogrStylePen->Color(bDefault);
  if (!bDefault) {
    stylePen->setPenColor(tl::Color(hexColor));
  }

  /* Pen Cap */
  const char *cap = ogrStylePen->Cap(bDefault);
  if (!bDefault) {
    StylePen::PenCap penCap;
    if (strcmp(cap, "cap:r") == 0) {
      penCap = StylePen::PenCap::ROUND;
    } else if (strcmp(cap, "cap:p") == 0) {
      penCap = StylePen::PenCap::PROJECTING;
    } else {
      penCap = StylePen::PenCap::BUTT;
    }
    stylePen->setPenCap(penCap);
  }

  /* Pattern */
  const char *pattern = ogrStylePen->Pattern(bDefault);
  if (!bDefault) {
    stylePen->setPattern(pattern);
  }

  /* PenJoin */
  const char *join = ogrStylePen->Join(bDefault);
  if (bDefault) {
    StylePen::PenJoin penJoin;
    if (strcmp(cap, "j:r") == 0) {
      penJoin = StylePen::PenJoin::ROUNDED;
    } else if (strcmp(cap, "j:b") == 0) {
      penJoin = StylePen::PenJoin::BEVEL;
    } else {
      penJoin = StylePen::PenJoin::MITER;
    }
    stylePen->setPenJoin(penJoin);
  }

  /* Pen Name or ID */
  const char *name = ogrStylePen->Id(bDefault);
  if (!bDefault) {
    StylePen::PenName penName;
    if (strcmp(cap, "ogr-pen-1") == 0) {
      penName = StylePen::PenName::PEN_NULL;
    } else if (strcmp(cap, "ogr-pen-2") == 0) {
      penName = StylePen::PenName::DASH;
    } else if (strcmp(cap, "ogr-pen-3") == 0) {
      penName = StylePen::PenName::SHORT_DASH;
    } else if (strcmp(cap, "ogr-pen-4") == 0) {
      penName = StylePen::PenName::LONG_DASH;
    } else if (strcmp(cap, "ogr-pen-5") == 0) {
      penName = StylePen::PenName::DOT_LINE;
    } else if (strcmp(cap, "ogr-pen-6") == 0) {
      penName = StylePen::PenName::DASH_DOT_LINE;
    } else if (strcmp(cap, "ogr-pen-7") == 0) {
      penName = StylePen::PenName::DASH_DOT_DOT_LINE;
    } else if (strcmp(cap, "ogr-pen-8") == 0) {
      penName = StylePen::PenName::ALTERNATE_LINE;
    } else {
      penName = StylePen::PenName::SOLID;
    }
    stylePen->setPenName(penName);
  }

  /* Pen Width */
  double width = ogrStylePen->Width(bDefault);
  if (!bDefault) {
    double penWidth;
    OGRSTUnitId ud = ogrStylePen->GetUnit();
    switch (ud) {
    case OGRSTUGround:
      penWidth = 72.0*39.37*width;
      break;
    case OGRSTUMM:
      penWidth = 0.001*72.0*39.37*width;
      break;
    case OGRSTUCM:
      penWidth = 0.01*72.0*39.37*width;
      break;
    case OGRSTUInches:
      penWidth = 72.0*width;
      break;
    default:
      penWidth = width;
      break;
    }

    stylePen->setPenWidth(static_cast<uint8_t>(penWidth));
  }

  /* Pen Perpendicular Offset */
  double perpendicularOffset = ogrStylePen->PerpendicularOffset(bDefault);
  if (!bDefault) {
    ///TODO: El valor depende de las unidades tambi?n
    stylePen->setPerpendicularOffset(static_cast<int32_t>(perpendicularOffset));
  }

  /* Pen Priority Level */
  uint32_t priority = ogrStylePen->Priority(bDefault);
  if (!bDefault) {
    stylePen->setPriorityLevel(priority);
  }

  gStyle->setStylePen(stylePen);
}

void GdalVector::readStyleBrush(OGRStyleBrush *ogrStyleBrush, std::shared_ptr<GraphicEntity> &gStyle)
{
  GBool bDefault = false;
  std::shared_ptr<StyleBrush> styleBrush = std::make_shared<StyleBrush>();

  /* Angle */
  double angle = ogrStyleBrush->Angle(bDefault);
  if ( !bDefault ) {
    styleBrush->setAngle(angle); //TODO: ?Mejor como float en radianes??
  }

  /* Back Color */
  const char *hexColor = ogrStyleBrush->BackColor(bDefault);
  if (!bDefault) {
    styleBrush->setBackColor(tl::Color(hexColor));
  }

  /* Fore Color */
  hexColor = ogrStyleBrush->ForeColor(bDefault);
  if (!bDefault) {
    styleBrush->setForeColor(tl::Color(hexColor));
  }

  /* Brush Name */
  const char *brush_name = ogrStyleBrush->Id(bDefault);
  if (!bDefault) {
    StyleBrush::BrushName brushName;
    if (strcmp(brush_name, "ogr-pen-1") == 0) {
      brushName = StyleBrush::BrushName::BRUSH_NULL;
    } else if (strcmp(brush_name, "ogr-pen-2") == 0) {
      brushName = StyleBrush::BrushName::HORIZONTAL_HATCH;
    } else if (strcmp(brush_name, "ogr-pen-3") == 0) {
      brushName = StyleBrush::BrushName::VERTICAL_HATCH;
    } else if (strcmp(brush_name, "ogr-pen-4") == 0) {
      brushName = StyleBrush::BrushName::FDIAGONAL_HATCH;
    } else if (strcmp(brush_name, "ogr-pen-5") == 0) {
      brushName = StyleBrush::BrushName::BDIAGONAL_HATCH;
    } else if (strcmp(brush_name, "ogr-pen-6") == 0) {
      brushName = StyleBrush::BrushName::CROSS_HATCH;
    } else if (strcmp(brush_name, "ogr-pen-7") == 0) {
      brushName = StyleBrush::BrushName::DIAGCROSS_HATCH;
    } else {
      brushName = StyleBrush::BrushName::SOLID;
    }
    styleBrush->setBrushName(brushName);
  }

  /* Priority Level */
  uint32_t  priority = ogrStyleBrush->Priority(bDefault);
  if (!bDefault) {
    styleBrush->setPriorityLevel(priority);
  }

  /* Scaling Factor */
  double scalingFactor = ogrStyleBrush->Size(bDefault);
  if (!bDefault) {
    styleBrush->setScalingFactor(scalingFactor);
  }

  /* Spacing */
  ///TODO: spacingX y spacingY est?n asociados a un tipo de unidad
  double spacingX = ogrStyleBrush->SpacingX(bDefault);
  GBool bDefault2 = false;
  double spacingY = ogrStyleBrush->SpacingY(bDefault);
  if (!bDefault && !bDefault2) {
    styleBrush->setSpacing(spacingX, spacingY);
  }

  gStyle->setStyleBrush(styleBrush);
}

void GdalVector::readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol,std::shared_ptr<GraphicEntity> &gStyle)
{
  GBool bDefault = false;
  std::shared_ptr<StyleSymbol> styleSymbol = std::make_shared<StyleSymbol>();

  /* Angle */
  double angle = ogrStyleSymbol->Angle(bDefault);
  if (!bDefault) {
    styleSymbol->setAngle(angle);
  }

  /* Color */
  const char *hexColor = ogrStyleSymbol->Color(bDefault);
  if (!bDefault) {
    styleSymbol->setColor(tl::Color(hexColor));
  }

  /* Name */
  const char *name = ogrStyleSymbol->Id(bDefault);
  if (!bDefault) {
    StyleSymbol::SymbolName symbolName = StyleSymbol::SymbolName::CROSS;
    if (strcmp(name, "ogr-sym-0") == 0) {
      symbolName = StyleSymbol::SymbolName::CROSS;
    } else if (strcmp(name, "ogr-sym-1") == 0) {
      symbolName = StyleSymbol::SymbolName::DIAGONAL_CROSS;
    } else if (strcmp(name, "ogr-sym-2") == 0) {
      symbolName = StyleSymbol::SymbolName::CIRCLE;
    } else if (strcmp(name, "ogr-sym-3") == 0) {
      symbolName = StyleSymbol::SymbolName::CIRCLE_FILLED;
    } else if (strcmp(name, "ogr-sym-4") == 0) {
      symbolName = StyleSymbol::SymbolName::SQUARE;
    } else if (strcmp(name, "ogr-sym-5") == 0) {
      symbolName = StyleSymbol::SymbolName::SQUARE_FILLED;
    } else if (strcmp(name, "ogr-sym-6") == 0) {
      symbolName = StyleSymbol::SymbolName::TRIANGLE;
    } else if (strcmp(name, "ogr-sym-7") == 0) {
      symbolName = StyleSymbol::SymbolName::TRIANGLE_FILLED;
    } else if (strcmp(name, "ogr-sym-8") == 0) {
      symbolName = StyleSymbol::SymbolName::STAR;
    } else if (strcmp(name, "ogr-sym-9") == 0) {
      symbolName = StyleSymbol::SymbolName::STAR_FILLED;
    } else if (strcmp(name, "ogr-sym-10") == 0) {
      symbolName = StyleSymbol::SymbolName::VERTICAL_BAR;
    } else {
      ///TODO: Bitmap...
    }
    styleSymbol->setName(symbolName);
  }

  /* Offset */
  GBool bDefault1 = false;
  double offsetX = ogrStyleSymbol->SpacingX(bDefault);
  double offsetY = ogrStyleSymbol->SpacingY(bDefault1);
  if (!bDefault && !bDefault1) {
    styleSymbol->setOffset(offsetX, offsetY);
  }

  /* Outline Color */
  hexColor = ogrStyleSymbol->OColor(bDefault);
  if (!bDefault) {
    styleSymbol->setOutlineColor(tl::Color(hexColor));
  }

  /* Priority Level */
  uint32_t  priorityLevel = ogrStyleSymbol->Priority(bDefault);
  if (!bDefault) {
    styleSymbol->setPriorityLevel(priorityLevel);
  }

  /* Scaling Factor */
  double scalingFactor = ogrStyleSymbol->Size( bDefault);
  if (!bDefault) {
    styleSymbol->setScalingFactor(scalingFactor);
  }

  gStyle->setStyleSymbol(styleSymbol);
}

void GdalVector::readStyleLabel(OGRStyleLabel *ogrStyleLabel, std::shared_ptr<GraphicEntity> &gStyle)
{
  GBool bDefault = false;
  std::shared_ptr<StyleLabel> styleLabel = std::make_shared<StyleLabel>();

  /* Anchor Position */
  int anchor = ogrStyleLabel->Anchor(bDefault);
  if (!bDefault) {
    StyleLabel::AnchorPosition anchorPosition;
    if (anchor == 1) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_BASELINE | StyleLabel::AnchorPosition::HORIZONTAL_LEFT;
    } else if (anchor == 2) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_BASELINE | StyleLabel::AnchorPosition::HORIZONTAL_CENTER;
    } else if (anchor == 3) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_BASELINE | StyleLabel::AnchorPosition::HORIZONTAL_RIGHT;
    } else if (anchor == 4) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_CENTER | StyleLabel::AnchorPosition::HORIZONTAL_LEFT;
    } else if (anchor == 5) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_CENTER | StyleLabel::AnchorPosition::HORIZONTAL_CENTER;
    } else if (anchor == 6) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_CENTER | StyleLabel::AnchorPosition::HORIZONTAL_RIGHT;
    } else if (anchor == 7) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_TOP | StyleLabel::AnchorPosition::HORIZONTAL_LEFT;
    } else if (anchor == 8) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_TOP | StyleLabel::AnchorPosition::HORIZONTAL_CENTER;
    } else if (anchor == 9) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_TOP | StyleLabel::AnchorPosition::HORIZONTAL_RIGHT;
    } else if (anchor == 10) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_BOTTOM | StyleLabel::AnchorPosition::HORIZONTAL_LEFT;
    } else if (anchor ==11) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_BOTTOM | StyleLabel::AnchorPosition::HORIZONTAL_CENTER;
    } else if (anchor ==12) {
      anchorPosition = StyleLabel::AnchorPosition::VERTICAL_BOTTOM | StyleLabel::AnchorPosition::HORIZONTAL_RIGHT;
    }
    styleLabel->setAnchorPosition(anchorPosition);
  }

  /* Angle */
  double angle = ogrStyleLabel->Angle(bDefault);
  if (!bDefault) {
    styleLabel->setAngle(angle);
  }

  /* Background Color */
  const char *hexColor = ogrStyleLabel->BackColor(bDefault);
  if (!bDefault) {
    styleLabel->setBackgroundColor(tl::Color(hexColor));
  }

  /* Foreground Color */
  hexColor = ogrStyleLabel->ForeColor(bDefault);
  if (!bDefault) {
    styleLabel->setForegroundColor(tl::Color(hexColor));
  }

  /* Outline Color */
  hexColor = ogrStyleLabel->OutlineColor(bDefault);
  if (!bDefault) {
    styleLabel->setOutlineColor(tl::Color(hexColor));
  }

  /* Shadow Color */
  hexColor = ogrStyleLabel->ShadowColor(bDefault);
  if (!bDefault) {
    styleLabel->setShadowColor(tl::Color(hexColor));
  }

  /* Label Placement */
  const char *placement = ogrStyleLabel->Placement(bDefault);
  if (!bDefault) {
    StyleLabel::LabelPlacement labelPlacement;
    if (strcmp(placement, "m:l") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::l;
    } else if (strcmp(placement, "m:s") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::s;
    } else if (strcmp(placement, "m:m") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::m;
    } else if (strcmp(placement, "m:w") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::w;
    } else if (strcmp(placement, "m:h") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::h;
    } else if (strcmp(placement, "m:a") == 0) {
      labelPlacement = StyleLabel::LabelPlacement::a;
    } else {
      labelPlacement = StyleLabel::LabelPlacement::p;
    }
    styleLabel->setLabelPlacement(labelPlacement);
  }

  /* Offset */
  GBool bDefault1 = false;
  double offsetX = ogrStyleLabel->SpacingX(bDefault);
  double offsetY = ogrStyleLabel->SpacingY(bDefault1);
  if (!bDefault && !bDefault1) {
    styleLabel->setOffset(offsetX, offsetY);
  }

  /* Stretch */
  double stretch = ogrStyleLabel->Stretch(bDefault);
  if (!bDefault) {
    styleLabel->setStretch(stretch);
  }

  /* Font */
  Font font;

  std::string fontName = ogrStyleLabel->FontName(bDefault);
  if (!bDefault) {
    font.setName(fontName);
  }

  bool bold = ogrStyleLabel->Bold(bDefault) == 1;
  if (!bDefault) {
    font.setBold(bold);
  }

  bool italic = ogrStyleLabel->Italic(bDefault) == 1;
  if (!bDefault) {
    font.setItalic(italic);
  }

  bool underline = ogrStyleLabel->Underline(bDefault) == 1;
  if (!bDefault) {
    font.setUnderline(underline);
  }

  double size = ogrStyleLabel->Size(bDefault);
  if (!bDefault) {
  //  OGRSTUnitId ud = ogrStyleLabel->GetUnit();
  //  switch (ud) {
  //  //case OGRSTUGround:
  //  //  _width = width / m_dfScale;
  //  //  break;
  //  case OGRSTUPixel:
  //    _width = -size;
  //    break;
  //  case OGRSTUPoints:
  //    _width = -size;
  //    break;
  //  case OGRSTUMM:
  //    _width = 0.001 * size;
  //    break;
  //  case OGRSTUCM:
  //    _width = 0.01 * size;
  //    break;
  //  case OGRSTUInches:
  //    _width = 0.0254 * size;
  //    break;
  //  default:
  //  break;
  //}
  //  font.setSize(size);
  }

  styleLabel->setFont(font);

  bool strikeout = ogrStyleLabel->Strikeout(bDefault) == 1;

  gStyle->setStyleLabel(styleLabel);
}

GdalVector::Status GdalVector::writeLayer(OGRLayer *pLayer, const graph::GLayer &layer)
{
  Status err = Status::SUCCESS;
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pLayer->GetLayerDefn());
  for (auto &entity : layer) {
    GraphicEntity::Type type = entity->getType();
    switch (type) {
    case tl::graph::GraphicEntity::Type::POINT_2D:
      err = writePoint(ogrFeature, std::dynamic_pointer_cast<GPoint>(entity));
      break;
    case tl::graph::GraphicEntity::Type::POINT_3D:
      err = writePoint(ogrFeature, std::dynamic_pointer_cast<GPoint3D>(entity));
      break;
    case tl::graph::GraphicEntity::Type::LINESTRING_2D:
      err = writeLineString(ogrFeature, std::dynamic_pointer_cast<GLineString>(entity));
      break;
    case tl::graph::GraphicEntity::Type::LINESTRING_3D:
      err = writeLineString(ogrFeature, std::dynamic_pointer_cast<GLineString3D>(entity));
      break;
    case tl::graph::GraphicEntity::Type::POLYGON_2D:
      err = writePolygon(ogrFeature, std::dynamic_pointer_cast<GPolygon>(entity));
      break;
    case tl::graph::GraphicEntity::Type::POLYGON_3D:
      err = writePolygon(ogrFeature, std::dynamic_pointer_cast<GPolygon3D>(entity));
      break;
    case tl::graph::GraphicEntity::Type::SEGMENT_2D:
      break;
    case tl::graph::GraphicEntity::Type::SEGMENT_3D:
      break;
    case tl::graph::GraphicEntity::Type::WINDOW:
      break;
    case tl::graph::GraphicEntity::Type::BOX:
      break;
    case tl::graph::GraphicEntity::Type::MULTIPOINT_2D:
      err = writeMultiPoint(ogrFeature, std::dynamic_pointer_cast<GMultiPoint>(entity));
      break;
    case tl::graph::GraphicEntity::Type::MULTIPOINT_3D:
      err = writeMultiPoint(ogrFeature, std::dynamic_pointer_cast<GMultiPoint3D>(entity));
      break;
    case tl::graph::GraphicEntity::Type::MULTILINE_2D:
      err = writeMultiLineString(ogrFeature, std::dynamic_pointer_cast<GMultiLineString>(entity));
      break;
    case tl::graph::GraphicEntity::Type::MULTILINE_3D:
      err = writeMultiLineString(ogrFeature, std::dynamic_pointer_cast<GMultiLineString3D>(entity));
      break;
    case tl::graph::GraphicEntity::Type::MULTIPOLYGON_2D:
      err = writeMultiPolygon(ogrFeature, std::dynamic_pointer_cast<GMultiPolygon>(entity));
      break;
    case tl::graph::GraphicEntity::Type::MULTIPOLYGON_3D:
      err = writeMultiPolygon(ogrFeature, std::dynamic_pointer_cast<GMultiPolygon3D>(entity));
      break;
    case tl::graph::GraphicEntity::Type::CIRCLE:
      break;
    case tl::graph::GraphicEntity::Type::ELLIPSE:
      break;
    default:
      break;
    }
  }

  if (pLayer->CreateFeature(ogrFeature) != OGRERR_NONE) {
    err = Status::FAILURE;
  }

  OGRFeature::DestroyFeature(ogrFeature);
  return err;
}

GdalVector::Status GdalVector::writePoint(OGRFeature *ogrFeature, const std::shared_ptr<GPoint> &gPoint)
{
  OGRPoint ogrPoint;
  ogrPoint.setX(gPoint->x);
  ogrPoint.setY(gPoint->y);
  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrPoint))
    return Status::SUCCESS;
  else
    return Status::FAILURE;
}

GdalVector::Status GdalVector::writePoint(OGRFeature *ogrFeature, const std::shared_ptr<GPoint3D> &gPoint3D)
{
  OGRPoint ogrPoint;
  ogrPoint.setX(gPoint3D->x);
  ogrPoint.setY(gPoint3D->y);
  ogrPoint.setZ(gPoint3D->z);
  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrPoint))
    return Status::SUCCESS;
  else
    return Status::FAILURE;
}

GdalVector::Status GdalVector::writeLineString(OGRFeature *ogrFeature, const std::shared_ptr<GLineString> &gLineString)
{
  OGRLineString ogrLineString;
  for (int i = 0; i < gLineString->size(); i++) {
    OGRPoint pt((*gLineString)[i].x, (*gLineString)[i].y);
    ogrLineString.addPoint(&pt);
  }

  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrLineString))
    return Status::SUCCESS;
  else
    return Status::FAILURE;
}

GdalVector::Status GdalVector::writeLineString(OGRFeature *ogrFeature, const std::shared_ptr<GLineString3D> &gLineString3D)
{
  OGRLineString ogrLineString;
  for (int i = 0; i < gLineString3D->size(); i++) {
    OGRPoint pt((*gLineString3D)[i].x, (*gLineString3D)[i].y, (*gLineString3D)[i].z);
    ogrLineString.addPoint(&pt);
  }

  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrLineString))
    return Status::SUCCESS;
  else
    return Status::FAILURE;
}

GdalVector::Status GdalVector::writePolygon(OGRFeature *ogrFeature, const std::shared_ptr<GPolygon> &gPolygon)
{
  OGRPolygon ogrPolygon;
  OGRLinearRing ogrLinearRing;
  for (int i = 0; i < gPolygon->size(); i++) {
    OGRPoint pt((*gPolygon)[i].x, (*gPolygon)[i].y);
    ogrLinearRing.addPoint(&pt);
  }
  ogrPolygon.addRing(&ogrLinearRing);

  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrPolygon))
    return Status::SUCCESS;
  else
    return Status::FAILURE;
}

GdalVector::Status GdalVector::writePolygon(OGRFeature *ogrFeature, const std::shared_ptr<GPolygon3D> &gPolygon3D)
{
  OGRPolygon ogrPolygon;
  OGRLinearRing ogrLinearRing;
  for (int i = 0; i < gPolygon3D->size(); i++) {
    OGRPoint pt((*gPolygon3D)[i].x, (*gPolygon3D)[i].y, (*gPolygon3D)[i].z);
    ogrLinearRing.addPoint(&pt);
  }
  ogrPolygon.addRing(&ogrLinearRing);

  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrPolygon))
    return Status::FAILURE;
  else
    return Status::SUCCESS;
  return Status::FAILURE;
}

GdalVector::Status GdalVector::writeMultiPoint(OGRFeature *ogrFeature, const std::shared_ptr<GMultiPoint> &gMultiPoint)
{
  return Status::FAILURE;
}

GdalVector::Status GdalVector::writeMultiPoint(OGRFeature *ogrFeature, const std::shared_ptr<GMultiPoint3D> &gMultiPoint3D)
{
  return Status::FAILURE;
}

GdalVector::Status GdalVector::writeMultiLineString(OGRFeature *ogrFeature, const std::shared_ptr<GMultiLineString> &gMultiLineString)
{
  return Status::FAILURE;
}

GdalVector::Status GdalVector::writeMultiLineString(OGRFeature *ogrFeature, const std::shared_ptr<GMultiLineString3D> &gMultiLineString3D)
{
  return Status::FAILURE;
}

GdalVector::Status GdalVector::writeMultiPolygon(OGRFeature *ogrFeature, const std::shared_ptr<GMultiPolygon> &gMultiPolygon)
{
  return Status::FAILURE;
}

GdalVector::Status GdalVector::writeMultiPolygon(OGRFeature *ogrFeature, const std::shared_ptr<GMultiPolygon3D> &gMultiPolygon3D)
{
  return Status::FAILURE;
}

void GdalVector::update()
{
  //mLayersCount = pDataset->GetLayerCount();
}

#endif // HAVE_GDAL







VectorGraphics::VectorGraphics()
  : File(),
    mLayers(0)
{
}

VectorGraphics::~VectorGraphics()
{
}

void VectorGraphics::close()
{
  if (mVectorFormat) mVectorFormat->close();
}

VectorGraphics::Status VectorGraphics::open(const char *file, Mode mode, FileOptions *options)
{
  close();

  mFile = file;
//  char ext[TL_MAX_EXT];
//  if (getFileExtension(file, ext, TL_MAX_EXT) != 0) return Status::FAILURE;
  std::string ext = fs::extension(mFile);

  const char *frtName;

#ifdef HAVE_GDAL
  if ((frtName = GdalVector::getDriverFromExt(ext.c_str())) != nullptr) {
    // Existe un driver de GDAL para el formato vectorial
    mVectorFormat = std::make_unique<GdalVector>();
  } else {
    return Status::FAILURE;
  }
#endif

  if (mVectorFormat) {
    mVectorFormat->open(file, mode, options);
    update();
    return Status::OPEN_OK;
  } else return Status::OPEN_FAIL;
}

VectorGraphics::Status VectorGraphics::open(const std::string &file, Mode mode, FileOptions *options)
{
  return open(file.c_str(), mode, options);
}

VectorGraphics::Status VectorGraphics::create()
{
  if (mVectorFormat) {
    return mVectorFormat->create();
  } else {
    return Status::FAILURE;
  }
}

VectorGraphics::Status VectorGraphics::createCopy(const char *fileOut)
{
  return Status::FAILURE;
}

///TODO: Por ahora leo todo pero habria que poder filtrar por ventana.
VectorGraphics::Status VectorGraphics::read()
{
  if (mVectorFormat) {
    int n = mVectorFormat->getLayersCount();
    for (int il = 0; il < n; il++) {
      //std::shared_ptr<GLayer> gLayer = std::make_shared<GLayer>();
      std::shared_ptr<GLayer> gLayer;
      mVectorFormat->read(il, gLayer.get());
      //TODO: devolver error y comprobar
      mLayers.push_back(gLayer);
    }
    return Status::SUCCESS;
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

VectorGraphics::Status VectorGraphics::read(int layerId, graph::GLayer *layer)
{
  if (mVectorFormat) {
    mVectorFormat->read(layerId, layer);
    return Status::SUCCESS;
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

VectorGraphics::Status VectorGraphics::read(const char *layerName, graph::GLayer *layer)
{
  if (mVectorFormat) {
    mVectorFormat->read(layerName, layer);
    return Status::SUCCESS;
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

VectorGraphics::Status VectorGraphics::read(const std::string &layerName, graph::GLayer *layer)
{
  if (mVectorFormat) {
    mVectorFormat->read(layerName, layer);
    return Status::SUCCESS;
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

VectorGraphics::Status VectorGraphics::writeLayer(int id, const graph::GLayer &layer)
{
  if (mVectorFormat) {
    return mVectorFormat->writeLayer(id, layer);
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

VectorGraphics::Status VectorGraphics::writeLayer(const char *layerName, const graph::GLayer &layer)
{
  if (mVectorFormat) {
    return mVectorFormat->writeLayer(layerName, layer);
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

VectorGraphics::Status VectorGraphics::writeLayer(const std::string &layerName, const graph::GLayer &layer)
{
  if (mVectorFormat) {
    return mVectorFormat->writeLayer(layerName.c_str(), layer);
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

int VectorGraphics::getLayersCount() const
{
  if (mVectorFormat) {
    return mVectorFormat->getLayersCount();
  } else {
    msgError("No open document");
    return -1;
  }
}

VectorGraphics::Status VectorGraphics::createLayer(const char *layerName)
{
  if (mVectorFormat) {
    return mVectorFormat->createLayer(layerName);
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

VectorGraphics::Status VectorGraphics::createLayer(const std::string &layerName)
{
  if (mVectorFormat) {
    return mVectorFormat->createLayer(layerName);
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

VectorGraphics::Status VectorGraphics::createLayer(const graph::GLayer &layer)
{
  if (mVectorFormat) {
    return mVectorFormat->createLayer(layer);
  } else {
    return VectorGraphics::Status::FAILURE;
  }
}

void VectorGraphics::update()
{

}

//VectFile::~VectFile()
//{}
//
//VectFile::Status VectFile::open()
//{
//  poDataset = (GDALDataset *) GDALOpen( mFile.c_str(), GA_ReadOnly );
//  if (poDataset == NULL) return Status::OPEN_ERROR;
//  else Status::OPEN_OK;
//}

} // End namespace TL
