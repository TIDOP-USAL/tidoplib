#include "vect/vectio.h"

#include "core/utils.h"
#include "graphic_entities/graphic_entity.h"

#ifdef HAVE_GDAL
I3D_SUPPRESS_WARNINGS
#include "ogrsf_frmts.h"
I3D_DEFAULT_WARNINGS
#endif // HAVE_GDAL

namespace I3D
{
  
int VrtVector::getLayersCount() const
{
  return mLayersCount;
}

#ifdef HAVE_GDAL
GdalVector::GdalVector() : VrtVector(), pDataset(0), pDriver(0), mDriverName("")
{

}

void GdalVector::close() 
{
  if (pDataset) GDALClose(pDataset), pDataset = NULL;
  mName = "";
  mLayersCount = 0;
}

int GdalVector::open(const char *file, Mode mode)
{
  //TODO: Se puede sacar a una clase de mas bajo nivel esta sección ...
  close();

  mName = file;
  char ext[I3D_MAX_EXT];
  if (getFileExtension(file, ext, I3D_MAX_EXT) != 0) return 1;

  mDriverName = getDriverFromExt(ext);
  if (mDriverName == NULL) return 1;

  // No parece que se necesite
  //GDALAccess gdal_access;
  //switch (mode) {
  //case I3D::Mode::Read:
  //  gdal_access = GA_ReadOnly;
  //  break;
  //case I3D::Mode::Update:
  //  gdal_access = GA_Update;
  //  break;
  //case I3D::Mode::Create:
  //  gdal_access = GA_Update;
  //  break;
  //default:
  //  gdal_access = GA_ReadOnly;
  //  break;
  //}
  //....

  if (mode == Mode::Create) {

    return 1;
  } else {
    pDataset = (GDALDataset*)GDALOpenEx( file, GDAL_OF_VECTOR, NULL, NULL, NULL ); //GDALOpen( file, gdal_access);
    if (pDataset == NULL) {
      return 1;
    } else {
      pDriver = pDataset->GetDriver();
      update();
      return 0; 
    }
  }

//  if (poDataset == NULL) return Status::OPEN_ERROR;
//  else Status::OPEN_OK;
}

int GdalVector::create() 
{
  return 1;
}

void GdalVector::read(VectorGraphics *vector) 
{
  OGRLayer  *pLayer;
}

void GdalVector::read(int id, GLayer *layer) 
{
  read(pDataset->GetLayer(id), layer);
}

void GdalVector::read(const char *name, GLayer *layer) 
{
  read(pDataset->GetLayerByName(name), layer);
}

void GdalVector::read(OGRLayer *pLayer, GLayer *layer)
{
  OGRFeature *pFeature;
  while ( (pFeature = pLayer->GetNextFeature()) != NULL ) {

    const char *layerName = NULL;
    if (strcmp(mDriverName , "DXF") == 0) {
      layerName = pFeature->GetFieldAsString(0);
    } else if ( strcmp(mDriverName , "DGN" ) == 0 ) {
      layerName = pFeature->GetFieldAsString(1);
    } else {
      layerName = pLayer->GetName();
    }
    layer->setName(layerName);
    if (OGRGeometry *pGeometry = pFeature->GetGeometryRef()) {
      std::shared_ptr<GraphicEntity> entity;
      OGRStyleMgr *ogrStyleMgr;
      try {
        readEntity(pGeometry, entity);
        ogrStyleMgr = new OGRStyleMgr();
        ogrStyleMgr->GetStyleString(pFeature);
        readStyles(ogrStyleMgr, entity.get());
        //readData()
        layer->add(entity);

      } catch (std::exception &e) {
        msgError(e.what());
      }
      if (ogrStyleMgr) delete ogrStyleMgr, ogrStyleMgr = 0;

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
          //  // Salida a fichero log con errores.  GDAL en OpenCv lo envía a cout
          //}
        }
    OGRFeature::DestroyFeature( pFeature );
  }
}

void GdalVector::readEntity(OGRGeometry *ogrGeometry, std::shared_ptr<GraphicEntity> gEntity)
{
  OGRwkbGeometryType type;
  if ( wkbHasZ(ogrGeometry->getGeometryType()) )
    type = wkbFlatten(ogrGeometry->getGeometryType());
  else
    type = wkbSetZ(ogrGeometry->getGeometryType());
  switch ( type ) {
  case wkbPoint:
    gEntity = std::make_shared<GPoint>();
    readPoint(dynamic_cast<OGRPoint *>(ogrGeometry), dynamic_cast<GPoint *>(gEntity.get()));
    break;
  case wkbLineString:
    gEntity = std::make_shared<GLineString>();
    readLineString(dynamic_cast<OGRLineString *>(ogrGeometry), dynamic_cast<GLineString *>(gEntity.get()));
    break;
  case wkbPolygon:
    gEntity = std::make_shared<GPolygon>();
    readPolygon(dynamic_cast<OGRPolygon *>(ogrGeometry), dynamic_cast<GPolygon *>(gEntity.get()));
    break;
  case wkbMultiPoint:
    gEntity = std::make_shared<GMultiPoint>();
    readMultiPoint(dynamic_cast<OGRMultiPoint *>(ogrGeometry), dynamic_cast<GMultiPoint *>(gEntity.get()));
    break;
  case wkbMultiLineString:
    gEntity = std::make_shared<GMultiLineString>();
    readMultiLineString(dynamic_cast<OGRMultiLineString *>(ogrGeometry), dynamic_cast<GMultiLineString *>(gEntity.get()));
    break;
  case wkbMultiPolygon:
    gEntity = std::make_shared<GMultiPolygon>();
    readMultiPolygon(dynamic_cast<OGRMultiPolygon *>(ogrGeometry), dynamic_cast<GMultiPolygon *>(gEntity.get()));
    break;
  default:
    break;
  }
}

void GdalVector::readPoint(OGRPoint *ogrPoint, GPoint *gPoint)
{
  gPoint->x = ogrPoint->getX();
  gPoint->y = ogrPoint->getY();
}

void GdalVector::readLineString(OGRLineString *ogrLineString, GLineString *gLineString)
{
  int n = ogrLineString->getNumPoints();
  gLineString->resize(n);
  for (int i = 0; i < n; i++) {
    //gLineString->add(PointD(ogrLineString->getX(i), ogrLineString->getY(i)));
    gLineString->operator[](i) = PointD(ogrLineString->getX(i), ogrLineString->getY(i));
  }
}

void GdalVector::readPolygon(OGRPolygon *ogrPolygon, GPolygon *gPolygon)
{
  // Contorno exterior
  OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
  int n = ogrLinearRing->getNumPoints();
  for (int i = 0; i < n; i++) {
    gPolygon->add(PointD(ogrLinearRing->getX(i), ogrLinearRing->getY(i)));
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

void GdalVector::readMultiPoint(OGRMultiPoint *ogrMultiPoint, GMultiPoint *gMultiPoint)
{
  int n = ogrMultiPoint->getNumGeometries();
  gMultiPoint->resize(n);
  for (int i = 0; i < n; i++) {
    OGRPoint *ogrPoint = (OGRPoint *)ogrMultiPoint->getGeometryRef(i);
    //gMultiPoint[i] = PointD(ogrPoint->getX(), ogrPoint->getY());
    gMultiPoint->operator[](i) = PointD(ogrPoint->getX(), ogrPoint->getY());
  }
}

void GdalVector::readMultiLineString(OGRMultiLineString *ogrMultiLineString, GMultiLineString *gMultiLineString)
{
  int n = ogrMultiLineString->getNumGeometries();
  //gMultiLineString->
  //for (int i = 0; i < n; i++) {
  //  readLineString(dynamic_cast<OGRLineString *>(omls->getGeometryRef(i)), spl->PDats(ipl) );
  //}
}

void GdalVector::readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon, GMultiPolygon *gMultiPolygon)
{

}

void GdalVector::readStyles(OGRStyleMgr *ogrStyle, GraphicStyle *gStyle)
{
  OGRStyleTool *ogrStyleTool = NULL;
  for (int i = 0; i < ogrStyle->GetPartCount(); i++) {
    if (ogrStyleTool = ogrStyle->GetPart(i)) {
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

void GdalVector::readStylePen(OGRStylePen *ogrStylePen, GraphicStyle *gStyle)
{
  GBool bDefault = false;
  std::shared_ptr<StylePen> stylePen = std::make_shared<StylePen>();
  const char *hexColor = ogrStylePen->Color(bDefault);
  if (!bDefault) stylePen->setPenColor(Color(hexColor));

  gStyle->setStylePen(stylePen);
}

void GdalVector::readStyleBrush(OGRStyleBrush *ogrStyleBrush, GraphicStyle *gStyle)
{
  std::shared_ptr<StyleBrush> styleBrush = std::make_shared<StyleBrush>();
  gStyle->setStyleBrush(styleBrush);
}

void GdalVector::readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol, GraphicStyle *gStyle)
{
  std::shared_ptr<StyleSymbol> styleSymbol = std::make_shared<StyleSymbol>();
  gStyle->setStyleSymbol(styleSymbol);
}

void GdalVector::readStyleLabel(OGRStyleLabel *ogrStyleLabel, GraphicStyle *gStyle)
{
  std::shared_ptr<StyleLabel> styleLabel = std::make_shared<StyleLabel>();
  gStyle->setStyleLabel(styleLabel);
}


int GdalVector::write(VectorGraphics *vector) 
{
  return 1;
}

const char *GdalVector::getDriverFromExt(const char *ext)
{
  char *format;
  if      ( strcmpi( ext, ".dxf" ) == 0 )  format = "DXF";
  else if ( strcmpi( ext, ".dwg" ) == 0 )  format = "DWG";
  else if ( strcmpi( ext, ".dgn" ) == 0 )  format = "DGN";
  else if ( strcmpi( ext, ".shp" ) == 0 )  format = "ESRI Shapefile";
  else if ( strcmpi( ext, ".gml" ) == 0 )  format = "GML";
  else if ( strcmpi( ext, ".kml" ) == 0 )  format = "LIBKML";
  else if ( strcmpi( ext, ".kmz" ) == 0 )  format = "LIBKML";
  else if ( strcmpi( ext, ".json") == 0 )  format = "GeoJSON";
  else if ( strcmpi( ext, ".osm" ) == 0 )  format = "OSM";
  else                                     format = 0;
  return( format );
}

void GdalVector::update() 
{
  mLayersCount = pDataset->GetLayerCount();
}

#endif // HAVE_GDAL







VectorGraphics::VectorGraphics() : 
  mName(""), 
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

Status VectorGraphics::open(const char *file, Mode mode)
{
  close();

  mName = file;
  char ext[I3D_MAX_EXT];
  if (getFileExtension(file, ext, I3D_MAX_EXT) != 0) return Status::OPEN_FAIL;

  const char *frtName;

#ifdef HAVE_GDAL
  if ((frtName = GdalVector::getDriverFromExt(ext)) != NULL) { 
    // Existe un driver de GDAL para el formato vectorial
    mVectorFormat = std::make_unique<GdalVector>();
  } else {
    return Status::OPEN_FAIL;
  }
#endif

  if (mVectorFormat) {
    mVectorFormat->open(file, mode);
    update();
    return Status::OPEN_OK;
  } else return Status::OPEN_FAIL;
}

Status VectorGraphics::create()
{
  return Status::FAILURE;
}

Status VectorGraphics::createCopy()
{
  return Status::FAILURE;
}

Status VectorGraphics::read()
{
  int n = mVectorFormat->getLayersCount();
  for ( int il = 0; il < n; il++ ) {
    std::shared_ptr<GLayer> gLayer = std::make_shared<GLayer>();
    mVectorFormat->read(il, gLayer.get());
    //TODO: devolver error y comprobar
    mLayers.push_back(gLayer);
  }
  return Status::FAILURE;
}

Status VectorGraphics::write()
{
  return Status::FAILURE;
}

int VectorGraphics::getLayersCount() const
{
  return mVectorFormat->getLayersCount();
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

} // End namespace I3D