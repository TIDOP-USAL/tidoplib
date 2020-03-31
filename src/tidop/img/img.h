#ifndef TL_IMG_H
#define TL_IMG_H

#include "config_tl.h"

//#include "tidop/core/defs.h"

namespace tl
{

enum class DataType : int8_t
{
  TL_8U,      // Equivalente a CV_8U y GDT_Byte
  TL_8S,      // Equivalente a CV_8S
  TL_16U,     // Equivalente a CV_16U y GDT_UInt16
  TL_16S,     // Equivalente a CV_16S y GDT_Int16
  TL_32U,     // Equivalente a GDT_UInt32
  TL_32S,     // Equivalente a CV_32S y GDT_Int32
  TL_32F,     // Equivalente a CV_32F y GDT_Float32  
  TL_64F      // Equivalente a CV_64F y GDT_Float64
};

} // End namespace tl

#endif // TL_IMG_H