#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>

#include <tidop/math/statistics.h>


using namespace tl::math;




int main(int argc, char** argv)
{
  std::vector<double> vd;
  vd.push_back(8.0);
  vd.push_back(8.5);
  vd.push_back(7.5);
  vd.push_back(9.0);
  vd.push_back(6.25);
  vd.push_back(5.5);
  vd.push_back(8.5);
  vd.push_back(7.5);
  vd.push_back(8.5);

  double m = mean(vd.begin(), vd.end());

  std::vector<int> vi;
  vi.push_back(1);
  vi.push_back(0);
  vi.push_back(1);
  vi.push_back(3);
  vi.push_back(2);
  vi.push_back(0);
  vi.push_back(1);
  int _mode = mode(vi.begin(), vi.end());

  return 0;
}
