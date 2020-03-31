#ifndef TL_GRAPHIC_IMG_FORMATS_H
#define TL_GRAPHIC_IMG_FORMATS_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <vector>

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal_priv.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL

#include "tidop/core/utils.h"

///TODO: Comprobar que se este usando GDAL y la versión para ciertas opciones (> 2.0) 


namespace tl
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
class TL_EXPORT ImageOptions 
  : public FileOptions
{

public:
  
  enum class Format
  {
    tiff,
    jpeg,
    jp2000,
    png,
    bmp
  };

public:

  ImageOptions(Format format);
  virtual ~ImageOptions();

  Format format();
    
  virtual const char *options() = 0;

protected:

  Format mFormat;
  std::vector<DataType> mDataTypes;

};

/*!
 * \brief Clase que gestiona las opciones del formato TIFF
 */
class TL_EXPORT TiffOptions
  : public ImageOptions
{

public:
  
  //enum
  //{
  //  TFW    = 0x00,         /*!<  Force the generation of an associated ESRI world file (.tfw) */
  //  RPB    = 0x01,         /*!<  Force the generation of an associated .RPB file to describe RPC (Rational Polynomial Coefficients), if RPC information is available. */
  //  RPCTXT = 0x02,         /*!<  Force the generation of an associated _RPC.TXT file to describe RPC (Rational Polynomial Coefficients), if RPC information is available. */ 
  //  TILED  = 0x03          /*!<  By default stripped TIFF files are created. This option can be used to force creation of tiled TIFF files. */
  //};

  enum Active
  {
    tfw         = 0x00,
    rpb         = 0x01,
    rpctxt      = 0x02,
    tiled       = 0x03,
    blockxsize  = 0x04,
    blockysize  = 0x05,
    jpegquality = 0x06,
    zlevel      = 0x07,
    bigtiff     = 0x08,
    compress    = 0x09,
    photometric = 0x10,
    alpha       = 0x11,
    profile     = 0x12,
    pixeltype   = 0x13,
    geotiff_keys_flavor
  };

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

public:

  TiffOptions();
  ~TiffOptions() override;

  const char *options() override;

  void enableTFW(bool value = true);
  bool isEnableTFW() const;

  void enableRPB(bool value = true);
  bool isEnableRPB() const;

  void enableRPCTX(bool value = true);
  bool isEnableRPCTX() const;

  void enableTiled(bool value = true);
  bool isEnableTiled() const;

  void setBlockXSize(int blockXSize);
  int blockXSize() const;

  int blockYSize() const;
  void setBlockYSize(int blockYSize);

  /*!
   * \brief Número de bits
   */
  int nBits() const;
  void setNBits(int nBits);

  uint8_t jpegQuality() const;
  uint8_t zLevel() const;
  BIGTIFF bigTiff() const;
  COMPRESS compress() const;
  PHOTOMETRIC photometric() const;
  ALPHA alpha() const;
  PROFILE profile() const;
  PIXELTYPE pixelType() const;
  GEOTIFF_KEYS_FLAVOR geotiffKeysFlavor() const;

  
  
  void setJpegQuality(uint8_t jpegQuality);
  void setZLevel(uint8_t zLevel);
  void setBigTiff(BIGTIFF bigTiff);
  void setCompress(COMPRESS compress);
  void setPhotometric(PHOTOMETRIC photometric);
  void setAlpha(ALPHA alpha);
  void setProfile(PROFILE profile);
  void setPixelType(PIXELTYPE pixelType);
  void setGeotiffKeysFlavor(GEOTIFF_KEYS_FLAVOR geotiffKeysFlavor);

protected:

  /*!
   * \brief Forzar la generación de un fichero ESRI world file (.tfw)
   */
  bool bTFW;

  /*!
   * \brief Force the generation of an associated .RPB file to describe 
   * RPC (Rational Polynomial Coefficients), if RPC information is available.
   */
  bool bRPB;

  /*!
   * \brief Force the generation of an associated _RPC.TXT file to describe RPC (Rational Polynomial Coefficients), if RPC information is available
   */
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
   * \brief Número de bits
   */
  int mNBits;

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


};

/*!
 * \brief Clase que gestiona las opciones del formato PNG
 */
class TL_EXPORT PngOptions
  : public ImageOptions
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
  ~PngOptions() override;
  
  const char *options() override;

  bool isEnableWorldFile() const;

  void setEnableWorldFile(bool enable);
};


/*!
 * \brief Clase que gestiona las opciones del formato Jpeg
 */
class TL_EXPORT JpegOptions 
  : public ImageOptions
{

public:
  
  JpegOptions();
  ~JpegOptions() override;
  
  const char *options() override;

  bool isEnableWorldFile() const;

  void setEnableWorldFile(bool enable);

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

};

/*!
 * \brief Clase que gestiona las opciones del formato bmp
 */
class TL_EXPORT BmpOptions
  : public ImageOptions
{

protected:

  bool bWorldFile; //Force the generation of an associated ESRI world file(with the extension.wld).

public:
  
  BmpOptions();
  ~BmpOptions() override;
  
  const char *options() override;

  bool isEnableWorldFile() const;

  void setEnableWorldFile(bool enable);

};


}



#endif // TL_GRAPHIC_IMG_FORMATS_H
