#include <gtest/gtest.h>

#include <tidop/geospatial/dtm.h>

using namespace tl;
using namespace geospatial;

//class DtmTest
//  : public testing::Test
//{
//public:

//  virtual void SetUp()
//  {
//    double radius = 0.;
//    dtm_linear = new DtmLinear(radius);
//    double radius1 = 0.0;
//    double radius2 = 0.0;
//    double angle = 0.0;
//    dtm_linear = new DtmNearest(radius1, radius2, angle);
//  }

//  virtual void TearDown()
//  {
//    if (dtm_linear){
//      delete dtm_linear;
//      dtm_linear = nullptr;
//    }
//  }

//  Dtm *dtm_linear;
//  Dtm *dtm_nearest;
//};

//TEST_F(DtmTest, Constructor)
//{
//  //EXPECT_TRUE(dtm_linear->interpolation() == Dtm::Interpolation::LINEAR);
//  //EXPECT_TRUE(dtm_nearest->interpolation() == Dtm::Interpolation::NEAREST);
//}
