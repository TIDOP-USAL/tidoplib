#include "vect/vectio.h"

#include "core/utils.h"
#include "graphic/entities.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogrsf_frmts.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL

namespace TL
{

using namespace graph;
using namespace geometry;


VrtVector::VrtVector() 
  : File(), 
    mLayersCount(0)
{}

int VrtVector::getLayersCount() const
{
  return mLayersCount;
}

#ifdef HAVE_GDAL
GdalVector::GdalVector() 
  : VrtVector(), 
    pDataset(0), 
	  pDriver(0), 
	  mDriverName("")
{
  GDALAllRegister();
  //RegisterGdal::init();
}

void GdalVector::close() 
{
  if (pDataset) GDALClose(pDataset), pDataset = NULL;
  mFile = "";
  mLayersCount = 0;
}

GdalVector::Status GdalVector::open(const char *file, Mode mode)
{
  //TODO: Se puede sacar a una clase de mas bajo nivel esta sección ...
  close();

  mFile = file;
  char ext[TL_MAX_EXT];
  if (getFileExtension(file, ext, TL_MAX_EXT) != 0) return Status::OPEN_FAIL;

  mDriverName = getDriverFromExt(ext);
  if (mDriverName == NULL) return Status::OPEN_FAIL;

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
    if (pDriver == NULL) {
      msgError("%s driver not available.", mDriverName);
      return Status::OPEN_FAIL;
    } else {
      pDataset = pDriver->Create(mFile.c_str(), 0, 0, 0, GDT_Unknown, NULL);
      if (pDataset == NULL) {
        msgError("Creation of output file failed.");
        return Status::OPEN_FAIL;
      }
      return Status::OPEN_OK;
    }
 
  } else {

    pDataset = (GDALDataset*)GDALOpenEx( file, GDAL_OF_VECTOR, NULL, NULL, NULL ); //GDALOpen( file, gdal_access);
    if (pDataset == NULL) {
      return Status::OPEN_FAIL;
    } else {
      pDriver = pDataset->GetDriver();
      update();
      return Status::OPEN_OK; 
    }

  }

//  if (poDataset == NULL) return Status::OPEN_ERROR;
//  else Status::OPEN_OK;
}

GdalVector::Status GdalVector::open(const std::string &file, Mode mode)
{
  return open(file.c_str(), mode);
}

GdalVector::Status  GdalVector::create() 
{
  return Status::FAILURE;
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

void GdalVector::read(const std::string &name, GLayer *layer) 
{
  read(pDataset->GetLayerByName(name.c_str()), layer);
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
      OGRStyleMgr *ogrStyleMgr = nullptr;
      try {
        readEntity(pGeometry, entity.get());
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

void GdalVector::readEntity(OGRGeometry *ogrGeometry, GraphicEntity *gEntity)
{
  OGRwkbGeometryType type;
  if ( wkbHasZ(ogrGeometry->getGeometryType()) )
    type = wkbFlatten(ogrGeometry->getGeometryType());
  else
    type = wkbSetZ(ogrGeometry->getGeometryType());
  switch ( type ) {
  case wkbPoint:
    gEntity = new GPoint();
    readPoint(static_cast<OGRPoint *>(ogrGeometry), static_cast<GPoint *>(gEntity));
    break;
  case wkbLineString:
    gEntity = new GLineString();
    readLineString(static_cast<OGRLineString *>(ogrGeometry), static_cast<GLineString *>(gEntity));
    break;
  case wkbPolygon:
    gEntity = new GPolygon();
    readPolygon(static_cast<OGRPolygon *>(ogrGeometry), static_cast<GPolygon *>(gEntity));
    break;
  case wkbMultiPoint:
    gEntity = new GMultiPoint();
    readMultiPoint(static_cast<OGRMultiPoint *>(ogrGeometry), static_cast<GMultiPoint *>(gEntity));
    break;
  case wkbMultiLineString:
    gEntity = new GMultiLineString();
    readMultiLineString(static_cast<OGRMultiLineString *>(ogrGeometry), static_cast<GMultiLineString *>(gEntity));
    break;
  case wkbMultiPolygon:
    gEntity = new GMultiPolygon();
    readMultiPolygon(static_cast<OGRMultiPolygon *>(ogrGeometry), static_cast<GMultiPolygon *>(gEntity));
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
    //gLineString->operator[](i) = Point<float>(ogrLineString->getX(i), ogrLineString->getY(i));
    (*gLineString)[i].x = ogrLineString->getX(i);
    (*gLineString)[i].y = ogrLineString->getY(i);
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
    OGRPoint *ogrPoint = dynamic_cast<OGRPoint *>(ogrMultiPoint->getGeometryRef(i));
    (*gMultiPoint)[i].x = ogrPoint->getX();
    (*gMultiPoint)[i].y = ogrPoint->getY();
  }
}

void GdalVector::readMultiLineString(OGRMultiLineString *ogrMultiLineString, GMultiLineString *gMultiLineString)
{
  int n = ogrMultiLineString->getNumGeometries();
  gMultiLineString->resize(n);
  for (int i = 0; i < n; i++) {
    OGRLineString *ogrLineString = dynamic_cast<OGRLineString *>(ogrMultiLineString->getGeometryRef(i));
    int np = ogrLineString->getNumPoints();
    (*gMultiLineString)[i].resize(np);
    for (int j = 0; i < np; j++) {
      (*gMultiLineString)[i][j].x = ogrLineString->getX(j);
      (*gMultiLineString)[i][j].y = ogrLineString->getY(j);
    }
  }
}

void GdalVector::readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon, GMultiPolygon *gMultiPolygon)
{
  int n = ogrMultiPolygon->getNumGeometries();
  gMultiPolygon->resize(n);
  for (int i = 0; i < n; i++) {
    OGRPolygon *ogrPolygon = dynamic_cast<OGRPolygon *>(ogrMultiPolygon->getGeometryRef(i));
    OGRLinearRing *ogrLinearRing = ogrPolygon->getExteriorRing();
    int np = ogrLinearRing->getNumPoints();
    (*gMultiPolygon)[i].resize(np);
    for (int j = 0; j < n; j++) {
      (*gMultiPolygon)[i][j].x = ogrLinearRing->getX(j);
      (*gMultiPolygon)[i][j].y = ogrLinearRing->getY(j);
    }
    int nir = ogrPolygon->getNumInteriorRings();

  }
}

void GdalVector::readStyles(OGRStyleMgr *ogrStyle, GraphicStyle *gStyle)
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

void GdalVector::readStylePen(OGRStylePen *ogrStylePen, GraphicStyle *gStyle)
{
  GBool bDefault = false;
  std::shared_ptr<StylePen> stylePen = std::make_shared<StylePen>();
  
  /* Pen Color */
  const char *hexColor = ogrStylePen->Color(bDefault);
  if (!bDefault) {
    stylePen->setPenColor(Color(hexColor));
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
    uint8_t penWidth;
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

    stylePen->setPenWidth(penWidth);
  }
  
  /* Pen Perpendicular Offset */
  double perpendicularOffset = ogrStylePen->PerpendicularOffset(bDefault);
  if (!bDefault) {
    ///TODO: El valor depende de las unidades también
    stylePen->setPerpendicularOffset(static_cast<int32_t>(perpendicularOffset));
  }

  /* Pen Priority Level */
  uint32_t priority = ogrStylePen->Priority(bDefault);
  if (!bDefault) {
    stylePen->setPriorityLevel(priority);
  }

  gStyle->setStylePen(stylePen);
}

void GdalVector::readStyleBrush(OGRStyleBrush *ogrStyleBrush, GraphicStyle *gStyle)
{
  GBool bDefault = false;
  std::shared_ptr<StyleBrush> styleBrush = std::make_shared<StyleBrush>();

  /* Angle */
  double angle = ogrStyleBrush->Angle(bDefault);
  if ( !bDefault ) {
    styleBrush->setAngle(angle); //TODO: ¿Mejor como float en radianes??
  }

  /* Back Color */
  const char *hexColor = ogrStyleBrush->BackColor(bDefault);
  if (!bDefault) {
    styleBrush->setBackColor(Color(hexColor));
  }
  
  /* Fore Color */
  hexColor = ogrStyleBrush->ForeColor(bDefault);
  if (!bDefault) {
    styleBrush->setForeColor(Color(hexColor));
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
  ///TODO: spacingX y spacingY están asociados a un tipo de unidad
  double spacingX = ogrStyleBrush->SpacingX(bDefault);
  GBool bDefault2 = false;
  double spacingY = ogrStyleBrush->SpacingY(bDefault);
  if (!bDefault && !bDefault2) {
    styleBrush->setSpacing(spacingX, spacingY);
  }

  gStyle->setStyleBrush(styleBrush);
}

void GdalVector::readStyleSymbol(OGRStyleSymbol *ogrStyleSymbol, GraphicStyle *gStyle)
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
    styleSymbol->setColor(Color(hexColor));
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
    styleSymbol->setOutlineColor(Color(hexColor));
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

void GdalVector::readStyleLabel(OGRStyleLabel *ogrStyleLabel, GraphicStyle *gStyle)
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
    styleLabel->setBackgroundColor(Color(hexColor));
  }

  /* Foreground Color */
  hexColor = ogrStyleLabel->ForeColor(bDefault);
  if (!bDefault) {
    styleLabel->setForegroundColor(Color(hexColor));
  }

  /* Outline Color */
  hexColor = ogrStyleLabel->OutlineColor(bDefault);
  if (!bDefault) {
    styleLabel->setOutlineColor(Color(hexColor));
  }

  /* Shadow Color */
  hexColor = ogrStyleLabel->ShadowColor(bDefault);
  if (!bDefault) {
    styleLabel->setShadowColor(Color(hexColor));
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


VectorGraphics::Status GdalVector::write(VectorGraphics *vector) 
{
  return Status::FAILURE;
}

VectorGraphics::Status GdalVector::createCopy(const char *fileOut)
{
  return Status::FAILURE;
}

VectorGraphics::Status GdalVector::createLayer(const char *layerName)
{
  OGRLayer *layer = pDataset->CreateLayer(layerName, NULL, wkbPoint, NULL );
  if (layer == NULL) {
    msgError("Layer creation failed.");
    return Status::FAILURE;
  } else {
    
  }
}

VectorGraphics::Status GdalVector::createLayer(const std::string &layerName)
{
  return createLayer(layerName.c_str());
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

VectorGraphics::Status VectorGraphics::open(const char *file, Mode mode)
{
  close();

  mFile = file;
  char ext[TL_MAX_EXT];
  if (getFileExtension(file, ext, TL_MAX_EXT) != 0) return Status::FAILURE;

  const char *frtName;

#ifdef HAVE_GDAL
  if ((frtName = GdalVector::getDriverFromExt(ext)) != NULL) { 
    // Existe un driver de GDAL para el formato vectorial
    mVectorFormat = std::make_unique<GdalVector>();
  } else {
    return Status::FAILURE;
  }
#endif

  if (mVectorFormat) {
    mVectorFormat->open(file, mode);
    update();
    return Status::OPEN_OK;
  } else return Status::OPEN_FAIL;
}

VectorGraphics::Status VectorGraphics::open(const std::string &file, Mode mode)
{
  return open(file.c_str(), mode);
}

VectorGraphics::Status VectorGraphics::create()
{
  return Status::FAILURE;
}

VectorGraphics::Status VectorGraphics::createCopy(const char *fileOut)
{
  return Status::FAILURE;
}

///TODO: Por ahora leo todo pero habría que poder filtrar por ventana.
VectorGraphics::Status VectorGraphics::read()
{
  int n = mVectorFormat->getLayersCount();
  for ( int il = 0; il < n; il++ ) {
    std::shared_ptr<GLayer> gLayer = std::make_shared<GLayer>();
    mVectorFormat->read(il, gLayer.get());
    //TODO: devolver error y comprobar
    mLayers.push_back(gLayer);
  }
  return Status::SUCCESS;
}

VectorGraphics::Status VectorGraphics::read(VectorGraphics *vector)
{
  mVectorFormat->read(vector);
  return Status::SUCCESS;
}

VectorGraphics::Status VectorGraphics::read(int layerId, graph::GLayer *layer)
{
  mVectorFormat->read(layerId, layer);
  return Status::SUCCESS;
}

VectorGraphics::Status VectorGraphics::read(const char *layerName, graph::GLayer *layer)
{
  mVectorFormat->read(layerName, layer);
  return Status::SUCCESS;
}

VectorGraphics::Status VectorGraphics::read(const std::string &layerName, graph::GLayer *layer)
{
  mVectorFormat->read(layerName, layer);
  return Status::SUCCESS;
}

VectorGraphics::Status VectorGraphics::write()
{
  return Status::FAILURE;
}

int VectorGraphics::getLayersCount() const
{
  return mVectorFormat->getLayersCount();
}

graph::GLayer *VectorGraphics::createLayer(const char *layerName)
{
  std::shared_ptr<GLayer> gLayer = std::make_shared<GLayer>();
  gLayer->setName(layerName);
  mLayers.push_back(gLayer);
  return gLayer.get();
}

graph::GLayer *VectorGraphics::createLayer(const std::string &layerName)
{
  return createLayer(layerName.c_str());
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