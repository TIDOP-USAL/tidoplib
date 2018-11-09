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
  : File()
{}

#ifdef HAVE_GDAL
GdalVector::GdalVector()
  : VrtVector(),
    pDataset(nullptr),
    pDriver(nullptr),
    mSpatialReference(),
    mDriverName("")
{
  RegisterGdal::init();
}

GdalVector::~GdalVector()
{
  if (pDataset) {
    GDALClose(pDataset);
    pDataset = nullptr;
  }
}

void GdalVector::close()
{
  if (pDataset) {
    GDALClose(pDataset);
    pDataset = nullptr;
  }
  mFile = "";
}

GdalVector::Status GdalVector::open(const std::string &file, Mode mode, FileOptions *options)
{
  //TODO: Se puede sacar a una clase de mas bajo nivel esta secci?n ...
  close();

  mFile = file;
  std::string ext = fs::extension(mFile);
  mDriverName = driverFromExt(ext.c_str());
  if (mDriverName == nullptr) return Status::open_fail;

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

  if (mode == Mode::create) {

    pDriver = GetGDALDriverManager()->GetDriverByName(mDriverName);
    if (pDriver == nullptr) return Status::open_fail;

    // Se crea el directorio si no existe
    char dir[TL_MAX_PATH];
    if ( getFileDriveDir(file.c_str(), dir, TL_MAX_PATH) == 0 )
      if ( createDir(dir) == -1) return Status::open_fail;
    return Status::open_ok;
  } else {

    pDataset = (GDALDataset*)GDALOpenEx( file.c_str(), GDAL_OF_VECTOR, nullptr, nullptr/*options->getOptions()*/, nullptr ); //GDALOpen( file, gdal_access);
    if (pDataset == nullptr) {
      return Status::open_fail;
    } else {
      pDriver = pDataset->GetDriver();
      update();
      return Status::open_ok;
    }

  }
}

///TODO: ver opciones de creaci?n: 2D, 3D, ...
GdalVector::Status  GdalVector::create()
{
  if (pDriver == nullptr) {
    msgError("Utilice el modo Create para abrir el archivo");
    return Status::failure;
  }

  if (pDataset) GDALClose(pDataset), pDataset = nullptr;
  pDataset = pDriver->Create(mFile.c_str(), 0, 0, 0, GDT_Unknown, nullptr);

  if (pDataset == nullptr) {
    msgError("Creation of output file failed.");
    return Status::failure;
  }

  update();

  return Status::success;
}

int GdalVector::layersCount() const
{
  return pDataset->GetLayerCount();
}

void GdalVector::read(int id, GLayer *layer)
{
  read(pDataset->GetLayer(id), layer);
}

void GdalVector::read(const std::string &name, GLayer *layer)
{
  read(pDataset->GetLayerByName(name.c_str()), layer);
}

GdalVector::Status GdalVector::createCopy(const std::string &fileOut)
{
  return Status::failure;
}

GdalVector::Status GdalVector::createLayer(const std::string &layerName)
{
  char **encoding = nullptr;
  encoding = CSLAddString(encoding,"ENCODING=UTF-8");

  OGRLayer *layer = nullptr;
  if (strcmp(mDriverName, "DXF") == 0 || strcmp(mDriverName, "DGN") == 0) {
    if (pDataset->GetLayerCount() == 0) {
      /// S?lo soportan la creaci?n de una capa con lo cual se a?adir? siempre a la capa "0"
      layer = pDataset->CreateLayer("0", &mSpatialReference /*poOutputSRS*/, static_cast<OGRwkbGeometryType>(wkbUnknown), encoding);
    }
  } else if (strcmp(mDriverName, "SHP") == 0) {
    layer = pDataset->CreateLayer(layerName.c_str(), &mSpatialReference, static_cast<OGRwkbGeometryType>(wkbUnknown), encoding);
  } else {
    layer = pDataset->CreateLayer(layerName.c_str(), &mSpatialReference, static_cast<OGRwkbGeometryType>(wkbUnknown), encoding);
  }

  if (layer == nullptr) {
    msgError("Layer creation failed.");
    return Status::failure;
  } else {

    return Status::success;
  }
}

GdalVector::Status GdalVector::createLayer(const graph::GLayer &layer)
{
  return createLayer(layer.name());
}

const char *GdalVector::driverFromExt(const char *ext)
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

GdalVector::Status GdalVector::writeLayer(int id, const graph::GLayer &layer)
{
  return writeLayer(pDataset->GetLayer(id), layer);
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

      OGRStyleMgr *ogrStyleMgr = nullptr;
      try {
        std::shared_ptr<GraphicEntity> entity = readEntity(pGeometry);
        ogrStyleMgr = new OGRStyleMgr();
        ogrStyleMgr->GetStyleString(ogrFeature);
        //readStyles(ogrStyleMgr, std::dynamic_pointer_cast<GraphicStyle>(entity));
        readStyles(ogrStyleMgr, entity);
        //readData()
        layer->push_back(entity);
        // comprobación de que se estuviese guardando
        //std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>((*layer->begin()));
        //polygon->size();
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

std::shared_ptr<GraphicEntity> GdalVector::readEntity(OGRGeometry *ogrGeometry)
{
  std::shared_ptr<GraphicEntity> gEntity;
  OGRwkbGeometryType type;
  if ( wkbHasZ(ogrGeometry->getGeometryType()) )
    type = wkbFlatten(ogrGeometry->getGeometryType());
  else
    type = wkbSetZ(ogrGeometry->getGeometryType());

//  OGRwkbGeometryType::wkbLineString25D;
//  OGRwkbGeometryType::wkbLineStringM;
//  OGRwkbGeometryType::wkbLineStringZM;
  int dim = ogrGeometry->getCoordinateDimension();
  switch ( type ) {
  case wkbUnknown:
    break;
  case wkbPoint:
    gEntity = readPoint(static_cast<OGRPoint *>(ogrGeometry));
    break;
  case wkbLineString:
    gEntity = readLineString(static_cast<OGRLineString *>(ogrGeometry));
    break;
  case wkbPolygon:
    gEntity = readPolygon(static_cast<OGRPolygon *>(ogrGeometry));
    break;
  case wkbMultiPoint:
    gEntity = readMultiPoint(static_cast<OGRMultiPoint *>(ogrGeometry));
    break;
  case wkbMultiLineString:
    gEntity = readMultiLineString(static_cast<OGRMultiLineString *>(ogrGeometry));
    break;
  case wkbMultiPolygon:
    gEntity = readMultiPolygon(static_cast<OGRMultiPolygon *>(ogrGeometry));
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
    if (dim == 2)
      gEntity = readPoint(static_cast<OGRPoint *>(ogrGeometry));
    else
      gEntity = readPoint3D(static_cast<OGRPoint *>(ogrGeometry));
    break;
  case wkbLineString25D:
    if (dim == 2)
      gEntity = readLineString(static_cast<OGRLineString *>(ogrGeometry));
    else
      gEntity = readLineString3D(static_cast<OGRLineString *>(ogrGeometry));
    break;
  case wkbPolygon25D:
    //gEntity = std::make_shared<GPolygon3D>();
    if (dim == 2)
      gEntity = readPolygon(static_cast<OGRPolygon *>(ogrGeometry));
    else
      gEntity = readPolygon3D(static_cast<OGRPolygon *>(ogrGeometry));
    break;
  case wkbMultiPoint25D:
    if (dim == 2)
      gEntity = readMultiPoint(static_cast<OGRMultiPoint *>(ogrGeometry));
    else
      gEntity = readMultiPoint3D(static_cast<OGRMultiPoint *>(ogrGeometry));
    break;
  case wkbMultiLineString25D:
    if (dim == 0)
      gEntity = readMultiLineString(static_cast<OGRMultiLineString *>(ogrGeometry));
    else
      gEntity = readMultiLineString3D(static_cast<OGRMultiLineString *>(ogrGeometry));
    break;
  case wkbMultiPolygon25D:
    if (dim == 0)
      gEntity = readMultiPolygon(static_cast<OGRMultiPolygon *>(ogrGeometry));
    else
      gEntity = readMultiPolygon3D(static_cast<OGRMultiPolygon *>(ogrGeometry));
    break;
  case wkbGeometryCollection25D:
    break;
  default:
    break;
  }

  return gEntity;
}

std::shared_ptr<GPoint> GdalVector::readPoint(OGRPoint *ogrPoint)
{
  std::shared_ptr<GPoint> gPoint(new GPoint(ogrPoint->getX(), ogrPoint->getY()));
  return gPoint;
}

std::shared_ptr<GPoint3D> GdalVector::readPoint3D(OGRPoint *ogrPoint)
{
  std::shared_ptr<GPoint3D> gPoint(new GPoint3D(ogrPoint->getX(), ogrPoint->getY(), ogrPoint->getZ()));
  return gPoint;
}

std::shared_ptr<GLineString> GdalVector::readLineString(OGRLineString *ogrLineString)
{
  int n = ogrLineString->getNumPoints();
  std::shared_ptr<GLineString> lineString(new GLineString);
  lineString->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    //gLineString->operator[](i) = Point<float>(ogrLineString->getX(i), ogrLineString->getY(i));
    (*lineString)[static_cast<size_t>(i)].x = ogrLineString->getX(i);
    (*lineString)[static_cast<size_t>(i)].y = ogrLineString->getY(i);
  }
  return lineString;
}

std::shared_ptr<GLineString3D> GdalVector::readLineString3D(OGRLineString *ogrLineString)
{
  int n = ogrLineString->getNumPoints();
  std::shared_ptr<GLineString3D> lineString(new GLineString3D);
  lineString->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    (*lineString)[static_cast<size_t>(i)].x = ogrLineString->getX(i);
    (*lineString)[static_cast<size_t>(i)].y = ogrLineString->getY(i);
    (*lineString)[static_cast<size_t>(i)].z = ogrLineString->getZ(i);
  }
  return lineString;
}

std::shared_ptr<GPolygon> GdalVector::readPolygon(OGRPolygon *ogrPolygon)
{
  // Contorno exterior
  OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
  int n = ogrLinearRing->getNumPoints();
  std::shared_ptr<GPolygon> gPolygon(new GPolygon);
  gPolygon->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    ///TODO: Es mas rapida la asignaci?n con movimiento
    (*gPolygon)[static_cast<size_t>(i)] = PointD(ogrLinearRing->getX(i), ogrLinearRing->getY(i));
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
  return gPolygon;
}

std::shared_ptr<GPolygon3D> GdalVector::readPolygon3D(OGRPolygon *ogrPolygon)
{
  // Contorno exterior
  OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
  int n = ogrLinearRing->getNumPoints();
  std::shared_ptr<GPolygon3D> gPolygon(new GPolygon3D);
  gPolygon->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    ///TODO: Es mas rapida la asignaci?n con movimiento
    (*gPolygon)[i] = Point3D(ogrLinearRing->getX(i), ogrLinearRing->getY(i), ogrLinearRing->getZ(i));
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
  return gPolygon;
}

std::shared_ptr<GMultiPoint> GdalVector::readMultiPoint(OGRMultiPoint *ogrMultiPoint)
{
  int n = ogrMultiPoint->getNumGeometries();
  std::shared_ptr<GMultiPoint> multiPoint(new GMultiPoint);
  multiPoint->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    OGRPoint *ogrPoint = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(i));
    (*multiPoint)[static_cast<size_t>(i)].x = ogrPoint->getX();
    (*multiPoint)[static_cast<size_t>(i)].y = ogrPoint->getY();
  }
  return multiPoint;
}

std::shared_ptr<GMultiPoint3D> GdalVector::readMultiPoint3D(OGRMultiPoint *ogrMultiPoint)
{
  int n = ogrMultiPoint->getNumGeometries();
  std::shared_ptr<GMultiPoint3D> multiPoint(new GMultiPoint3D);
  multiPoint->resize(static_cast<size_t>(n));
  for (int i = 0; i < n; i++) {
    OGRPoint *ogrPoint = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(i));
    (*multiPoint)[static_cast<size_t>(i)].x = ogrPoint->getX();
    (*multiPoint)[static_cast<size_t>(i)].y = ogrPoint->getY();
    (*multiPoint)[static_cast<size_t>(i)].z = ogrPoint->getZ();
  }
  return multiPoint;
}

std::shared_ptr<graph::GMultiLineString> GdalVector::readMultiLineString(OGRMultiLineString *ogrMultiLineString)
{
  size_t n = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
  std::shared_ptr<GMultiLineString> multiLineString(new GMultiLineString);
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
  return multiLineString;
}

std::shared_ptr<graph::GMultiLineString3D> GdalVector::readMultiLineString3D(OGRMultiLineString *ogrMultiLineString)
{
  size_t n = static_cast<size_t>(ogrMultiLineString->getNumGeometries());
  std::shared_ptr<GMultiLineString3D> multiLineString(new GMultiLineString3D);
  multiLineString->resize(n);
  for (size_t i = 0; i < n; i++) {
    OGRLineString *ogrLineString = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(static_cast<int>(i)));
    size_t np = static_cast<size_t>(ogrLineString->getNumPoints());
    (*multiLineString)[i].resize(np);
    for (size_t j = 0; j < np; j++) {
      (*multiLineString)[i][j].x = ogrLineString->getX(static_cast<int>(j));
      (*multiLineString)[i][j].y = ogrLineString->getY(static_cast<int>(j));
      (*multiLineString)[i][j].z = ogrLineString->getZ(static_cast<int>(j));
    }
  }
  return multiLineString;
}

std::shared_ptr<graph::GMultiPolygon> GdalVector::readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon)
{
  size_t n = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
  std::shared_ptr<GMultiPolygon> multiPolygon( new GMultiPolygon);
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
  return multiPolygon;
}

std::shared_ptr<graph::GMultiPolygon3D> GdalVector::readMultiPolygon3D(OGRMultiPolygon *ogrMultiPolygon)
{
  size_t n = static_cast<size_t>(ogrMultiPolygon->getNumGeometries());
  std::shared_ptr<GMultiPolygon3D> multiPolygon( new GMultiPolygon3D);
  multiPolygon->resize(n);
  for (size_t i = 0; i < n; i++) {
    OGRPolygon *ogrPolygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(static_cast<int>(i)));
    OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
    size_t np = static_cast<size_t>(ogrLinearRing->getNumPoints());
    (*multiPolygon)[i].resize(np);
    for (size_t j = 0; j < n; j++) {
      (*multiPolygon)[i][j].x = ogrLinearRing->getX(static_cast<int>(j));
      (*multiPolygon)[i][j].y = ogrLinearRing->getY(static_cast<int>(j));
      (*multiPolygon)[i][j].z = ogrLinearRing->getZ(static_cast<int>(j));
    }
    int nir = ogrPolygon->getNumInteriorRings();

  }
  return multiPolygon;
}

void GdalVector::readStyles(OGRStyleMgr *ogrStyle, std::shared_ptr<GraphicEntity>  &gStyle)
{
  OGRStyleTool *ogrStyleTool = nullptr;
  for (int i = 0; i < ogrStyle->GetPartCount(); i++) {
    if ((ogrStyleTool = ogrStyle->GetPart(i)) != nullptr) {
      OGRSTClassId oci = ogrStyleTool->GetType();
      switch (oci) {
      case OGRSTCPen:
        gStyle->setStylePen(readStylePen(dynamic_cast<OGRStylePen *>(ogrStyleTool)));
        break;
      case OGRSTCBrush:
        gStyle->setStyleBrush(readStyleBrush(dynamic_cast<OGRStyleBrush *>(ogrStyleTool)));
        break;
      case OGRSTCSymbol:
        gStyle->setStyleSymbol(readStyleSymbol(dynamic_cast<OGRStyleSymbol *>(ogrStyleTool)));
        break;
      case OGRSTCLabel:
        gStyle->setStyleLabel(readStyleLabel(dynamic_cast<OGRStyleLabel *>(ogrStyleTool)));
        break;
      case OGRSTCVector:

        break;
      default:
        break;
      }
    }
  }
}

std::shared_ptr<StylePen> GdalVector::readStylePen(OGRStylePen *ogrStylePen)
{

  GBool bDefault = false;
  std::shared_ptr<StylePen> stylePen = std::make_shared<StylePen>();

  try{

    /* Pen Color */
    const char *hexColor = ogrStylePen->Color(bDefault);
    if (!bDefault) {
      stylePen->setColor(tl::graph::Color(hexColor+1));
    }

    /* Pen Cap */
    const char *cap = ogrStylePen->Cap(bDefault);
    if (!bDefault) {
      StylePen::Cap penCap;
      if (strcmp(cap, "cap:r") == 0) {
        penCap = StylePen::Cap::round;
      } else if (strcmp(cap, "cap:p") == 0) {
        penCap = StylePen::Cap::projective;
      } else {
        penCap = StylePen::Cap::butt;
      }
      stylePen->setCap(penCap);
    }

    /* Pattern */
    const char *pattern = ogrStylePen->Pattern(bDefault);
    if (!bDefault) {
      stylePen->setPattern(pattern);
    }

    /* PenJoin */
    const char *join = ogrStylePen->Join(bDefault);
    if (!bDefault) {
      StylePen::Join penJoin;
      if (strcmp(join, "j:r") == 0) {
        penJoin = StylePen::Join::rounded;
      } else if (strcmp(join, "j:b") == 0) {
        penJoin = StylePen::Join::bevel;
      } else {
        penJoin = StylePen::Join::miter;
      }
      stylePen->setJoin(penJoin);
    }

    /* Pen Name or ID */
    const char *name = ogrStylePen->Id(bDefault);
    if (!bDefault) {
      StylePen::Name penName;
      if (strcmp(name, "ogr-pen-1") == 0) {
        penName = StylePen::Name::null;
      } else if (strcmp(name, "ogr-pen-2") == 0) {
        penName = StylePen::Name::dash;
      } else if (strcmp(name, "ogr-pen-3") == 0) {
        penName = StylePen::Name::short_dash;
      } else if (strcmp(name, "ogr-pen-4") == 0) {
        penName = StylePen::Name::long_dash;
      } else if (strcmp(name, "ogr-pen-5") == 0) {
        penName = StylePen::Name::dot_line;
      } else if (strcmp(name, "ogr-pen-6") == 0) {
        penName = StylePen::Name::dash_dot_line;
      } else if (strcmp(name, "ogr-pen-7") == 0) {
        penName = StylePen::Name::dash_dot_dot_line;
      } else if (strcmp(name, "ogr-pen-8") == 0) {
        penName = StylePen::Name::alternate_line;
      } else {
        penName = StylePen::Name::solid;
      }
      stylePen->setName(penName);
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
      stylePen->setWidth(static_cast<uint8_t>(penWidth));
    }

    /* Pen Perpendicular Offset */
    double perpendicularOffset = ogrStylePen->PerpendicularOffset(bDefault);
    if (!bDefault) {
      ///TODO: El valor depende de las unidades tambi?n
      stylePen->setPerpendicularOffset(static_cast<int32_t>(perpendicularOffset));
    }

    /* Pen Priority Level */
    uint32_t priority = static_cast<uint32_t>(ogrStylePen->Priority(bDefault));
    if (!bDefault) {
      stylePen->setPriorityLevel(priority);
    }

  } catch (std::exception &e){
    msgError(e.what());
  }

  return stylePen;
}

std::shared_ptr<StyleBrush> GdalVector::readStyleBrush(OGRStyleBrush *ogrStyleBrush)
{
  GBool bDefault = false;
  std::shared_ptr<StyleBrush> styleBrush = std::make_shared<StyleBrush>();

  try {
    /* Angle */
    double angle = ogrStyleBrush->Angle(bDefault);
    if ( !bDefault ) {
      styleBrush->setAngle(angle); //TODO: ?Mejor como float en radianes??
    }

    /* Back Color */
    const char *hexColor = ogrStyleBrush->BackColor(bDefault);
    if (!bDefault) {
      styleBrush->setBackColor(tl::graph::Color(hexColor+1));
    }

    /* Fore Color */
    hexColor = ogrStyleBrush->ForeColor(bDefault);
    if (!bDefault) {
      styleBrush->setForeColor(tl::graph::Color(hexColor+1));
    }

    /* Brush Name */
    const char *brush_name = ogrStyleBrush->Id(bDefault);
    if (!bDefault) {
      StyleBrush::Name brushName;
      if (strcmp(brush_name, "ogr-pen-1") == 0) {
        brushName = StyleBrush::Name::null;
      } else if (strcmp(brush_name, "ogr-pen-2") == 0) {
        brushName = StyleBrush::Name::horizontal_hatch;
      } else if (strcmp(brush_name, "ogr-pen-3") == 0) {
        brushName = StyleBrush::Name::vertical_hatch;
      } else if (strcmp(brush_name, "ogr-pen-4") == 0) {
        brushName = StyleBrush::Name::fdiagonal_hatch;
      } else if (strcmp(brush_name, "ogr-pen-5") == 0) {
        brushName = StyleBrush::Name::bdiagonal_hatch;
      } else if (strcmp(brush_name, "ogr-pen-6") == 0) {
        brushName = StyleBrush::Name::cross_hatch;
      } else if (strcmp(brush_name, "ogr-pen-7") == 0) {
        brushName = StyleBrush::Name::diagcross_hatch;
      } else {
        brushName = StyleBrush::Name::solid;
      }
      styleBrush->setBrushName(brushName);
    }

    /* Priority Level */
    uint32_t  priority = static_cast<uint32_t>(ogrStyleBrush->Priority(bDefault));
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

  } catch (std::exception &e){
    msgError(e.what());
  }

  return styleBrush;
}

std::shared_ptr<StyleSymbol> GdalVector::readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol)
{
  GBool bDefault = false;
  std::shared_ptr<StyleSymbol> styleSymbol = std::make_shared<StyleSymbol>();

  try {

    /* Angle */
    double angle = ogrStyleSymbol->Angle(bDefault);
    if (!bDefault) {
      styleSymbol->setAngle(angle);
    }

    /* Color */
    const char *hexColor = ogrStyleSymbol->Color(bDefault);
    if (!bDefault) {
      styleSymbol->setColor(tl::graph::Color(hexColor+1));
    }

    /* Name */
    const char *name = ogrStyleSymbol->Id(bDefault);
    if (!bDefault) {
      StyleSymbol::Name symbolName = StyleSymbol::Name::cross;
      if (strcmp(name, "ogr-sym-0") == 0) {
        symbolName = StyleSymbol::Name::cross;
      } else if (strcmp(name, "ogr-sym-1") == 0) {
        symbolName = StyleSymbol::Name::diagonal_cross;
      } else if (strcmp(name, "ogr-sym-2") == 0) {
        symbolName = StyleSymbol::Name::circle;
      } else if (strcmp(name, "ogr-sym-3") == 0) {
        symbolName = StyleSymbol::Name::circle_filled;
      } else if (strcmp(name, "ogr-sym-4") == 0) {
        symbolName = StyleSymbol::Name::square;
      } else if (strcmp(name, "ogr-sym-5") == 0) {
        symbolName = StyleSymbol::Name::square_filled;
      } else if (strcmp(name, "ogr-sym-6") == 0) {
        symbolName = StyleSymbol::Name::triangle;
      } else if (strcmp(name, "ogr-sym-7") == 0) {
        symbolName = StyleSymbol::Name::triangle_filled;
      } else if (strcmp(name, "ogr-sym-8") == 0) {
        symbolName = StyleSymbol::Name::star;
      } else if (strcmp(name, "ogr-sym-9") == 0) {
        symbolName = StyleSymbol::Name::star_filled;
      } else if (strcmp(name, "ogr-sym-10") == 0) {
        symbolName = StyleSymbol::Name::vertical_bar;
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
      styleSymbol->setOutlineColor(tl::graph::Color(hexColor+1));
    }

    /* Priority Level */
    uint32_t  priorityLevel = static_cast<uint32_t>(ogrStyleSymbol->Priority(bDefault));
    if (!bDefault) {
      styleSymbol->setPriorityLevel(priorityLevel);
    }

    /* Scaling Factor */
    double scalingFactor = ogrStyleSymbol->Size( bDefault);
    if (!bDefault) {
      styleSymbol->setScalingFactor(scalingFactor);
    }
  } catch (std::exception &e) {
    msgError(e.what());
  }

  return styleSymbol;
}

std::shared_ptr<StyleLabel> GdalVector::readStyleLabel(OGRStyleLabel *ogrStyleLabel)
{
  GBool bDefault = false;
  std::shared_ptr<StyleLabel> styleLabel = std::make_shared<StyleLabel>();

  try {

    /* Anchor Position */
    int anchor = ogrStyleLabel->Anchor(bDefault);
    if (!bDefault) {
      StyleLabel::AnchorPosition anchorPosition;
      if (anchor == 1) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_baseline |
                         StyleLabel::AnchorPosition::horizontal_left;
      } else if (anchor == 2) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_baseline |
                         StyleLabel::AnchorPosition::horizontal_center;
      } else if (anchor == 3) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_baseline |
                         StyleLabel::AnchorPosition::horizontal_right;
      } else if (anchor == 4) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_center |
                         StyleLabel::AnchorPosition::horizontal_left;
      } else if (anchor == 5) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_center |
                         StyleLabel::AnchorPosition::horizontal_center;
      } else if (anchor == 6) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_center |
                         StyleLabel::AnchorPosition::horizontal_right;
      } else if (anchor == 7) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_top |
                         StyleLabel::AnchorPosition::horizontal_left;
      } else if (anchor == 8) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_top |
                         StyleLabel::AnchorPosition::horizontal_center;
      } else if (anchor == 9) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_top |
                         StyleLabel::AnchorPosition::horizontal_right;
      } else if (anchor == 10) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_bottom |
                         StyleLabel::AnchorPosition::horizontal_left;
      } else if (anchor ==11) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_bottom |
                         StyleLabel::AnchorPosition::horizontal_center;
      } else if (anchor ==12) {
        anchorPosition = StyleLabel::AnchorPosition::vertical_bottom |
                         StyleLabel::AnchorPosition::horizontal_right;
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
      styleLabel->setBackgroundColor(tl::graph::Color(hexColor+1));
    }

    /* Foreground Color */
    hexColor = ogrStyleLabel->ForeColor(bDefault);
    if (!bDefault) {
      styleLabel->setForegroundColor(tl::graph::Color(hexColor+1));
    }

    /* Outline Color */
    hexColor = ogrStyleLabel->OutlineColor(bDefault);
    if (!bDefault) {
      styleLabel->setOutlineColor(tl::graph::Color(hexColor+1));
    }

    /* Shadow Color */
    hexColor = ogrStyleLabel->ShadowColor(bDefault);
    if (!bDefault) {
      styleLabel->setShadowColor(tl::graph::Color(hexColor+1));
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

    const char *fontName = ogrStyleLabel->FontName(bDefault);
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
//      OGRSTUnitId ud = ogrStyleLabel->GetUnit();
//      switch (ud) {
//      //case OGRSTUGround:
//      //  _width = width / m_dfScale;
//      //  break;
//      case OGRSTUPixel:
//        size = -size;
//        break;
//      case OGRSTUPoints:
//        size = -size;
//        break;
//      case OGRSTUMM:
//        size = 0.001 * size;
//        break;
//      case OGRSTUCM:
//        size = 0.01 * size;
//        break;
//      case OGRSTUInches:
//        size = 0.0254 * size;
//        break;
//      default:
//      break;
//      }
      font.setSize(static_cast<int>(size));
    }

    bool strikeout = ogrStyleLabel->Strikeout(bDefault);
    if (!bDefault) {
      font.setStrikethrough(strikeout);
    }

    styleLabel->setFont(font);


  } catch (std::exception &e) {
    msgError(e.what());
  }

  return styleLabel;
}

GdalVector::Status GdalVector::writeLayer(OGRLayer *pLayer, const graph::GLayer &layer)
{
  Status err = Status::success;
  OGRFeature *ogrFeature = OGRFeature::CreateFeature(pLayer->GetLayerDefn());
  for (auto &entity : layer) {
    GraphicEntity::Type type = entity->type();
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
    err = Status::failure;
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
    return Status::success;
  else
    return Status::failure;
}

GdalVector::Status GdalVector::writePoint(OGRFeature *ogrFeature, const std::shared_ptr<GPoint3D> &gPoint3D)
{
  OGRPoint ogrPoint;
  ogrPoint.setX(gPoint3D->x);
  ogrPoint.setY(gPoint3D->y);
  ogrPoint.setZ(gPoint3D->z);
  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrPoint))
    return Status::success;
  else
    return Status::failure;
}

GdalVector::Status GdalVector::writeLineString(OGRFeature *ogrFeature, const std::shared_ptr<GLineString> &gLineString)
{
  OGRLineString ogrLineString;
  for (size_t i = 0; i < gLineString->size(); i++) {
    OGRPoint pt((*gLineString)[i].x, (*gLineString)[i].y);
    ogrLineString.addPoint(&pt);
  }

  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrLineString))
    return Status::success;
  else
    return Status::failure;
}

GdalVector::Status GdalVector::writeLineString(OGRFeature *ogrFeature, const std::shared_ptr<GLineString3D> &gLineString3D)
{
  OGRLineString ogrLineString;
  for (size_t i = 0; i < gLineString3D->size(); i++) {
    OGRPoint pt((*gLineString3D)[i].x, (*gLineString3D)[i].y, (*gLineString3D)[i].z);
    ogrLineString.addPoint(&pt);
  }

  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrLineString))
    return Status::success;
  else
    return Status::failure;
}

GdalVector::Status GdalVector::writePolygon(OGRFeature *ogrFeature, const std::shared_ptr<GPolygon> &gPolygon)
{
  OGRPolygon ogrPolygon;
  OGRLinearRing ogrLinearRing;
  for (size_t i = 0; i < gPolygon->size(); i++) {
    OGRPoint pt((*gPolygon)[i].x, (*gPolygon)[i].y);
    ogrLinearRing.addPoint(&pt);
  }
  ogrPolygon.addRing(&ogrLinearRing);

  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrPolygon))
    return Status::success;
  else
    return Status::failure;
}

GdalVector::Status GdalVector::writePolygon(OGRFeature *ogrFeature, const std::shared_ptr<GPolygon3D> &gPolygon3D)
{
  OGRPolygon ogrPolygon;
  OGRLinearRing ogrLinearRing;
  for (size_t i = 0; i < gPolygon3D->size(); i++) {
    OGRPoint pt((*gPolygon3D)[i].x, (*gPolygon3D)[i].y, (*gPolygon3D)[i].z);
    ogrLinearRing.addPoint(&pt);
  }
  ogrPolygon.addRing(&ogrLinearRing);

  if (OGRERR_NONE == ogrFeature->SetGeometry(&ogrPolygon))
    return Status::failure;
  else
    return Status::success;
  return Status::failure;
}

GdalVector::Status GdalVector::writeMultiPoint(OGRFeature *ogrFeature, const std::shared_ptr<GMultiPoint> &gMultiPoint)
{
  return Status::failure;
}

GdalVector::Status GdalVector::writeMultiPoint(OGRFeature *ogrFeature, const std::shared_ptr<GMultiPoint3D> &gMultiPoint3D)
{
  return Status::failure;
}

GdalVector::Status GdalVector::writeMultiLineString(OGRFeature *ogrFeature, const std::shared_ptr<GMultiLineString> &gMultiLineString)
{
  return Status::failure;
}

GdalVector::Status GdalVector::writeMultiLineString(OGRFeature *ogrFeature, const std::shared_ptr<GMultiLineString3D> &gMultiLineString3D)
{
  return Status::failure;
}

GdalVector::Status GdalVector::writeMultiPolygon(OGRFeature *ogrFeature, const std::shared_ptr<GMultiPolygon> &gMultiPolygon)
{
  return Status::failure;
}

GdalVector::Status GdalVector::writeMultiPolygon(OGRFeature *ogrFeature, const std::shared_ptr<GMultiPolygon3D> &gMultiPolygon3D)
{
  return Status::failure;
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

VectorGraphics::Status VectorGraphics::open(const std::string &file, Mode mode, FileOptions *options)
{
  close();

  mFile = file;

  std::string ext = fs::extension(mFile);

  const char *frtName;

#ifdef HAVE_GDAL
  if ((frtName = GdalVector::driverFromExt(ext.c_str())) != nullptr) {
    // Existe un driver de GDAL para el formato vectorial
    mVectorFormat = std::make_unique<GdalVector>();
  } else {
    return Status::failure;
  }
#endif

  if (mVectorFormat) {
    mVectorFormat->open(file, mode, options);
    update();
    return Status::open_ok;
  } else return Status::open_fail;
}

VectorGraphics::Status VectorGraphics::create()
{
  if (mVectorFormat) {
    return mVectorFormat->create();
  } else {
    return Status::failure;
  }
}

VectorGraphics::Status VectorGraphics::createCopy(const std::string &fileOut)
{
  return Status::failure;
}

//VectorGraphics::Status VectorGraphics::read()
//{
//  if (mVectorFormat) {
//    int n = mVectorFormat->layersCount();
//    for (int il = 0; il < n; il++) {
//      //std::shared_ptr<GLayer> gLayer = std::make_shared<GLayer>();
//      std::shared_ptr<GLayer> gLayer;
//      mVectorFormat->read(il, gLayer.get());
//      //TODO: devolver error y comprobar
//      mLayers.push_back(gLayer);
//    }
//    return Status::SUCCESS;
//  } else {
//    return VectorGraphics::Status::FAILURE;
//  }
//}

VectorGraphics::Status VectorGraphics::read(int layerId, graph::GLayer *layer)
{
  if (mVectorFormat) {
    mVectorFormat->read(layerId, layer);
    return Status::success;
  } else {
    return VectorGraphics::Status::failure;
  }
}

VectorGraphics::Status VectorGraphics::read(const std::string &layerName, graph::GLayer *layer)
{
  if (mVectorFormat) {
    mVectorFormat->read(layerName, layer);
    return Status::success;
  } else {
    return VectorGraphics::Status::failure;
  }
}

VectorGraphics::Status VectorGraphics::writeLayer(int id, const graph::GLayer &layer)
{
  if (mVectorFormat) {
    return mVectorFormat->writeLayer(id, layer);
  } else {
    return VectorGraphics::Status::failure;
  }
}

VectorGraphics::Status VectorGraphics::writeLayer(const std::string &layerName, const graph::GLayer &layer)
{
  if (mVectorFormat) {
    return mVectorFormat->writeLayer(layerName, layer);
  } else {
    return VectorGraphics::Status::failure;
  }
}

int VectorGraphics::layersCount() const
{
  if (mVectorFormat) {
    return mVectorFormat->layersCount();
  } else {
    msgError("No open document");
    return -1;
  }
}

VectorGraphics::Status VectorGraphics::createLayer(const std::string &layerName)
{
  if (mVectorFormat) {
    return mVectorFormat->createLayer(layerName);
  } else {
    return VectorGraphics::Status::failure;
  }
}

VectorGraphics::Status VectorGraphics::createLayer(const graph::GLayer &layer)
{
  if (mVectorFormat) {
    return mVectorFormat->createLayer(layer);
  } else {
    return VectorGraphics::Status::failure;
  }
}

void VectorGraphics::update()
{

}

} // End namespace TL
