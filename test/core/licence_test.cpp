#include <gtest/gtest.h>

#include <tidop/core/licence.h>

using namespace TL;

class LicenceTest : public testing::Test
{
public:

  virtual void SetUp()
  {
    licence2 = new Licence("Name", "text");
  }

  virtual void TearDown()
  {
    if (licence2){
      delete licence2;
      licence2 = nullptr;
    }
  }

  Licence licence;
  Licence *licence2;
};

TEST_F(LicenceTest, DefaultConstructor)
{
  EXPECT_TRUE(licence.empty());
}

TEST_F(LicenceTest, Constructor)
{
  EXPECT_FALSE(licence2->empty());
}

TEST_F(LicenceTest, setProductName)
{
  licence2->setProductName("OpenCV");
  EXPECT_EQ("OpenCV", licence2->productName());
}

TEST_F(LicenceTest, setText)
{
  licence2->setText("Texto de la licencia");
  EXPECT_EQ("Texto de la licencia", licence2->text());
}

TEST_F(LicenceTest, setVersion)
{
  licence2->setVersion("1.0.0");
  EXPECT_EQ("1.0.0", licence2->version());
}

TEST_F(LicenceTest, setAutor)
{
  licence2->setAutor("autor");
  EXPECT_EQ("autor", licence2->autor());
}

TEST_F(LicenceTest, setUrl)
{
  licence2->setUrl("www.url.com");
  EXPECT_EQ("www.url.com", licence2->url());
}




class AppLicenceTest : public testing::Test
{
public:

  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }

  AppLicence licence;
};


TEST_F(AppLicenceTest, DefaultConstructor)
{
  EXPECT_EQ(0, licence.thirdPartyLicences()->size());
}
