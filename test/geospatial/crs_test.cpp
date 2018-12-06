#include <gtest/gtest.h>

#include <tidop/geospatial/crs.h>

using namespace TL;
using namespace TL::geospatial;

/* CrsTest */

class CrsTest : public testing::Test
{
public:

  virtual void SetUp()
  {
    epsg25830 = new Crs("EPSG:25830");
    epsg4258 = new Crs("EPSG:4258");
  }

  virtual void TearDown()
  {
    if (epsg25830){
      delete epsg25830;
      epsg25830 = nullptr;
    }

    if (epsg4258){
      delete epsg4258;
      epsg4258 = nullptr;
    }
  }

  Crs *epsg25830;
  Crs *epsg4258;
};


TEST_F(CrsTest, isGeocentric)
{
  EXPECT_FALSE(epsg25830->isGeocentric());
  EXPECT_FALSE(epsg4258->isGeocentric());
}

TEST_F(CrsTest, isGeographic)
{
  EXPECT_FALSE(epsg25830->isGeographic());
  EXPECT_TRUE(epsg4258->isGeographic());
}
