#include "vectwriter.h"

#include "tidop/core/utils.h"
#include "tidop/core/gdalreg.h"
#include "tidop/graphic/layer.h"
#include "tidop/graphic/entities/point.h"
#include "tidop/graphic/entities/linestring.h"
#include "tidop/graphic/entities/polygon.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogrsf_frmts.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

namespace tl
{

using namespace graph;

VectorWriter::VectorWriter(const std::string &fileName)
  : mFileName(fileName)
{
}


/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_GDAL

class VectorWriterGdal
  : public VectorWriter
{

public:

	VectorWriterGdal(const std::string &fileName)
    : VectorWriter(fileName),
      mDataset(nullptr),
      mDriver(nullptr),
      mSpatialReference()
  {
    RegisterGdal::init();
  }

  ~VectorWriterGdal() override
  {
    this->close();
  }

  void open() override;
  bool isOpen() override;
  void close() override;
  void create() override;
  void write(const GLayer &layer) override;

private:

  std::string driverFromExt(std::string &extension) const;
  OGRLayer *createLayer(const std::string &layerName);
  void writePoint(OGRFeature *ogrFeature, const GPoint *gPoint);
  void writePoint(OGRFeature *ogrFeature, const GPoint3D *gPoint3D);
  void writeLineString(OGRFeature *ogrFeature, const GLineString *gLineString);
  void writeLineString(OGRFeature *ogrFeature, const GLineString3D *gLineString3D);
  void writePolygon(OGRFeature *ogrFeature, const GPolygon *gPolygon);
  void writePolygon(OGRFeature *ogrFeature, const GPolygon3D *gPolygon3D);
  void writeMultiPoint(OGRFeature *ogrFeature, const GMultiPoint *gMultiPoint);
  void writeMultiPoint(OGRFeature *ogrFeature, const GMultiPoint3D *gMultiPoint3D);
  void writeMultiLineString(OGRFeature *ogrFeature, const GMultiLineString *gMultiLineString);
  void writeMultiLineString(OGRFeature *ogrFeature, const GMultiLineString3D *gMultiLineString3D);
  void writeMultiPolygon(OGRFeature *ogrFeature, const GMultiPolygon *gMultiPolygon);
  void writeMultiPolygon(OGRFeature *ogrFeature, const GMultiPolygon3D *gMultiPolygon3D);
  void writeStyles(OGRStyleMgr *ogrStyleMgr, const GraphicEntity *gStyle);

private:

  GDALDataset *mDataset;
  GDALDriver *mDriver;
  OGRSpatialReference mSpatialReference;
};

void VectorWriterGdal::open()
{
  this->close();

  std::string extension = fs::path(mFileName).extension().string();
  std::string driver_name = driverFromExt(extension);

  if (driver_name.empty()) throw std::runtime_error("Vector file open fail. Driver not found");

  mDriver = GetGDALDriverManager()->GetDriverByName(driver_name.c_str()); 

  if (mDriver == nullptr) throw std::runtime_error("Vector file open fail");

  mDataset = static_cast<GDALDataset *>(GDALOpenEx(mFileName.c_str(),
                                        GDAL_OF_VECTOR,
                                        nullptr,
                                        nullptr/*options*/,
                                        nullptr));
}
  
inline bool VectorWriterGdal::isOpen()
{
  return mDriver != nullptr;
}

inline void VectorWriterGdal::close()
{
  if (mDataset) {
    GDALClose(mDataset);
    mDataset = nullptr;
  }
}

void VectorWriterGdal::create()
{
  if (mDriver == nullptr) throw std::runtime_error("Driver not found"); 

  if (mDataset) {
    GDALClose(mDataset);
    mDataset = nullptr;
  }

  mDataset = mDriver->Create(mFileName.c_str(), 0, 0, 0, GDT_Unknown, nullptr);
  
  if (mDataset == nullptr) throw std::runtime_error("Creation of file failed"); 
}

void VectorWriterGdal::write(const GLayer &layer)
{
  OGRLayer *ogrLayer = mDataset->GetLayerByName(layer.name().c_str());
  if (!ogrLayer) {
    ogrLayer = this->createLayer(layer.name());
  }

  std::vector<std::shared_ptr<experimental::TableField>> fields = layer.tableFields();

  for (size_t i = 0; i < fields.size(); i++) {

    experimental::TableField::Type type = fields[i]->type();
    OGRFieldType ogr_type;
    switch (type) {
      case tl::experimental::TableField::Type::INT:
        ogr_type = OFTInteger;
        break;
      case tl::experimental::TableField::Type::INT64:
        ogr_type = OFTInteger64;
        break;
      case tl::experimental::TableField::Type::DOUBLE:
        ogr_type = OFTReal;
        break;
      case tl::experimental::TableField::Type::STRING:
        ogr_type = OFTString;
        break;
      default:
        ogr_type = OFTString;
        break;
    }

    OGRFieldDefn fieldDefinition(fields[i]->name().c_str(), ogr_type);
    fieldDefinition.SetWidth(fields[i]->size());
    OGRErr error = ogrLayer->CreateField(&fieldDefinition);
    TL_ASSERT(error == OGRERR_NONE, "Creating field failed");

  }

  OGRStyleTable oStyleTable;
  OGRStyleMgr *ogrStyleMgr = new OGRStyleMgr(&oStyleTable);

  OGRFeature *ogrFeature; 

  for (auto &entity : layer) {

    ogrFeature = OGRFeature::CreateFeature(ogrLayer->GetLayerDefn());

    std::shared_ptr<experimental::TableRegister> data = entity->data();
    for (size_t i = 0; i < data->size(); i++) {
      TL_TODO("En función del tipo de dato. Por ahora sólo cadenas")
      ogrFeature->SetField(i, data->value(i).c_str());
    }

    GraphicEntity::Type type = entity->type();
    switch (type) {
    case GraphicEntity::Type::point_2d:
      this->writePoint(ogrFeature, dynamic_cast<GPoint *>(entity.get()));
      break;
    case GraphicEntity::Type::point_3d:
      this->writePoint(ogrFeature, dynamic_cast<GPoint3D *>(entity.get()));
      break;
    case GraphicEntity::Type::linestring_2d:
      this->writeLineString(ogrFeature, dynamic_cast<GLineString *>(entity.get()));
      break;
    case GraphicEntity::Type::linestring_3d:
      this->writeLineString(ogrFeature, dynamic_cast<GLineString3D *>(entity.get()));
      break;
    case GraphicEntity::Type::polygon_2d:
      this->writePolygon(ogrFeature, dynamic_cast<GPolygon *>(entity.get()));
      break;
    case GraphicEntity::Type::polygon_3d:
      this->writePolygon(ogrFeature, dynamic_cast<GPolygon3D *>(entity.get()));
      break;
    case GraphicEntity::Type::segment_2d:
      break;
    case GraphicEntity::Type::segment_3d:
      break;
    case GraphicEntity::Type::window:
      break;
    case GraphicEntity::Type::box:
      break;
    case GraphicEntity::Type::multipoint_2d:
      this->writeMultiPoint(ogrFeature, dynamic_cast<GMultiPoint *>(entity.get()));
      break;
    case GraphicEntity::Type::multipoint_3d:
      this->writeMultiPoint(ogrFeature, dynamic_cast<GMultiPoint3D *>(entity.get()));
      break;
    case GraphicEntity::Type::multiline_2d:
      this->writeMultiLineString(ogrFeature, dynamic_cast<GMultiLineString *>(entity.get()));
      break;
    case GraphicEntity::Type::multiline_3d:
      this->writeMultiLineString(ogrFeature, dynamic_cast<GMultiLineString3D *>(entity.get()));
      break;
    case GraphicEntity::Type::multipolygon_2d:
      this->writeMultiPolygon(ogrFeature, dynamic_cast<GMultiPolygon *>(entity.get()));
      break;
    case GraphicEntity::Type::multipolygon_3d:
      this->writeMultiPolygon(ogrFeature, dynamic_cast<GMultiPolygon3D *>(entity.get()));
      break;
    case GraphicEntity::Type::circle:
      break;
    case GraphicEntity::Type::ellipse:
      break;
    default:
      break;
    }

    this->writeStyles(ogrStyleMgr, entity.get());

    if (ogrLayer->CreateFeature(ogrFeature) != OGRERR_NONE) throw std::runtime_error("Create Feature Error");

  }


  if (ogrStyleMgr) {
    delete ogrStyleMgr;
    ogrStyleMgr = nullptr;
  }

  OGRFeature::DestroyFeature(ogrFeature);

}

void VectorWriterGdal::writeStyles(OGRStyleMgr *ogrStyleMgr, 
                                   const GraphicEntity *gStyle)
{
  OGRStyleTool *ogrStyleTool = nullptr;

  TL_TODO("Escribir los estilos. Hay que establecer un flag para ver si el estilo está activo");

  delete ogrStyleTool;
}

std::string VectorWriterGdal::driverFromExt(std::string &extension) const
{
  std::string format;
  if (boost::iequals(extension, ".dxf" ))  
    format = "DXF";
  else if (boost::iequals(extension, ".dwg" ))  
    format = "DWG";
  else if (boost::iequals(extension, ".dgn" ))  
    format = "DGN";
  else if (boost::iequals(extension, ".shp" ))  
    format = "ESRI Shapefile";
  else if (boost::iequals(extension, ".gml" ))  
    format = "GML";
  else if (boost::iequals(extension, ".kml" ))  
    format = "LIBKML";
  else if (boost::iequals(extension, ".kmz" ))  
    format = "LIBKML";
  else if (boost::iequals(extension, ".json"))  
    format = "GeoJSON";
  else if (boost::iequals(extension, ".osm" ))  
    format = "OSM";
  else format = "";
  return format;
}

OGRLayer *VectorWriterGdal::createLayer(const std::string &layerName)
{
  OGRLayer *layer = nullptr;
    
  char **encoding = nullptr;
  encoding = CSLAddString(encoding, "ENCODING=UTF-8");
  const char *driver_name = mDataset->GetDriverName();
  if (strcmp(driver_name, "DXF") == 0 || strcmp(driver_name, "DGN") == 0) {
    if (mDataset->GetLayerCount() == 0) {
      /// S?lo soportan la creaci?n de una capa con lo cual se a?adir? siempre a la capa "0"
      layer = mDataset->CreateLayer("0", 
                                    &mSpatialReference /*poOutputSRS*/, 
                                    static_cast<OGRwkbGeometryType>(wkbUnknown), 
                                    encoding);
    }
  } else if (strcmp(driver_name, "SHP") == 0) {
    layer = mDataset->CreateLayer(layerName.c_str(), 
                                  &mSpatialReference, 
                                  static_cast<OGRwkbGeometryType>(wkbUnknown), 
                                  encoding);
  } else {
    layer = mDataset->CreateLayer(layerName.c_str(), 
                                  &mSpatialReference, 
                                  static_cast<OGRwkbGeometryType>(wkbUnknown),
                                  encoding);
  }
  return layer;
}

void VectorWriterGdal::writePoint(OGRFeature *ogrFeature, 
                                  const GPoint *gPoint)
{
  OGRPoint ogrPoint;
  ogrPoint.setX(gPoint->x);
  ogrPoint.setY(gPoint->y);

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrPoint))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writePoint(OGRFeature *ogrFeature, 
                                  const GPoint3D *gPoint3D)
{
  OGRPoint ogrPoint;
  ogrPoint.setX(gPoint3D->x);
  ogrPoint.setY(gPoint3D->y);
  ogrPoint.setZ(gPoint3D->z);

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrPoint))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writeLineString(OGRFeature *ogrFeature, 
                                       const GLineString *gLineString)
{
  OGRLineString ogrLineString;
  for (size_t i = 0; i < gLineString->size(); i++) {
    OGRPoint pt((*gLineString)[i].x, (*gLineString)[i].y);
    ogrLineString.addPoint(&pt);
  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrLineString))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writeLineString(OGRFeature *ogrFeature, 
                                       const GLineString3D *gLineString3D)
{
  OGRLineString ogrLineString;
  for (size_t i = 0; i < gLineString3D->size(); i++) {
    OGRPoint pt((*gLineString3D)[i].x, (*gLineString3D)[i].y, (*gLineString3D)[i].z);
    ogrLineString.addPoint(&pt);
  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrLineString))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writePolygon(OGRFeature *ogrFeature, 
                                    const GPolygon *gPolygon)
{
  OGRPolygon ogrPolygon;
  OGRLinearRing ogrLinearRing;
  for (size_t i = 0; i < gPolygon->size(); i++) {
    OGRPoint pt((*gPolygon)[i].x, (*gPolygon)[i].y);
    ogrLinearRing.addPoint(&pt);
  }
  if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());

  for (size_t i = 0; i < gPolygon->holes(); i++) {
    PolygonHole<PointD> hole = gPolygon->hole(i);
    OGRLinearRing ogrLinearRing;
    for (size_t j = 0; j < hole.size(); j++) {
      OGRPoint pt(hole[j].x, hole[j].y);
      ogrLinearRing.addPoint(&pt);
    }
    if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                               CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrPolygon))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writePolygon(OGRFeature *ogrFeature, 
                                    const GPolygon3D *gPolygon3D)
{
  OGRPolygon ogrPolygon;
  OGRLinearRing ogrLinearRing;
  for (size_t i = 0; i < gPolygon3D->size(); i++) {
    OGRPoint pt((*gPolygon3D)[i].x, (*gPolygon3D)[i].y, (*gPolygon3D)[i].z);
    ogrLinearRing.addPoint(&pt);
  }
  if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());

  for (size_t i = 0; i < gPolygon3D->holes(); i++) {
    Polygon3DHole<Point3D> hole = gPolygon3D->hole(i);
    OGRLinearRing ogrLinearRing;
    for (size_t j = 0; j < hole.size(); j++) {
      OGRPoint pt(hole[j].x, hole[j].y, hole[j].z);
      ogrLinearRing.addPoint(&pt);
    }
    if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                               CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrPolygon))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writeMultiPoint(OGRFeature *ogrFeature, 
                                       const GMultiPoint *gMultiPoint)
{
  OGRMultiPoint ogrMultiPoint;

  for (auto &point : *gMultiPoint) {
    OGRPoint ogrPoint;
    ogrPoint.setX(point.x);
    ogrPoint.setY(point.y);
    if (OGRERR_NONE != ogrMultiPoint.addGeometry(&ogrPoint))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                               CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiPoint))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writeMultiPoint(OGRFeature *ogrFeature, 
                                       const GMultiPoint3D *gMultiPoint3D)
{
  OGRMultiPoint ogrMultiPoint;

  for (auto &point : *gMultiPoint3D) {
    OGRPoint ogrPoint;
    ogrPoint.setX(point.x);
    ogrPoint.setY(point.y);
    ogrPoint.setZ(point.z);
    if (OGRERR_NONE != ogrMultiPoint.addGeometry(&ogrPoint))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                               CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());

  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiPoint))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writeMultiLineString(OGRFeature *ogrFeature, 
                                            const GMultiLineString *gMultiLineString)
{
  OGRMultiLineString ogrMultiLineString;
  for (auto &lineString : *gMultiLineString) {
    OGRLineString ogrLineString;
    for (size_t i = 0; i < lineString.size(); i++) {
      OGRPoint pt(lineString[i].x, lineString[i].y);
      ogrLineString.addPoint(&pt);
    }
    if (OGRERR_NONE != ogrMultiLineString.addGeometry(&ogrLineString))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                               CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiLineString))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writeMultiLineString(OGRFeature *ogrFeature, 
                                            const GMultiLineString3D *gMultiLineString3D)
{
  OGRMultiLineString ogrMultiLineString;
  for (auto &lineString : *gMultiLineString3D) {
    OGRLineString ogrLineString;
    for (size_t i = 0; i < lineString.size(); i++) {
      OGRPoint pt(lineString[i].x, lineString[i].y, lineString[i].z);
      ogrLineString.addPoint(&pt);
    }
    if (OGRERR_NONE != ogrMultiLineString.addGeometry(&ogrLineString))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                               CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiLineString))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writeMultiPolygon(OGRFeature *ogrFeature, 
                                         const GMultiPolygon *gMultiPolygon)
{
  OGRMultiPolygon ogrMultiPolygon;
  for (auto &polygon : *gMultiPolygon) {

    OGRPolygon ogrPolygon;
    OGRLinearRing ogrLinearRing;

    for (size_t i = 0; i < polygon.size(); i++) {
      OGRPoint pt(polygon[i].x, polygon[i].y);
      ogrLinearRing.addPoint(&pt);
    }
    if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
   
    for (size_t i = 0; i < polygon.holes(); i++) {
      PolygonHole<PointD> hole = polygon.hole(i);
      OGRLinearRing ogrLinearRing;
      for (size_t j = 0; j < hole.size(); j++) {
        OGRPoint pt(hole[j].x, hole[j].y);
        ogrLinearRing.addPoint(&pt);
      }
      if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
        throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s",
                                 CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    }

    if (OGRERR_NONE != ogrMultiPolygon.addGeometry(&ogrPolygon))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                               CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());

  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiPolygon))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

void VectorWriterGdal::writeMultiPolygon(OGRFeature *ogrFeature, 
                                         const GMultiPolygon3D *gMultiPolygon3D)
{
  OGRMultiPolygon ogrMultiPolygon;
  for (auto &polygon : *gMultiPolygon3D) {

    OGRPolygon ogrPolygon;
    OGRLinearRing ogrLinearRing;

    for (size_t i = 0; i < polygon.size(); i++) {
      OGRPoint pt(polygon[i].x, polygon[i].y, polygon[i].z);
      ogrLinearRing.addPoint(&pt);
    }
    if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
   
    for (size_t i = 0; i < polygon.holes(); i++) {
      Polygon3DHole<Point3D> hole = polygon.hole(i);
      OGRLinearRing ogrLinearRing;
      for (size_t j = 0; j < hole.size(); j++) {
        OGRPoint pt(hole[j].x, hole[j].y, hole[j].z);
        ogrLinearRing.addPoint(&pt);
      }
      if (OGRERR_NONE != ogrPolygon.addRing(&ogrLinearRing))
        throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s",
                                 CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
    }

    if (OGRERR_NONE != ogrMultiPolygon.addGeometry(&ogrPolygon))
      throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                               CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());

  }

  if (OGRERR_NONE != ogrFeature->SetGeometry(&ogrMultiPolygon))
    throw std::runtime_error(MessageManager::Message("GDAL ERROR (%i): %s", 
                             CPLGetLastErrorNo(), CPLGetLastErrorMsg()).message());
}

#endif

/* ---------------------------------------------------------------------------------- */


std::unique_ptr<VectorWriter> VectorWriterFactory::createWriter(const std::string &fileName)
{
  std::string extension = fs::path(fileName).extension().string();
  std::unique_ptr<VectorWriter> vector_writer;
#ifdef HAVE_GDAL
  if (boost::iequals(extension, ".dxf") ||
        boost::iequals(extension, ".dwg") ||
        boost::iequals(extension, ".dgn") ||
        boost::iequals(extension, ".shp") ||
        boost::iequals(extension, ".gml") ||
        boost::iequals(extension, ".kml") ||
        boost::iequals(extension, ".kmz") ||
        boost::iequals(extension, ".json") ||
        boost::iequals(extension, ".osm")) {
    vector_writer = std::make_unique<VectorWriterGdal>(fileName);
  } else
#endif
  {
    throw std::runtime_error("Invalid Vector Writer");
  }
  return vector_writer;
}

} // End namespace tl
