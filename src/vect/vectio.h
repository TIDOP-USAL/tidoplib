#ifndef I3D_VECT_IO_H
#define I3D_VECT_IO_H

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#include "core/defs.h"


namespace I3D
{

/*!
 * \brief Clase contenedor y de gestión de gráficos vectoriales
 *
 */
class VectorGraphics
{
public:
  VectorGraphics();
  ~VectorGraphics();

private:

};



} // End namespace I3D



#endif // I3D_VECT_IO_H
