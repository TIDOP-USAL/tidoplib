#include "vect/vectio.h"

#include "core/utils.h"

namespace I3D
{



GdalVector::GdalVector() : VrtVector(), pDataset(0), driver(0)
{

}

void GdalVector::close() 
{

}

int GdalVector::open(const char *file, Mode mode)
{
  //TODO: Se puede sacar a una clase de mas bajo nivel esta sección ...
  close();

  mName = file;
  char ext[I3D_MAX_EXT];
  if (getFileExtension(file, ext, I3D_MAX_EXT) != 0) return 1;

  const char *driverName = getDriverFromExt(ext);
  if (driverName == NULL) return 1;

  GDALAccess gdal_access;
  switch (mode) {
  case I3D::Mode::Read:
    gdal_access = GA_ReadOnly;
    break;
  case I3D::Mode::Update:
    gdal_access = GA_Update;
    break;
  case I3D::Mode::Create:
    gdal_access = GA_Update;
    break;
  default:
    gdal_access = GA_ReadOnly;
    break;
  }
  //....

  if (mode == Mode::Create) {
    return 1;
  } else {
    pDataset = (GDALDataset*)GDALOpen( file, gdal_access);
    if (pDataset == NULL) {
      return 1;
    } else {
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

void GdalVector::read() 
{

}

int GdalVector::write() 
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

}









VectorGraphics::VectorGraphics() : mName("")
{
}

VectorGraphics::~VectorGraphics()
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