#ifndef I3D_IMG_FORMATS_H
#define I3D_IMG_FORMATS_H

namespace I3D
{

class VrtFrmtOptions
{
public:
  VrtFrmtOptions();
  ~VrtFrmtOptions();

private:

};


class TiffOptions : public VrtFrmtOptions
{

  enum
  {
    TFW,
    RPB,
    RPCTXT,
    TILED
  };

  enum class BIGTIFF
  {
    YES,            /*!< Fuerza BigTiff. */
    NO,             /*!< Fuerza tiff normal. */
    IF_NEEDED,      /*!< . */ 
    IF_SAFER        /*!< . */
  };

  BIGTIFF mBigTiff;


public:

  TiffOptions();
  ~TiffOptions();

private:

};


}



#endif // I3D_IMG_FORMATS_H