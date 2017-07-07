#ifndef I3D_IMG_FORMATS_H
#define I3D_IMG_FORMATS_H

//TODO: Comprobar que se este usando GDAL y la versión para ciertas opciones (> 2.0) 


namespace I3D
{

// Las opciones en GDAL se añaden:

//  char **papszOptions = NULL;
//  papszOptions = CSLSetNameValue( papszOptions, "TILED", "YES" );
//  papszOptions = CSLSetNameValue( papszOptions, "COMPRESS", "PACKBITS" );
//  poDstDS = poDriver->CreateCopy( pszDstFilename, poSrcDS, FALSE,
//                                  papszOptions, GDALTermProgress, NULL );

class RasterOptions
{
public:
  
  enum class Format
  {
    TIFF,
    JPG,
    JP2000,
    PNG,
    BMP
  };

protected:

  Format mFormat;
  DataType mDataTypes[];

public:
  RasterOptions(Format format);
  virtual ~RasterOptions();

  Format getFormat();
  virtual const char *getOptions() = 0;

private:

};


class TiffOptions : public RasterOptions
{

public:
  
  enum
  {
    TFW,            /*!<  Force the generation of an associated ESRI world file (.tfw) */
    RPB,            /*!<  Force the generation of an associated .RPB file to describe RPC (Rational Polynomial Coefficients), if RPC information is available. */
    RPCTXT,         /*!<  Force the generation of an associated _RPC.TXT file to describe RPC (Rational Polynomial Coefficients), if RPC information is available. */ 
    TILED           /*!<  By default stripped TIFF files are created. This option can be used to force creation of tiled TIFF files. */
  };

  enum class BIGTIFF : uint8_t
  {
    YES,            /*!< Fuerza BigTiff. */
    NO,             /*!< Fuerza tiff normal. */
    IF_NEEDED,      /*!< . */ 
    IF_SAFER        /*!< . */
  };

  enum class COMPRESS : uint8_t
  {
    JPEG,
    LZW,
    PACKBITS,
    DEFLATE,
    CCITTRLE,
    CCITTFAX3,
    CCITTFAX4,
    LZMA,
    NONE
  };

  enum class PHOTOMETRIC : uint8_t
  {
    MINISBLACK, 
    MINISWHITE, 
    RGB, CMYK, 
    YCBCR, 
    CIELAB, 
    ICCLAB, 
    ITULAB
  };

  enum class ALPHA : uint8_t
  { 
    YES, 
    NON_PREMULTIPLIED, 
    PREMULTIPLIED, 
    UNSPECIFIED 
  };

  enum class PROFILE : uint8_t
  { 
    GDALGeoTIFF, 
    GeoTIFF, 
    BASELINE 
  };

  enum class PIXELTYPE : uint8_t
  { 
    DEFAULT, 
    SIGNEDBYTE 
  };

  enum class GEOTIFF_KEYS_FLAVOR : uint8_t
  { 
    STANDARD, 
    ESRI_PE 
  };

protected:

  /*!
   * \brief Ancho de tesela.
   * Por defecto el ancho de tesela es de 256
   */
  int BlockXSize;

  /*!
   * \brief Alto de tesela.
   * Por defecto el alto de tesela es de 256
   */
  int BlockYSize;

  /*!
   * \brief BigTiff
   */
  BIGTIFF mBigTiff;

  /*!
   * \brief Compresión utilizada
   */
  COMPRESS mCompress;

  /*!
   * \brief Calidad de compresión jpeg
   * Calidad de compresión cuando se selecciona este método de compresión.
   * El valor esta comprendido entre 0 y 100. Un valor de 100 establece la mayor calidad
   * de imagen y la menor compresión.
   */
  uint8_t mJpegQuality;

  /*!
   * \brief Nivel de compresión DEFLATE
   * Nivel de compresión cuando se selecciona el método de compresión DEFLATE.
   * El valor se encuentra entre 1 y 9. Por defecto es 6. 
   */
  uint8_t mZLevel;

protected:

  //DataType dataTypes;

public:

  TiffOptions();
  ~TiffOptions();

  const char *getOptions() override;

private:

};


}



#endif // I3D_IMG_FORMATS_H