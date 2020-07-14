
/* Cabeceras TidopLib */
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geometry/transform.h>
#include <tidop/img/imgio.h>
#include <tidop/math/angles.h>

// filesystem
#if (__cplusplus >= 201703L)
//C++17
//http://en.cppreference.com/w/cpp/filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
//Boost
//http://www.boost.org/doc/libs/1_66_0/libs/filesystem/doc/index.htm
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace TL;
using namespace TL::geometry;



/// --file=/home/esteban/desarrollo/data/geoflight/07_0159001.tif --dtm=/home/esteban/desarrollo/data/geoflight/PNOA_MDT05_ETRS89_HU30_0062_LID.asc --x0=531904.511769 --y0=4798399.791190 --z0=2583.106269 --omega=3.858538 --phi=7.638517 --kappa=6.170853 --scale=1

int main(int argc, char** argv)
{

  fs::path app_path(argv[0]);
  std::string cmd_name = app_path.stem().string();


  std::string img_file;
  std::string dtm_file;
  double x0;
  double y0;
  double z0;
  double omega;
  double phi;
  double kappa;
  double scale = 1.;

  Command cmd(cmd_name, "Generación de fichero de georeferencia TFW");
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

  TL::Mdt mdt;
  if (mdt.open(dtm_file) != TL::Mdt::Status::OPEN_OK) return 1;
  PointD pt(x0, y0);
  float z = mdt.getZ(pt);
  float dz = static_cast<float>(z0) - z;

  TL::GeoRasterGraphics image;
  if (image.open(img_file) == TL::RasterGraphics::Status::OPEN_OK) {
    Helmert3D<Point3D> helmert3d(x0, y0, z0, scale, omega, phi, kappa);

    std::vector<PointD> pti;
    std::vector<PointD> ptw(4);    // Coordenadas imagen y mundo
    pti.push_back(PointD(0., 0.));
    pti.push_back(PointD(image.getCols(), 0.));
    pti.push_back(PointD(image.getCols(), image.getRows()));
    pti.push_back(PointD(0., image.getRows()));

    ///TODO: Se calcula la transformación foto terreno sobre el plano con la z del fotocentro.
    /// Se genera el tfw aproximado
    float focal = 14497.f;
    double s = static_cast<double>(dz / focal);

    ptw[0].x = pt.x - (image.getCols()/2.)*s;
    ptw[0].y = pt.y + (image.getRows()/2.)*s;

    ptw[1].x = pt.x + (image.getCols()/2.)*s;
    ptw[1].y = pt.y + (image.getRows()/2.)*s;

    ptw[2].x = pt.x + (image.getCols()/2.)*s;
    ptw[2].y = pt.y - (image.getRows()/2.)*s;

    ptw[3].x = pt.x - (image.getCols()/2.)*s;
    ptw[3].y = pt.y - (image.getRows()/2.)*s;

    Affine<PointD> affine;
    std::vector<double> error;
    double rmse;
    affine.compute(pti, ptw, &error, &rmse);
    double a;
    double b;
    double c;
    double d;
    affine.getParameters(&a, &b, &c, &d);

    fs::path _path(img_file);
    _path.replace_extension(".tfw");

    std::fstream fs;
    fs.open(_path.string(), std::fstream::out | std::fstream::trunc);
    if (fs.is_open()) {
      fs << a << std::endl;
      fs << c << std::endl;
      fs << b << std::endl;
      fs << d << std::endl;
      fs << affine.tx << std::endl;
      fs << affine.ty << std::endl;
      fs.close();
    }


    //for( int ic = 0; ic < 4; ic++ ) {
      //tor->ExecuteInverse( pti[ic], ptw[ic] );
    //}

  } else {
    msgError("Error al abrir la imagen: %s", img_file.c_str());
    return 1;
  }

  return 0;
}
