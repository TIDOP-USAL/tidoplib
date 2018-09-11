
/* Cabeceras TidopLib */
#include "core/console.h"
#include "core/messages.h"
#include "geometry/entities/point.h"
#include "geospatial/crs.h"
#include "geometry/transform.h"
#include "img/imgio.h"
#include "math/angles.h"

using namespace TL;
using namespace TL::geometry;



/******************************************************************************\
 *  CABECERA DE FUNCION                                                       *
 ******************************************************************************
 * Nombre.....: svdinverse()                                                  *
 * Descripción: Obtiene la inversa de una matriz por el método SVD ( Single   *
 *             Value Descomposition ).                                        *
 *              Si el número de filas (m), supera al de columnas (n), la      *
 *             la matriz que se obtiene es la que corresponde al residuo      *
 *             mínimo.                                                        *
 *              Si llega un vector de términos independientes ('b'), resuelve *
 *             el sistema poniendo en 'b' las soluciones                     *
 * Entrada....: - *a: Entrada: Matriz directa                                 *
 *                    Salida:  Matriz inversa                                 *
 *              -  m: Número de filas                                         *
 *              -  n: Número de columnas ( m > n )                            *
 *              - *b: Entrada: Vector de términos independientes              *
 *                    Salida: Solucciones del sistema                         *
 * Salida.....: ( 0): Se invirtió sin problemas                               *
 *              ( 1): No pudo invertirse                                      *
 * Notas......: Se han centrado todos los valores. No dice nada el libro pero *
 *     me esta dando muchos problemas siempre que son muy distintos los       *
 *     coeficientes.                                                          *
\******************************************************************************/
int svdinverse( double *a, int m, int n, double *b )
{
  cv::Mat ma( m, n, CV_64F, a );
  int iret = cv::invert( ma, ma, cv::DECOMP_SVD ) == 0;  // Devuelve 0 si es singula
  if ( iret == 0 ) {
    cv::Mat mb( m, 1, CV_64F, b );
    mb = ma * mb;
    memcpy( a, ma.data, sizeof(double)*m*n );
    memcpy( b, mb.data, sizeof(double)*m );
  }
  return( iret );
}


/******************************************************************************\
 *  CABECERA DE FUNCION                                                       *
 ******************************************************************************
 * Nombre.....: gausj()                                                       *
 * Descripción: Resulve un sistema de ecuaciones lineales por el método de    *
 *             Gauss-Jordan                                                   *
 * Entrada....: - *a: Entrada: Matriz de coeficientes del sistema             *
 *                    Salida:  Matriz inversa                                 *
 *              -  n: Número de ecuaciones del sistema                        *
 *              - *b: Entrada: Vector de términos independientes              *
 *                    Salida:  Solucciones del sistema                        *
 *              -  m: De columnas de b ( por si queremos resolver mas de un   *
 *                   sistema a la vez )                                       *
 * Salida.....: ( 0): Se resolvió sin problemas                               *
 *              ( 1): Sistema de ecuaciones imposible                         *
 *              ( 2): Sistema de ecuaciones indeterminado                     *
\******************************************************************************/
//int gaussj(double *a, int n, double *b, int m)
//{
//  int *indxc = 0, *indxr = 0, *ipiv = 0;
//  int i, icol = 0, irow = 0, j, k, l, ll;
//  double big, dum, pivinv;

//  __try {
//    indxc = new int[n];
//    indxr = new int[n];
//    ipiv  = new int[n];
//    memset( ipiv, 0, sizeof(int)*n );

//    for (i=0; i < n; i++) {
//      big = 0.0;
//      for (j=0; j < n; j++) {
//        if (ipiv[j] != 1) {
//          for (k=0; k < n; k++) {
//            if (ipiv[k] == 0) {
//              if (fabs(a[j*n+k]) >= big) {
//                big = fabs(a[j*n+k]);
//                irow = j;
//                icol = k;
//              }
//            } else if (ipiv[k] > 1)
//              //throw sigrid_error( "gaussj: Singular Matrix-1" );
//          }
//        }
//      }
//      ++(ipiv[icol]);
//      if (irow != icol) {
//        for (l=0; l < n; l++) SWAP(a[irow*n+l], a[icol*n+l]);
//        for (l=0; l < m; l++) SWAP(b[irow*m+l], b[icol*m+l]);
//      }

//      indxr[i] = irow;
//      indxc[i] = icol;
//      if (a[icol*n+icol] == 0.0)
//        throw sigrid_error( "gaussj: Singular Matrix-2" );

//      pivinv = 1.0/a[icol*n+icol];
//      a[icol*n+icol] = 1.0;
//      for (l=0; l < n; l++) a[icol*n+l] *= pivinv;
//      for (l=0; l < m; l++) b[icol*m+l] *= pivinv;
//      for (ll=0; ll < n; ll++) {
//        if (ll != icol) {
//          dum = a[ll*n+icol];
//          a[ll*n+icol] = 0.0;
//          for (l=0; l < n; l++) a[ll*n+l] -= a[icol*n+l]*dum;
//          for (l=0; l < m; l++) b[ll*m+l] -= b[icol*m+l]*dum;
//        }
//      }
//    }

//    for (k=n-1; k >= 0; k--) {
//      if (indxr[k] != indxc[k])
//      SWAP(a[k*n+indxr[k]], a[k*n+indxc[k]]);
//    }
//  } __finally {
//    delete[] ipiv;
//    delete[] indxr;
//    delete[] indxc;
//  }
//  return (0);
//}


/******************************************************************************\
 *  CABECERA DE FUNCION                                                       *
 ******************************************************************************
 * Nombre.....: CalMatRot()                                                   *
 * Descripción: Obtiene la matríz de rotación en función de los tres anulos   *
 *             fi, omega y kappa                                              *
 * Entrada....: - *aph: Posición de la cámara x0, y0, z0, fi, omega, kappa    *
 *              - r[3][3]: Matriz de rotación                                 *
 * Notas......: De aph[] solo utiliza los 3 angulos ( aph[3],aph[4],aph[5] )  *
 *             aph[0] = x0,  aph[1] = y0, aph[2] = z0,                        *
 *             aph[3] = f,   aph[4] = w,  aph[5] = k                          *
 *              De acuerdo con la mayoría de los restituidores el angulo de   *
 *             rotación es  omega, fi y kappa.                                *
 *              Comprobado perfectamente para restituidor de topolev          *
\******************************************************************************/
void CalMatRot( double *aph, double r[3][3] )
{
  double &f = aph[3], &w = aph[4], &k = aph[5];
  double sin_f = sin(f);
  double cos_f = cos(f);
  double sin_w = sin(w);
  double cos_w = cos(w);
  double sin_k = sin(k);
  double cos_k = cos(k);

  r[0][0] = cos_f * cos_k;
  r[1][0] = -cos_f * sin_k;
  r[2][0] = sin_f;
  r[0][1] = sin_w * sin_f * cos_k + cos_w * sin_k;
  r[1][1] = -sin_w * sin_f * sin_k + cos_w * cos_k;
  r[2][1] = -sin_w * cos_f;
  r[0][2] = -cos_w * sin_f * cos_k + sin_w * sin_k;
  r[1][2] = cos_w * sin_f * sin_k + sin_w * cos_k;
  r[2][2] = cos_w * cos_f;
}

/******************************************************************************\
 *  CABECERA DE FUNCION                                                       *
 ******************************************************************************
 * Nombre.....: GetPhotoCoo()                                                 *
 * Descripción: Obtiene las coordenadas de un punto en una foto, conocidas    *
 *             la posición de la cámara ( aph[] ) y las coordenas del punto   *
 *             en el terreno                                                  *
 * Entrada....: - *aph: Posición de la cámara x0, y0, z0, fi, omega, kappa    *
 *              - *xt: Coordenadas x, y, z del punto en el terreno            *
 *              - *xf: Coordenadas x, y del punto en la foto (salida)         *
 *              - df:  Distancia focal de la cámara                           *
 *              - r[3][3]: Matríz de rotación                                 *
 * Notas......: Los datos de aph[] son:                                       *
 *             aph[3] = f,   aph[4] = w,  aph[5] = k                          *
 *              Si se ha calculado ya la matriz de rotación, se pasa ese      *
 *             argumento, y se llama a la primera de las funciones            *
 *              Si no se ha calculado se llama a la segunda de las funciones, *
 *             que obtiene la matriz de rotación a partir de la posición de   *
 *             la cámara.                                                     *
 *              En caso de que no varie la posición, es conveniente calcular- *
 *             la una sola vez y llamar a la 1ª función ( a fin de no repetir *
 *             innecesariamente el calculo de r[][] )                         *
\******************************************************************************/
int GetPhotoCoo( double *aph, double *xt, double *xf, double df, double r[3][3] )
{
  double dx, dy, dz, daux;
  dx = xt[0] - aph[0];
  dy = xt[1] - aph[1];
  dz = xt[2] - aph[2];
  daux = -df / ( r[0][2] * dx + r[1][2] * dy + r[2][2] * dz );
  xf[0] = ( r[0][0] * dx + r[1][0] * dy + r[2][0] * dz ) * daux;
  xf[1] = ( r[0][1] * dx + r[1][1] * dy + r[2][1] * dz ) * daux;
  return( 0 );
}
int GetPhotoCoo( double *aph, double *xt, double *xf, double df )
{
  double r[3][3];  // Matriz de rotación
  CalMatRot( aph, r );
  GetPhotoCoo( aph, xt, xf, df, r );
  return( 0 );
}


/******************************************************************************\
 *  CABECERA DE FUNCION                                                       *
 ******************************************************************************
 * Nombre.....: GetTerrCoo()                                                  *
 * Descripción: Obtiene las coordenadas de un punto del terreno, conocidas    *
 *             sus coordenadas foto                                           *
 * Entrada....: - *aph: Posición de la cámara                                 *
 *              - *xf: Coordenadas del punto en la foto                       *
 *              - *xt: Coordenadas x,y,z del punto en el terreno (salida)     *
 *              - df:  Distancia focal cámara                                 *
 *              - r[3][3]: Matríz de rotación cámara                          *
 * Notas......: Los datos de aph[] son:                                       *
 *             aph[0] = x0,  aph[1] = y0, aph[2] = z0,                        *
 *             aph[3] = f,   aph[4] = w,  aph[5] = k                          *
 *              La coordenada z del terreno (xt[2]), debe venir con su valor  *
 *             exacto                                                         *
\******************************************************************************/
//int GetTerrCoo( double *aph, double *xf, double *xt, double df, double r[3][3] )
//{
//   double a[2][2], b[2];
//   a[0][0] = r[0][2] * xf[0] + r[0][0] * df;
//   a[0][1] = r[1][2] * xf[0] + r[1][0] * df;
//   a[1][0] = r[0][2] * xf[1] + r[0][1] * df;
//   a[1][1] = r[1][2] * xf[1] + r[1][1] * df;
//   b[0] = ( r[2][2] * xf[0] + r[2][0] * df ) * ( aph[2] - xt[2] );
//   b[1] = ( r[2][2] * xf[1] + r[2][1] * df ) * ( aph[2] - xt[2] );
//   gaussj( (double *)a, 2, b );
//   xt[0] = b[0] + aph[0];
//   xt[1] = b[1] + aph[1];

//   return(0);
//}
//int GetTerrCoo( double *aph, double *xf, double *xt, double df )
//{
//  double r[3][3];  // Matriz de rotación
//  CalMatRot( aph, r );
//  GetTerrCoo( aph, xf, xt, df, r );
//  return( 0 );
//}

/******************************************************************************\
 *  CABECERA DE FUNCION                                                       *
 ******************************************************************************
 * Nombre.....: GetTerrCoo()                                                  *
 * Descripción: Obtiene las coordenadas de un punto del terreno, conocidas    *
 *             sus coordenadas en 2 fotos                                     *
 * Entrada....: - *aph1: Posición de la cámara 1                              *
 *              - *aph2: Posición de la cámara 2                              *
 *              - *xf: Coordenadas del punto en cada una de las fotos         *
 *                     xf[0]: x foto en la foto 1                             *
 *                     xf[1]: y foto en la foto 1                             *
 *                     xf[2]: x foto en la foto 2                             *
 *                     xf[3]: y foto en la foto 2                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            = f,   aph[4] = w,  aph[5] = k                          *
 *              Si se ha calculado ya las matrices de rotación, se pasan estos*
 *             argumentos, y se llama a la primera de las funciones           *
 *              Si no se han calculado se llama a la segunda de las funciones,*
 *             que obtiene las matrices de rotación a partir de la posición de*
 *             las cámaras.                                                   *
 *              En caso de que no varie la posición, es conveniente calcular- *
 *             las una sola vez y llamar a la 1ª función ( a fin de no repetir*
 *             innecesariamente el calculo de r[][] )                         *
 *              En el calculo hay 4 ecuaciones y 3 incógnitas. Por ello se    *
 *             calcula la solucción óptima por el método SVD                  *
\******************************************************************************/
double (*fnGetZ)( double *, void *) = 0;
int GetTerrCoo( double *aph1, double *aph2, double *xf, double *xt, double df,
                double r1[3][3], double r2[3][3] )
{
  // Calcula coordenadas terreno
  // (xt-x0)(xf*r02+df*r00)+(yt-y0)(xf*r12+df*r10)+(zt-z0)(xf*r22+df*r20)=0
  // (xt-x0)(yf*r02+df*r01)+(yt-y0)(yf*r12+df*r11)+(zt-z0)(yf*r22+df*r21)=0
  double a[4*3], b[4];
  double &xf1 = xf[0], &yf1 = xf[1], &xf2 = xf[2], &yf2 = xf[3];
  double &x01 = aph1[0], &y01 = aph1[1], &z01 = aph1[2];
  double &x02 = aph2[0], &y02 = aph2[1], &z02 = aph2[2];
  a[0] = xf1 * r1[0][2] + df * r1[0][0];
  a[1] = xf1 * r1[1][2] + df * r1[1][0];
  a[2] = xf1 * r1[2][2] + df * r1[2][0];
  a[3] = yf1 * r1[0][2] + df * r1[0][1];
  a[4] = yf1 * r1[1][2] + df * r1[1][1];
  a[5] = yf1 * r1[2][2] + df * r1[2][1];
  a[6] = xf2 * r2[0][2] + df * r2[0][0];
  a[7] = xf2 * r2[1][2] + df * r2[1][0];
  a[8] = xf2 * r2[2][2] + df * r2[2][0];
  a[9] = yf2 * r2[0][2] + df * r2[0][1];
  a[10]= yf2 * r2[1][2] + df * r2[1][1];
  a[11]= yf2 * r2[2][2] + df * r2[2][1];
  b[0] = x01 * a[0] + y01 * a[1] + z01 * a[2];
  b[1] = x01 * a[3] + y01 * a[4] + z01 * a[5];
  b[2] = x02 * a[6] + y02 * a[7] + z02 * a[8];
  b[3] = x02 * a[9] + y02 * a[10]+ z02 * a[11];

  // Resuelve obteniendo solucción óptima ( sobra una ecuación )
  svdinverse( a, 4, 3, b );
  memcpy( xt, b, 3 * sizeof(double) );

  // Si existe "DTM de atado", lo aplica
  if ( fnGetZ ) {
    double zpp = xt[2]; // z calculada mediante el punto de paso
    double &zdm = xt[2];// z calculada mediante dtm
    double pzd = (*fnGetZ)( xt, 0 );  // Obtiene z del dtm y su precisión
    double zdif = fabs( zpp - zdm );
    if ( pzd && zdif > pzd ) {
      // Asigna z ponderada en función de z obtenida del punto de paso y del dtm
      double p = (zdif-pzd) / pzd;  // Peso
      p *= p;   // &&&&&& A ver .......
      xt[2] = ( zpp + p*zdm ) / ( p + 1 );
    } else {
      xt[2] = zpp;
    }
  }
  return(0);
}
int GetTerrCoo( double *aph1, double *aph2, double *xf, double *xt, double df )
{
  double r1[3][3], r2[3][3];  // Matrices de rotación
  CalMatRot( aph1, r1 );
  CalMatRot( aph2, r2 );
  GetTerrCoo( aph1, aph2, xf, xt, df, r1, r2 );
  return(0);
}


template<typename Point_t>
class DifferentialRectification
  : public Transform3D<Point_t>
{
public:

  /*!
   * \brief Traslación en x
   */
  double tx;

  /*!
   * \brief Traslación en y
   */
  double ty;

  /*!
   * \brief Traslación en z
   */
  double tz;

private:

  /*!
   * \brief Escala
   */
  double mScale;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mOmega;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mPhi;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mKappa;

  /*!
   * \brief Matriz de rotación
   */
  std::array<std::array<double, 3>, 3> mR;

  /*!
   * \brief Matriz de rotación inversa
   */
  std::array<std::array<double, 3>, 3> mRinv;

  /*!
   * \brief Tipo de objeto punto. Puede ser Point<Point_t> o CV::Point_<Point_t>
   */
  typedef Point_t value_type;

  /*!
   * \brief Tipo de dato basico. int, float o double
   */
  typedef typename Point_t::value_type sub_type;

public:

  /*!
   * \brief Constructor por defecto
   */
  DifferentialRectification();

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en X
   * \param[in] ty Traslación en Y
   * \param[in] tz Traslación en Z
   * \param[in] scale Escala
   * \param[in] omega Rotación respecto al eje X
   * \param[in] phi Rotación respecto al eje Y
   * \param[in] kappa Rotación respecto al eje Z
   */
  DifferentialRectification(double tx, double ty, double tz, double scale, double omega, double phi, double kappa);

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en X
   * \param[in] ty Traslación en Y
   * \param[in] tz Traslación en Z
   * \param[in] scale Escala
   * \param[in] rotation Matriz de rotación
   */
  DifferentialRectification(double tx, double ty, double tz, double scale, const std::array<std::array<double, 3>, 3> &rotation);

  ~DifferentialRectification() override {}

  transform_status compute(const std::vector<Point_t> &pts1,
                           const std::vector<Point_t> &pts2,
                           std::vector<double> *error, double *rmse) override;

  transform_status transform(const std::vector<Point_t> &ptsIn,
                             std::vector<Point_t> *ptsOut,
                             transform_order trfOrder) const override;

  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder) const override;

  Point_t transform(const Point_t &ptIn, transform_order trfOrder) const override;

  double scale() const;
  void setScale(double scale);

  double omega() const;
  void setOmega(double omega);

  double phi() const;
  void setPhi(double phi);

  double kappa() const;
  void setKappa(double kappa);

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

};

template<typename Point_t> inline
DifferentialRectification<Point_t>::DifferentialRectification()
  : Transform3D<Point_t>(transform_type::DIFFRECTIFICATION, 3),
    tx(0.),
    ty(0.),
    tz(0.),
    mScale(1.),
    mOmega(0.),
    mPhi(0.),
    mKappa(0.)
{
  update();
}

template<typename Point_t> inline
DifferentialRectification<Point_t>::DifferentialRectification(double tx, double ty, double tz, double scale, double omega, double phi, double kappa)
  : Transform3D<Point_t>(transform_type::HELMERT_3D, 3),
    tx(tx),
    ty(ty),
    tz(tz),
    mScale(scale),
    mOmega(omega),
    mPhi(phi),
    mKappa(kappa)
{
  update();
}

template<typename Point_t> inline
DifferentialRectification<Point_t>::DifferentialRectification(double tx, double ty, double tz, double scale, const std::array<std::array<double, 3>, 3> &rotation)
  : Transform3D<Point_t>(transform_type::HELMERT_3D, 3),
    tx(tx),
    ty(ty),
    tz(tz),
    mScale(scale),
    mR(rotation)
{
  eulerAngles(mR, &mOmega, &mPhi, &mKappa);
  update();
}

template<typename Point_t> inline
transform_status DifferentialRectification<Point_t>::compute(const std::vector<Point_t> &pts1,
                                                    const std::vector<Point_t> &pts2,
                                                    std::vector<double> *error,
                                                    double *rmse)
{

}

template<typename Point_t> inline
transform_status DifferentialRectification<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                                               std::vector<Point_t> *ptsOut,
                                                               transform_order trfOrder) const
{
  transform_status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
    if ( r_status == transform_status::FAILURE ) break;
  }
  return r_status;
}

template<typename Point_t> inline
transform_status DifferentialRectification<Point_t>::transform(const Point_t &ptIn,
                                                               Point_t *ptOut,
                                                               transform_order trfOrder) const
{
  transform_status r_status = transform_status::SUCCESS;
  Point_t ptAux = ptIn;
  try {
    if (trfOrder == transform_order::DIRECT){
      //if ( dtmf && ptt.z == 0. ) dtmf->GetZ( &ptt );    // &&&& Puede ser problemático si ptt.z es de verdad 0.
      //GetPhotoCoo( aph, (double*)&ptIn, (double *)ptOut, aph[6], r );
      //return( true );

      //ptOut->x = static_cast<sub_type>( mScale * (ptAux.x*mR[0][0] + ptAux.y*mR[0][1] + ptAux.z*mR[0][2]) + tx );
      //ptOut->y = static_cast<sub_type>( mScale * (ptAux.x*mR[1][0] + ptAux.y*mR[1][1] + ptAux.z*mR[1][2]) + ty );
      //ptOut->z = static_cast<sub_type>( mScale * (ptAux.x*mR[2][0] + ptAux.y*mR[2][1] + ptAux.z*mR[2][2]) + tz );
    } else {
      //int niter;
      //CSL_Vector ptaux = pto;   // Si pto y ptt coinciden hay problemas
      //for( niter = 0; niter < itermax; niter++ ) {
      //  double zin = ptt.z;
      //  GetTerrCoo( aph, (double*)&ptaux, (double *)&ptt, aph[6], r );
      //  if ( !dtmf || fabs( dtmf->GetZ( &ptt ) - zin ) < 1. ) break;
      //}
      //return( niter < itermax );

      //sub_type dx = ptIn.x - tx;
      //sub_type dy = ptIn.y - ty;
      //sub_type dz = ptIn.z - tz;
      //ptOut->x = static_cast<sub_type>(mScale * (dx*mRinv[0][0] + dy*mRinv[0][1] + dz*mRinv[0][2]));
      //ptOut->y = static_cast<sub_type>(mScale * (dx*mRinv[1][0] + dy*mRinv[1][1] + dz*mRinv[1][2]));
      //ptOut->z = static_cast<sub_type>(mScale * (dx*mRinv[2][0] + dy*mRinv[2][1] + dz*mRinv[2][2]));
    }
  } catch (std::exception &e ) {
    msgError("Error in Helmert 3D transformation: %s", e.what());
    r_status = transform_status::FAILURE;
  }
  return r_status;
}

template<typename Point_t> inline
Point_t DifferentialRectification<Point_t>::transform(const Point_t &ptIn,
                                                      transform_order trfOrder) const
{
}

template<typename Point_t> inline
double DifferentialRectification<Point_t>::scale() const
{
  return mScale;
}

template<typename Point_t> inline
void DifferentialRectification<Point_t>::setScale(double scale)
{
  mScale = scale;
}

template<typename Point_t> inline
double DifferentialRectification<Point_t>::omega() const
{
return mOmega;
}

template<typename Point_t> inline
void DifferentialRectification<Point_t>::setOmega(double omega)
{
  mOmega = omega;
  update();
}

template<typename Point_t> inline
double DifferentialRectification<Point_t>::phi() const
{
  return mPhi;
}

template<typename Point_t> inline
void DifferentialRectification<Point_t>::setPhi(double phi)
{
  mPhi = phi;
  update();
}

template<typename Point_t> inline
double DifferentialRectification<Point_t>::kappa() const
{
  return mKappa;
}

template<typename Point_t> inline
void DifferentialRectification<Point_t>::setKappa(double kappa)
{
  mKappa = kappa;
  update();
}



template<typename Point_t> inline
void DifferentialRectification<Point_t>::update()
{
  rotationMatrix(mOmega, mPhi, mKappa, &mR);
#ifdef HAVE_OPENCV
  cv::Mat inv = cv::Mat(3, 3, CV_64F, mR.data()).inv();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      mRinv[i][j] = inv.at<double>(i, j);
    }
  }
#elif HAVE_EIGEN
  Eigen::Map<Matrix<double, 3, 3, RowMajor> > mat(mR.data());
  mat = mat.inverse();
#endif
}

template<typename Point_t>
class TransformOrtho
  : public TrfMultiple<Point_t>
{
public:
  TransformOrtho() {}
};

//// ============================================================================
////   CABECERA DE CLASE
//// ============================================================================
////  Nombre.....: CSL_TransOrtho
////  Descripción: Define la clase CSL_TransOrtho (Transformación ortofoto)
//// ============================================================================
//class CSL_TransOrtho : public CSL_TransMult
//{
//  // Datos miembro de la clase
//public:

//  // Constructoras y destructoras
//public:
//  CSL_TransOrtho();
//  ~CSL_TransOrtho( );

//  // Funciones virtuales
//public:

//  // Funciones propias
//public:
//  cv::Mat CameraIntrinsicParametersGet();
//  cv::Mat CameraExtrinsicParametersGet();
//  void CameraIntrinsicParametersSet( cv::Mat &A, double *_df = 0 );
//  void CameraExtrinsicParametersSet( cv::Mat &Rt );
//  void GetProject( CSL_TransProject *tpr, double z );
//  void GetProject( CSL_TransProject *tpr, double *a );
//  CSL_TransProject *GetProject( CSL_Vector *pt );
//  CSL_TransAfin   *GetTransAfin();
//  CSL_TransRecDif *GetTransRecDif();
//  void Set( CSL_TransOrtho &tor );
//  void SetTransAfin( CSL_TransAfin *tra );
//  void SetTransRecDif( CSL_TransRecDif *trd );
//};

//void GetTerrCoo( CSL_TransOrtho &tro1, CSL_TransOrtho &tro2, CSL_Vector &cp1,
//                 CSL_Vector &cp2, CSL_Vector &ct, CSL_Vector *res = 0  );


/// --file=/home/esteban/desarrollo/data/geoflight/07_0159001.tif --dtm=/home/esteban/desarrollo/data/geoflight/PNOA_MDT05_ETRS89_HU30_0062_LID.asc --x0=531904.511769 --y0=4798399.791190 --z0=2583.106269 --omega=3.858538 --phi=7.638517 --kappa=6.170853 --scale=1

int main(int argc, char** argv)
{

  char name[TL_MAX_FNAME];
  getFileName(getRunfile(), name, TL_MAX_FNAME);


  std::string img_file;
  std::string dtm_file;
  double x0;
  double y0;
  double z0;
  double omega;
  double phi;
  double kappa;
  double scale = 1.;

  Command cmd(name, "Generación de fichero de georeferencia TFW");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("file", "Fichero que se va a georeferenciar", &img_file));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("dtm",  "Modelo digital del terreno", &dtm_file));
  cmd.push_back(std::make_shared<ArgumentDoubleRequired>("x0", 'x', "X0", &x0));
  cmd.push_back(std::make_shared<ArgumentDoubleRequired>("y0", 'y', "Y0", &y0));
  cmd.push_back(std::make_shared<ArgumentDoubleRequired>("z0", 'z', "Z0", &z0));
  cmd.push_back(std::make_shared<ArgumentDoubleRequired>("omega", 'o', "Ángulo omega", &omega));
  cmd.push_back(std::make_shared<ArgumentDoubleRequired>("phi", 'p', "Ángulo omega", &phi));
  cmd.push_back(std::make_shared<ArgumentDoubleRequired>("kappa", 'k', "Ángulo omega", &kappa));
  cmd.push_back(std::make_shared<ArgumentDoubleOptional>("scale", 's', "Escala", &scale));

  Command::Status status = cmd.parse(argc, argv);
  if (status == Command::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == Command::Status::SHOW_HELP) {
    return 0;
  } else if (status == Command::Status::SHOW_LICENCE) {
    return 0;
  } else if (status == Command::Status::SHOW_VERSION) {
    return 0;
  }

//  // Se definen los parámetros y opciones
//  CmdParser cmdParser(name, "Generación de fichero de georeferencia TFW");
//  cmdParser.addParameter("file", "Fichero que se va a georeferenciar");
//  cmdParser.addParameter("dtm", "Modelo digital del terreno");
//  cmdParser.addParameter("x0", "");
//  cmdParser.addParameter("y0", "");
//  cmdParser.addParameter("z0", "");
//  cmdParser.addParameter("omega", "");
//  cmdParser.addParameter("phi", "");
//  cmdParser.addParameter("kappa", "");
//  cmdParser.addParameter("scale", "");

//  // Parseo de los argumentos y comprobación de los mismos
//  CmdParser::Status status = cmdParser.parse(argc, argv);
//  if (status == CmdParser::Status::PARSE_ERROR ) {
//    return 1;
//  } else if (status == CmdParser::Status::PARSE_HELP) {
//    return 0;
//  }

  //std::string img_file = cmdParser.getValue<std::string>("file");
  //std::string dtm_file = cmdParser.getValue<std::string>("dtm");
  //double x0 = cmdParser.getValue<double>("x0");
  //double y0 = cmdParser.getValue<double>("y0");
  //double z0 = cmdParser.getValue<double>("z0");
  //double omega = math::decimalDegreesToRadians(cmdParser.getValue<double>("omega"));
  //double phi = math::decimalDegreesToRadians(cmdParser.getValue<double>("phi"));
  //double kappa = math::decimalDegreesToRadians(cmdParser.getValue<double>("kappa"));
  //double scale = cmdParser.getValue<double>("scale");

  TL::Mdt mdt;
  if (mdt.open(dtm_file) != TL::Mdt::Status::OPEN_OK) return 1;
  PointD pt(x0, y0);
  float z = mdt.getZ(pt);

  TL::RasterGraphics image;
  if (image.open(img_file) == TL::RasterGraphics::Status::OPEN_OK) {

    Helmert3D<Point3D> helmert3d(x0, y0, z0, scale, omega, phi, kappa);

    std::vector<PointD> pti;
    std::vector<PointD> ptw;    // Coordenadas imagen y mundo
    pti.push_back(PointD(0., 0.));
    pti.push_back(PointD(0., image.getRows()));
    pti.push_back(PointD(image.getCols(), 0.));
    pti.push_back(PointD(image.getCols(), image.getRows()));
    for( int ic = 0; ic < 4; ic++ ) {
      //tor->ExecuteInverse( pti[ic], ptw[ic] );
    }

  } else {
    msgError("Error al abrir la imagen: %s", img_file.c_str());
    return 1;
  }

  return 0;
}
