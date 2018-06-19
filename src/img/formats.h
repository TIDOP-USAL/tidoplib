#ifndef TL_GRAPHIC_IMG_FORMATS_H
#define TL_GRAPHIC_IMG_FORMATS_H

#include "config_tl.h"
#include "core/defs.h"

#include <vector>

#ifdef HAVE_GDAL
TL_DISABLE_WARNING(4251)
#include "gdal_priv.h"
TL_ENABLE_WARNING(4251)
#endif // HAVE_GDAL

#include "core/utils.h" 

///TODO: Comprobar que se este usando GDAL y la versión para ciertas opciones (> 2.0) 


namespace TL
{

// Las opciones en GDAL se añaden:

//  char **papszOptions = NULL;
//  papszOptions = CSLSetNameValue( papszOptions, "TILED", "YES" );
//  papszOptions = CSLSetNameValue( papszOptions, "COMPRESS", "PACKBITS" );
//  poDstDS = poDriver->CreateCopy( pszDstFilename, poSrcDS, FALSE,
//                                  papszOptions, GDALTermProgress, NULL );

enum class DataType : int8_t;

/*!
 * \brief Opciones del formato
 */
class TL_EXPORT RasterOptions : public FileOptions
{

public:
  
  enum class Format
  {
    TIFF,
    JPEG,
    JP2000,
    PNG,
    BMP
  };

protected:

  Format mFormat;
  std::vector<DataType> mDataTypes;

public:

  RasterOptions(Format format);
  virtual ~RasterOptions();

  Format getFormat();
    
  virtual const char *getOptions() = 0;
};

/*!
 * \brief Clase que gestiona las opciones del formato TIFF
 */
class TL_EXPORT TiffOptions : public RasterOptions
{

public:
  
  //enum
  //{
  //  TFW    = 0x00,         /*!<  Force the generation of an associated ESRI world file (.tfw) */
  //  RPB    = 0x01,         /*!<  Force the generation of an associated .RPB file to describe RPC (Rational Polynomial Coefficients), if RPC information is available. */
  //  RPCTXT = 0x02,         /*!<  Force the generation of an associated _RPC.TXT file to describe RPC (Rational Polynomial Coefficients), if RPC information is available. */ 
  //  TILED  = 0x03          /*!<  By default stripped TIFF files are created. This option can be used to force creation of tiled TIFF files. */
  //};

  //enum
  //{
  //  TFW,
  //  RPB,
  //  RPCTXT,
  //  TILED,
  //  BLOCKXSIZE,
  //  BLOCKYSIZE,
  //  JPEGQUALITY,
  //  ZLEVEL,
  //  BIGTIFF,
  //  COMPRESS,
  //  PHOTOMETRIC,
  //  ALPHA,
  //  PROFILE,
  //  PIXELTYPE,
  //  GEOTIFF_KEYS_FLAVOR
  //};

  //typedef bool TFW;
  //typedef bool RPB;
  //typedef bool RPCTX;
  //typedef bool TILED;
  //typedef int BLOCKXSIZE;
  //typedef int BLOCKYSIZE;
  //typedef uint8_t JPEGQUALITY;
  //typedef uint8_t ZLEVEL;

  /*!
   * \brief Propiedades de BIGTIFF
   */
  enum class BIGTIFF : uint8_t
  {
    YES,            /*!< Fuerza BigTiff. */
    NO,             /*!< Fuerza tiff normal. */
    IF_NEEDED,      /*!< BigTiff si es necesario. */ 
    IF_SAFER        /*!< . */
  };

  /*!
   * \brief Tipo de compresión
   */
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
   * \brief Forzar la generación de un fichero ESRI world file (.tfw)
   */
  bool bTFW;

  /*!
   * \brief Force the generation of an associated .RPB file to describe RPC (Rational Polynomial Coefficients), if RPC information is available.
   */
  bool bRPB;

  bool bRPCTX;

  /*!
   * \brief Teselado
   */
  bool bTiled;

  /*!
   * \brief Ancho de tesela.
   * Por defecto el ancho de tesela es de 256
   */
  int mBlockXSize;

  /*!
   * \brief Alto de tesela.
   * Por defecto el alto de tesela es de 256
   */
  int mBlockYSize;

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

  /*!
   * \brief BigTiff
   */
  BIGTIFF mBigTiff;

  /*!
   * \brief Compresión utilizada
   */
  COMPRESS mCompress;

  PHOTOMETRIC mPhotometric;

  ALPHA mAlpha;

  PROFILE mProfile;

  PIXELTYPE mPixelType;

  GEOTIFF_KEYS_FLAVOR mGeotiffKeysFlavor;

public:

  TiffOptions();
  ~TiffOptions();

  const char *getOptions() override;

  void enableTFW(bool value = true);
  void enableRPB(bool value = true);
  void enableRPCTX(bool value = true);
  void enableTiled(bool value = true);

  int getBlockXSize();
  int getBlockYSize();
  uint8_t getJpegQuality();
  uint8_t getZLevel();
  BIGTIFF getBigTiff();
  COMPRESS getCompress();
  PHOTOMETRIC getPhotometric();
  ALPHA getAlpha();
  PROFILE getProfile();
  PIXELTYPE getPixelType();
  GEOTIFF_KEYS_FLAVOR getGeotiffKeysFlavor();

  bool isEnableTFW();
  bool isEnableRPB();
  bool isEnableRPCTX();
  bool isEnableTiled();

  void setBlockXSize(int blockXSize);
  void setBlockYSize(int blockYSize);
  void setJpegQuality(uint8_t jpegQuality);
  void setZLevel(uint8_t zLevel);
  void setBigTiff(BIGTIFF bigTiff);
  void setCompress(COMPRESS compress);
  void setPhotometric(PHOTOMETRIC photometric);
  void setAlpha(ALPHA alpha);
  void setProfile(PROFILE profile);
  void setPixelType(PIXELTYPE pixelType);
  void setGeotiffKeysFlavor(GEOTIFF_KEYS_FLAVOR geotiffKeysFlavor);


  //template <typename Opt_t>
  //void setOption(Opt_t value)
  //{
  //  Opt_t t = Opt_t();
  //  printf(typeid(Opt_t).name());
  //  /*if (strcmp(typeid(Opt_t).name(), typeid(TFW).name()) == 0) {
  //    bTFW = value;
  //  } else if (typeid(Opt_t) == typeid(RPB)) {
  //    bRPB = value;
  //  } else if (typeid(Opt_t) == typeid(RPCTX)) {
  //    bRPCTX = value;
  //  } else if (typeid(Opt_t) == typeid(TILED)) {
  //    bTiled = value;
  //  } else if (typeid(Opt_t) == typeid(BLOCKXSIZE)) {
  //    mBlockXSize = value;
  //  } else if (typeid(Opt_t) == typeid(BLOCKYSIZE)) {
  //    mBlockYSize = value;
  //  } else if (typeid(Opt_t) == typeid(JPEGQUALITY)) {
  //    mJpegQuality = value;
  //  } else if (typeid(Opt_t) == typeid(ZLEVEL)) {
  //    mZLevel = value;
  //  } else*/ if (typeid(Opt_t) == typeid(BIGTIFF)) {
  //    mBigTiff = static_cast<BIGTIFF>(value);
  //  } else if (typeid(Opt_t) == typeid(COMPRESS)) {
  //    mCompress = static_cast<COMPRESS>(value);
  //  } else if (typeid(Opt_t) == typeid(PHOTOMETRIC)) {
  //    mPhotometric = static_cast<PHOTOMETRIC>(value);
  //  } else if (typeid(Opt_t) == typeid(ALPHA)) {
  //    mAlpha = static_cast<ALPHA>(value);
  //  } else if (typeid(Opt_t) == typeid(PROFILE)) {
  //    mProfile = static_cast<PROFILE>(value);
  //  } else if (typeid(Opt_t) == typeid(PIXELTYPE)) {
  //    mPixelType = static_cast<PIXELTYPE>(value);
  //  } else if (typeid(Opt_t) == typeid(GEOTIFF_KEYS_FLAVOR)) {
  //    mGeotiffKeysFlavor = static_cast<GEOTIFF_KEYS_FLAVOR>(value);
  //  } else {
  //    msgWarning("Opción");
  //  } 
  //}


private:

};

/*!
 * \brief Clase que gestiona las opciones del formato PNG
 */
class TL_EXPORT PngOptions : public RasterOptions
{

public:

#if GDAL_VERSION_MAJOR >= 2
  std::string title;         // Title, written in a TEXT or iTXt chunk (GDAL >= 2.0 )
  std::string description;   // Description, written in a TEXT or iTXt chunk (GDAL >= 2.0 )
  std::string copyright;     // Copyright, written in a TEXT or iTXt chunk (GDAL >= 2.0 )
  std::string comment;       // Comment, written in a TEXT or iTXt chunk (GDAL >= 2.0 )
#endif

protected:

  // Force the generation of an associated ESRI world file (with the extension .wld). See World File section for details.
  bool bWorldFile;

  //ZLEVEL=n: Set the amount of time to spend on compression. The default is 6. A value of 1 is fast but does no compression, and a value of 9 is slow but does the best compression.
  //WRITE_METADATA_AS_TEXT=YES/NO: Whether to write source dataset metadata in TEXT chunks (GDAL >= 2.0 )
  //NBITS=1/2/4: Force number of output bits (GDAL >= 2.1 )

public:
  
  PngOptions();
  ~PngOptions();
  
  const char *getOptions() override;

  bool isEnableWorldFile() const;

  void setEnableWorldFile(bool enable);
};


/*!
 * \brief Clase que gestiona las opciones del formato Jpeg
 */
class TL_EXPORT JpegOptions : public RasterOptions
{

protected:
  
  bool bWorldFile; //Force the generation of an associated ESRI world file (with the extension .wld).
  int mQuality;    // By default the quality flag is set to 75, but this option can be used to select other values. Values must be in the range 10-100. Low values result in higher compression ratios, but poorer image quality. Values above 95 are not meaningfully better quality but can but substantially larger.

//PROGRESSIVE=ON: Enabled generation of progressive JPEGs. In some cases these will display a reduced resolution image in viewers such as Netscape, and Internet Explorer, before the full file has been downloaded. However, some applications cannot read progressive JPEGs at all. GDAL can read progressive JPEGs, but takes no advantage of their progressive nature.
//INTERNAL_MASK=YES/NO: By default, if needed, an internal mask in the "zlib compressed mask appended to the file" approach is written to identify pixels that are not valid data. Starting with GDAL 1.10, this can be disabled by setting this option to NO.
//ARITHMETIC=YES/NO: (Starting with GDAL 1.10) To enable arithmetic coding. Not enabled in all libjpeg builds, because of possible legal restrictions.
//BLOCK=1...16: (Starting with GDAL 1.10 and libjpeg 8c) DCT block size. All values from 1 to 16 are possible. Default is 8 (baseline format). A value other than 8 will produce files incompatible with versions prior to libjpeg 8c.
//COLOR_TRANSFORM=RGB or RGB1: (Starting with GDAL 1.10 and libjpeg 9). Set to RGB1 for lossless RGB. Note: this will produce files incompatible with versions prior to libjpeg 9.
//SOURCE_ICC_PROFILE=value: (Starting with GDAL 1.11). ICC profile encoded in Base64.
//COMMENT=string: (Starting with GDAL 2.0). String to embed in a comment JPEG marker. When reading, such strings are exposed in the COMMENT metadata item.
//EXIF_THUMBNAIL=YES/NO: (Starting with GDAL 2.0). Whether to generate an EXIF thumbnail(overview), itself JPEG compressed. Defaults to NO. If enabled, the maximum dimension of the thumbnail will be 128, if neither THUMBNAIL_WIDTH nor THUMBNAIL_HEIGHT are specified.
//THUMBNAIL_WIDTH=n: (Starting with GDAL 2.0). Width of thumbnail. Only taken into account if EXIF_THUMBNAIL=YES.
//THUMBNAIL_HEIGHT=n: (Starting with GDAL 2.0). Height of thumbnail. Only taken into account if EXIF_THUMBNAIL=YES.

public:
  
  JpegOptions();
  ~JpegOptions();
  
  const char *getOptions() override;

  bool isEnableWorldFile() const;

  void setEnableWorldFile(bool enable);

};

/*!
 * \brief Clase que gestiona las opciones del formato bmp
 */
class TL_EXPORT BmpOptions : public RasterOptions
{

protected:

  bool bWorldFile; //Force the generation of an associated ESRI world file(with the extension.wld).

public:
  
  BmpOptions();
  ~BmpOptions();
  
  const char *getOptions() override;

  bool isEnableWorldFile() const;

  void setEnableWorldFile(bool enable);

};


}



#endif // TL_GRAPHIC_IMG_FORMATS_H
