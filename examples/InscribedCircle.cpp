#include <vector>
#include <array>

#include <Eigen/SVD>

#include "inscribedCircle.h"

#include "transform.h"
#include "geometric_entities/point.h"

using namespace I3D;
using namespace I3D::geometry;

void solveSVD(int nRows, int nCols, double *a, double *b, double *c)
{
  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, nCols, nRows);
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, nRows);
  Eigen::VectorXd C = A.transpose().jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
}



int main(int argc, char** argv)
{

  CrsTransform<Point3D> trfCrs("EPSG:25830", "EPSG:4326");


  //std::vector<pcl::PointXYZ> vertex {
  //  pcl::PointXYZ(-1.032114506,	-2.032521248,	-0.868462145),
  //  pcl::PointXYZ(-1.037448406,	-2.029991388,	-0.879820168),
  //  pcl::PointXYZ(-1.045242906,	-2.027841568,	-0.881824493),
  //  pcl::PointXYZ(-1.057255030,	-2.025498152,	-0.875770807),
  //  pcl::PointXYZ(-1.084037781,	-2.021459818,	-0.851076961),
  //  pcl::PointXYZ(-1.097332239,	-2.019968987,	-0.833975792),
  //  pcl::PointXYZ(-1.126415968,	-2.016304016,	-0.800367355),
  //  pcl::PointXYZ(-1.135803819,	-2.014907598,	-0.791532278),
  //  pcl::PointXYZ(-1.143613696,	-2.013751268,	-0.784131229),
  //  pcl::PointXYZ(-1.153755426,	-2.012072563,	-0.776191473),
  //  pcl::PointXYZ(-1.191970825,	-2.005923748,	-0.744605184),
  //  pcl::PointXYZ(-1.227418423,	-2.001644135,	-0.701876223),
  //  pcl::PointXYZ(-1.250880837,	-1.999816895,	-0.664111435),
  //  pcl::PointXYZ(-1.260020494,	-1.999348044,	-0.647108853),
  //  pcl::PointXYZ(-1.254010916,	-2.002003908,	-0.636146843),
  //  pcl::PointXYZ(-1.249991298,	-2.003518581,	-0.631281435),
  //  pcl::PointXYZ(-1.236120343,	-2.008097649,	-0.620606184),
  //  pcl::PointXYZ(-1.172698498,	-2.032410860,	-0.539953530),
  //  pcl::PointXYZ(-1.171728611,	-2.033545494,	-0.531524003),
  //  pcl::PointXYZ(-1.165696502,	-2.036163568,	-0.520970345),
  //  pcl::PointXYZ(-1.155209661,	-2.039740801,	-0.511811435),
  //  pcl::PointXYZ(-1.147368908,	-2.042291403,	-0.506133676),
  //  pcl::PointXYZ(-1.130790949,	-2.047986984,	-0.491275102),
  //  pcl::PointXYZ(-1.117684603,	-2.051910400,	-0.484990537),
  //  pcl::PointXYZ(-1.085955024,	-2.061482191,	-0.469088376),
  //  pcl::PointXYZ(-1.056810021,	-2.069511175,	-0.461677313),
  //  pcl::PointXYZ(-1.027299881,	-2.077573061,	-0.454812407),
  //  pcl::PointXYZ(-0.998612285,	-2.085593224,	-0.446415275),
  //  pcl::PointXYZ(-0.998442888,	-2.082420588,	-0.476736039),
  //  pcl::PointXYZ(-1.014490366,	-2.074996471,	-0.509143949),
  //  pcl::PointXYZ(-1.020042539,	-2.071649313,	-0.527699828),
  //  pcl::PointXYZ(-1.024172783,	-2.068889618,	-0.544047534),
  //  pcl::PointXYZ(-1.024986506,	-2.068236589,	-0.548300087),
  //  pcl::PointXYZ(-1.026944280,	-2.066862345,	-0.556670904),
  //  pcl::PointXYZ(-1.027720809,	-2.061460495,	-0.605803430),
  //  pcl::PointXYZ(-1.027700901,	-2.059820175,	-0.621321440),
  //  pcl::PointXYZ(-1.029857635,	-2.057232857,	-0.640668988),
  //  pcl::PointXYZ(-1.029966831,	-2.056794405,	-0.644547582),
  //  pcl::PointXYZ(-1.030662656,	-2.056005716,	-0.650357127),
  //  pcl::PointXYZ(-1.032083750,	-2.053142786,	-0.674029410),
  //  pcl::PointXYZ(-1.031935692,	-2.052427053,	-0.681125522),
  //  pcl::PointXYZ(-1.031587601,	-2.049393177,	-0.710558057),
  //  pcl::PointXYZ(-1.030433774,	-2.048322439,	-0.723362327),
  //  pcl::PointXYZ(-1.030323029,	-2.047525883,	-0.731134415),
  //  pcl::PointXYZ(-1.030365944,	-2.045006752,	-0.754794836),
  //  pcl::PointXYZ(-1.028714299,	-2.042692423,	-0.780494928),
  //  pcl::PointXYZ(-1.031238317,	-2.040423870,	-0.795976758),
  //  pcl::PointXYZ(-1.032269835,	-2.038659334,	-0.810201883),
  //  pcl::PointXYZ(-1.032722354,	-2.037669182,	-0.818481207),
  //  pcl::PointXYZ(-1.029273987,	-2.035270452,	-0.849189758),
  //  pcl::PointXYZ(-1.027913451,	-2.034745455,	-0.857330739)
  //};
  //
  //std::array<double, 4> plane;
  //nPointsPlaneLS(vertex.begin(), vertex.end(), plane, true);

  //pcl::PointXYZ p(0, 0, -1);

  //// Vector normal al plano
  //pcl::PointXYZ Z(plane[0], plane[1], plane[2]);

  //// Se establecen unos ejes arbitrarios a partir del punto central y el primer punto de la polilinea
  //std::array<pcl::PointXYZ, 2> box;
  //box[0].x = box[0].y = box[0].z = std::numeric_limits<float>().max();
  //box[1].x = box[1].y = box[1].z = -std::numeric_limits<float>().max();
  //for (int i = 0; i < box.size(); i++) {
  //  if (box[0].x > vertex[i].x) box[0].x = vertex[i].x;
  //  if (box[0].y > vertex[i].y) box[0].y = vertex[i].y;
  //  if (box[0].z > vertex[i].z) box[0].z = vertex[i].z;
  //  if (box[1].x < vertex[i].x) box[1].x = vertex[i].x;
  //  if (box[1].y < vertex[i].y) box[1].y = vertex[i].y;
  //  if (box[1].z < vertex[i].z) box[1].z = vertex[i].z;
  //}

  //pcl::PointXYZ pt_c((box[0].x + box[1].x) / 2., (box[0].y + box[1].y) / 2., (box[0].z + box[1].z) / 2.);
  //
  ////// Para asegurarmos que los puntos esten en el plano realmente
  ////projectPointToPlane(pt_c, plane, &pt_c);
  ////pcl::PointXYZ pt_1;
  ////projectPointToPlane(vertex[0], plane, &pt_1);
  //pcl::PointXYZ pt_1 = vertex[0];

  //pcl::PointXYZ X = vector3D(pt_c, pt_1);
  //pcl::PointXYZ Y = crossProduct3D(Z, X);

  //// Se normalizan los vectores
  //double n = dotProduct3D(X, X);
  //if (n > 0.) {
  //  X.x = X.x / sqrt(n);
  //  X.y = X.y / sqrt(n);
  //  X.z = X.z / sqrt(n);
  //}
  //n = dotProduct3D(Y, Y);
  //if (n > 0.) {
  //  Y.x = Y.x / sqrt(n);
  //  Y.y = Y.y / sqrt(n);
  //  Y.z = Y.z / sqrt(n);
  //}



  ////// Rotar plano
  ////pcl::PointXYZ A(-1.191970825, -2.005923748, -0.744605184);
  ////pcl::PointXYZ B(-1.084037781, -2.021459818, -0.851076961);
  ////pcl::PointXYZ C(-1.117684603, -2.051910400, -0.484990537);

  ////pcl::PointXYZ X = vector3D(A, B);
  ////pcl::PointXYZ Y = vector3D(A, C);
  ////pcl::PointXYZ Z = crossProduct3D(X, Y);

  ////pcl::PointXYZ p(0, 0, -1);

  ////if (dotProduct3D(Z, p) > 0) {
  ////  Z.x = -Z.x;
  ////  Z.y = -Z.y;
  ////  Z.z = -Z.z;
  ////}
  ////Y = crossProduct3D(Z, X);

  ////double n = dotProduct3D(X, X);
  ////if (n > 0.) {
  ////  X.x = X.x / sqrt(n);
  ////  X.y = X.y / sqrt(n);
  ////  X.z = X.z / sqrt(n);
  ////}
  ////n = dotProduct3D(Y, Y);
  ////if (n > 0.) {
  ////  Y.x = Y.x / sqrt(n);
  ////  Y.y = Y.y / sqrt(n);
  ////  Y.z = Y.z / sqrt(n);
  ////}
  ////n = dotProduct3D(Z, Z);
  ////if (n > 0.) {
  ////  Z.x = Z.x / sqrt(n);
  ////  Z.y = Z.y / sqrt(n);
  ////  Z.z = Z.z / sqrt(n);
  ////}

  //// Matriz de rotación
  //double rot[3*3];
  //rot[0] = X.x;
  //rot[2] = X.z;
  //rot[1] = X.y;
  //rot[3] = Y.x;
  //rot[4] = Y.y;
  //rot[5] = Y.z;
  //rot[6] = Z.x;
  //rot[7] = Z.y;
  //rot[8] = Z.z;

  //std::vector<pcl::PointXYZ> vertex_z(vertex.size());
  //for (int i = 0; i < vertex.size(); i++) {
  //  vertex_z[i].x = vertex[i].x*rot[0] + vertex[i].y*rot[1] + vertex[i].z*rot[2];
  //  vertex_z[i].y = vertex[i].x*rot[3] + vertex[i].y*rot[4] + vertex[i].z*rot[5];
  //  vertex_z[i].z = vertex[i].x*rot[6] + vertex[i].y*rot[7] + vertex[i].z*rot[8];
  //  printf("%f %f %f\n", vertex_z[i].x, vertex_z[i].y, vertex_z[i].z);
  //}

  //pcl::PointXYZ center2D;
  //poleOfInaccessibility2D(vertex_z.begin(), vertex_z.end(), &center2D);
  //// Se añade la Z del plano al punto
  //pcl::PointXYZ center3D(center2D.x, center2D.y, vertex_z[0].z);
  //double radius = distPointToPolygon2D(center3D, vertex_z.begin(), vertex_z.end());

  ////// inversa con eigen
  ////Eigen::MatrixXd mat_rot = Eigen::Map<Eigen::MatrixXd>(rot, 3, 3);
  ////Eigen::MatrixXd mat_rot_inv = mat_rot.inverse();
  ////double rot_inv[3*3];
  ////std::memcpy(rot_inv, mat_rot_inv.data(), 9*sizeof(double));

  ////center3D.x = center2D.x*rot_inv[0] + center2D.y*rot_inv[1] + vertex[0].z*rot_inv[2];
  ////center3D.y = center2D.x*rot_inv[3] + center2D.y*rot_inv[4] + vertex[0].z*rot_inv[5];
  ////center3D.z = center2D.x*rot_inv[6] + center2D.y*rot_inv[7] + vertex[0].z*rot_inv[8];

  exit(EXIT_SUCCESS);
}

