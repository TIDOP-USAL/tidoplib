#include "geometric_entities\segment.h"
#include "geometric_entities\operations.h"

namespace I3D
{

#pragma warning(disable : 4244)
void lineBuffer(const Line &ln, const int size, std::vector<cv::Point> *buff)
{
  *buff = std::vector<cv::Point>(4);
  cv::Point pt1 = ln.pt1;
  cv::Point pt2 = ln.pt2;
  double acimut = azimut(pt1, pt2);
  double dx = size * sin(acimut + CV_PI / 2.);
  double dy = size * cos(acimut + CV_PI / 2.);
  (*buff)[0] = cv::Point(pt1.x + dx, pt1.y + dy);
  (*buff)[1] = cv::Point(pt2.x + dx, pt2.y + dy);
  (*buff)[2] = cv::Point(pt2.x - dx, pt2.y - dy);
  (*buff)[3] = cv::Point(pt1.x - dx, pt1.y - dy);
}
#pragma warning ( default : 4244 )

#pragma warning(disable : 4244)
int projectPointInSegment(const Line &ln, const cv::Point &pt, cv::Point *ptp)
{
  int iret = 0;
  cv::Point v1 = pt - ln.pt1;
  cv::Point v2 = ln.vector();
  double daux = v1.ddot(v2);
  float r = (float)(daux / (v2.x * v2.x + v2.y * v2.y));
  ptp->x = ln.pt1.x + (ln.pt2.x - ln.pt1.x)*r;
  ptp->y = ln.pt1.y + (ln.pt2.y - ln.pt1.y)*r;

  if (daux <= 0) iret = -1;
  else if (daux >= (v2.x * v2.x + v2.y * v2.y)) iret = 1;
  else if (daux == 0) iret = 2; // Esta en la línea
  return iret;
}
#pragma warning ( default : 4244 )

double distPointToSegment(const cv::Point &pt, const Line &ln)
{
  cv::Point ptp;
  int ipr = projectPointInSegment(ln, pt, &ptp);
  if (ipr == -1) ptp = ln.pt1;
  else if (ipr == 1) ptp = ln.pt2;
  return distance(pt, ptp);
}

double distPointToLine(const cv::Point &pt, const Line &ln)
{
  cv::Point v1 = pt - ln.pt1;
  cv::Point v2 = ln.vector();
  return abs(v1.ddot(v2) / (v2.x * v2.x + v2.y * v2.y));
}

double minDistanceSegments(const Line &ln1, const Line &ln2)
{
  double dist[4];
  dist[0] = distPointToSegment(ln1.pt1, ln2);
  dist[1] = distPointToSegment(ln1.pt2, ln2);
  dist[2] = distPointToSegment(ln2.pt1, ln1);
  dist[3] = distPointToSegment(ln2.pt2, ln1);
  return *std::min_element(dist, dist + 4);
}

#pragma warning(disable : 4244)
int intersectSegments(const Line &ln1, const Line &ln2, cv::Point *pt)
{
  int iret = 0;
  cv::Point vs1, vs2;
  vs1 = ln1.vector();
  vs2 = ln2.vector();
  // si el producto vectorial de los vectores que unen ambos segmentos es 0 son paralelas
  if (double cross_product = vs1.cross(vs2)) {
    cv::Point v11_12 = ln2.pt1 - ln1.pt1;
    double t = v11_12.cross(vs2) / cross_product;
    double u = v11_12.cross(vs1) / cross_product;
    if (t >= 0.  &&  t < 1.  &&  u >= 0.  &&  u < 1.) {
      pt->x = ln1.pt1.x + (t * vs1.x);
      pt->y = ln1.pt1.y + (t * vs1.y);
      iret = 1;
    }
  }
  return(iret);
}
#pragma warning(disable : 4244)

#pragma warning(disable : 4244)
int intersectLines(const Line &ln1, const Line &ln2, cv::Point *pt)
{
  int iret = 0;
  cv::Point vs1, vs2;
  vs1 = ln1.vector();
  vs2 = ln2.vector();
  // si el producto vectorial de los vectores que unen ambos segmentos es 0 son paralelas
  if (double cross_product = vs1.cross(vs2)) {
    cv::Point v11_12 = ln2.pt1 - ln1.pt1;
    double t = v11_12.cross(vs2) / cross_product;

    pt->x = ln1.pt1.x + (t * vs1.x);
    pt->y = ln1.pt1.y + (t * vs1.y);
    iret = 1;
  }
  return(iret);
}
#pragma warning(disable : 4244)

void joinLinesByDist(const std::vector<Line> &linesIn, std::vector<Line> *linesOut, int dist)
{
  std::vector<ldGroupLines> linesGrops;
  groupLinesByDist(linesIn, &linesGrops, dist);

  for (int ilg = 0; ilg < linesGrops.size(); ilg++) {
    std::vector<cv::Point> pts;
    int xmin = _INT_MAX;
    int xmax = _INT_MIN;
    for (int il = 0; il < linesGrops[ilg].getSize(); il++) {
      pts.push_back(linesGrops[ilg][il].pt1);
      xmin = std::min(xmin, linesGrops[ilg][il].pt1.x);
      xmax = std::max(xmax, linesGrops[ilg][il].pt1.x);
      pts.push_back(linesGrops[ilg][il].pt2);
      xmin = std::min(xmin, linesGrops[ilg][il].pt2.x);
      xmax = std::max(xmax, linesGrops[ilg][il].pt2.x);
    }
    double m = 0.;
    double b = 0.;
    regressionLinearYX(pts, &m, &b);

    cv::Point pt1 = cv::Point(xmin, cvRound(m * xmin + b));
    cv::Point pt2 = cv::Point(xmax, cvRound(m * xmax + b));
    linesOut->push_back(Line(pt1, pt2));
  }
}

/* ---------------------------------------------------------------------------------- */

ldGroupLines::ldGroupLines()
{
  bbox = WindowI();
}

ldGroupLines::ldGroupLines(const std::vector<Line> &lines)
{
  linesgroup = lines; 
  for (size_t i = 0; i < linesgroup.size(); i++) {
    if (bbox.pt1.x > linesgroup[i].pt1.x) bbox.pt1.x = linesgroup[i].pt1.x;
    if (bbox.pt1.y > linesgroup[i].pt1.y) bbox.pt1.y = linesgroup[i].pt1.y;
    if (bbox.pt2.x < linesgroup[i].pt2.x) bbox.pt2.x = linesgroup[i].pt2.x;
    if (bbox.pt2.y < linesgroup[i].pt2.y) bbox.pt2.y = linesgroup[i].pt2.y;
  }
}

void ldGroupLines::add(const Line &line)
{
  Line laux = line;
  linesgroup.push_back(line);
  WindowI w = laux.getWindow();
  //Se actualiza la ventana  envolvente
  bbox = (bbox.isEmpty() ) ? w : joinWindow(bbox, w);
}

void ldGroupLines::add(const cv::Vec4i &lvect)
{
  Line _line(lvect);
  add(_line);
}

double ldGroupLines::angleMean()
{
  double angle = 0.0;
  for (int i = 0; i < linesgroup.size(); i++){
    angle += linesgroup[i].angleOX();
  }
  angle /= linesgroup.size();
  return angle;
}

void ldGroupLines::DeleteLine(int id)
{
  linesgroup.erase(linesgroup.begin() + id);
}

/* ---------------------------------------------------------------------------------- */

void groupParallelLines(std::vector<Line> linesaux, std::vector<ldGroupLines> *curLinesGrops, double angTol) 
{

  //Comenzamos a agrupar por la primera linea
  while (linesaux.size() > 0) {

    ldGroupLines lg;
    lg.add(linesaux[0]);
    linesaux.erase(linesaux.begin());

    for (int ilg = 0; ilg < lg.getSize(); ilg++) {
      for (int i = 0; i < linesaux.size(); i++) {
        if (lg[ilg].isParallel(linesaux[i], angTol)) {
          lg.add(linesaux[i]);
          linesaux.erase(linesaux.begin() + i);
          i--;
        }
      }
    }
    curLinesGrops->push_back(lg);
  }
}

void groupLinesByDist(const std::vector<Line> &linesIn, std::vector<ldGroupLines> *curLinesGrops, int dist) 
{

  //Comenzamos a agrupar por la primera linea
  std::vector<Line> linesaux = linesIn;
  while (linesaux.size() > 0) {

    ldGroupLines lg;
    lg.add(linesaux[0]);
    linesaux.erase(linesaux.begin());

    for (int ilg = 0; ilg < lg.getSize(); ilg++) {
      for (int i = 0; i < linesaux.size(); i++) {
        if (lg[ilg].isNear(linesaux[i], dist)) {
          lg.add(linesaux[i]);
          linesaux.erase(linesaux.begin() + i);
          i--;
        }
      }
    }
    curLinesGrops->push_back(lg);
  }
}
} // End namespace I3D
