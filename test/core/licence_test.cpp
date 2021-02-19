#define BOOST_TEST_MODULE Tidop licence test
#include <boost/test/unit_test.hpp>
#include <tidop/core/licence.h>

using namespace tl;

BOOST_AUTO_TEST_SUITE(LicenceTestSuite)

struct LicenceTest
{

  LicenceTest()
    : licence2(nullptr)
  { 
  }

  ~LicenceTest()
  {
    if (licence2){
      delete licence2;
      licence2 = nullptr;
    }
  }

  void setup()
  {
    licence2 = new Licence("PhotoMatch", "GNU Lesser General Public License v3.0");
    licence2->setUrl("https://github.com/TIDOP-USAL/photomatch");
    licence2->setAutor("Esteban Ruiz de Oña Crespo");
    licence2->setAutorEmail("estebanrdo@gmail.com");
    licence2->setVersion("1.0.0");
  }

  void teardown()
  {

  }

  Licence licence;
  Licence *licence2;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, LicenceTest)
{
  BOOST_CHECK_EQUAL(true, licence.empty());
}

BOOST_FIXTURE_TEST_CASE(constructor, LicenceTest)
{
  BOOST_CHECK_EQUAL(false, licence2->empty());
}

BOOST_FIXTURE_TEST_CASE(product_name, LicenceTest)
{
  BOOST_CHECK_EQUAL("PhotoMatch", licence2->productName());
}

BOOST_FIXTURE_TEST_CASE(type, LicenceTest)
{
  BOOST_CHECK_EQUAL("GNU Lesser General Public License v3.0", licence2->type());
}

BOOST_FIXTURE_TEST_CASE(version, LicenceTest)
{
  BOOST_CHECK_EQUAL("1.0.0", licence2->version());
}

BOOST_FIXTURE_TEST_CASE(setAutor, LicenceTest)
{
  BOOST_CHECK_EQUAL("Esteban Ruiz de Oña Crespo", licence2->autor());
}

BOOST_FIXTURE_TEST_CASE(url, LicenceTest)
{
  BOOST_CHECK_EQUAL("https://github.com/TIDOP-USAL/photomatch", licence2->url());
}


BOOST_AUTO_TEST_SUITE_END()


//class AppLicenceTest : public testing::Test
//{
//public:
//
//  virtual void SetUp()
//  {
//  }
//
//  virtual void TearDown()
//  {
//  }
//
//  AppLicence licence;
//};
//
//
//TEST_F(AppLicenceTest, DefaultConstructor)
//{
//  EXPECT_EQ(0, licence.thirdPartyLicences()->size());
//}