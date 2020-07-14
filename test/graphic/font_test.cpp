#define BOOST_TEST_MODULE Tidop graphic font test
#include <boost/test/unit_test.hpp>
#include <tidop/graphic/font.h>


using namespace tl;
using namespace graph;

BOOST_AUTO_TEST_SUITE(FontTestSuite)

struct FontTest
{

  FontTest()
  {
    font = new Font;
  }

  ~FontTest()
  {
    if (font) {
      delete font;
      font = nullptr;
    }
  }

  Font defFont;
  Font *font;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, FontTest)
{
  BOOST_CHECK_EQUAL("Arial", defFont.name());
  BOOST_CHECK_EQUAL(10, defFont.size());
  BOOST_CHECK_EQUAL(false, defFont.isBold());
  BOOST_CHECK_EQUAL(false, defFont.isItalic());
  BOOST_CHECK_EQUAL(false, defFont.isUnderline());
  BOOST_CHECK_EQUAL(false, defFont.isStrikethrough());
}

BOOST_FIXTURE_TEST_CASE(setName, FontTest)
{
  font->setName("Times New Roman");
  BOOST_CHECK_EQUAL("Times New Roman", font->name());
}

BOOST_FIXTURE_TEST_CASE(setSize, FontTest)
{
  font->setSize(14);
  BOOST_CHECK_EQUAL(14, font->size());
}

BOOST_FIXTURE_TEST_CASE(setBold, FontTest)
{
  font->setBold(true);
  BOOST_CHECK_EQUAL(true, font->isBold());
}

BOOST_FIXTURE_TEST_CASE(setItalic, FontTest)
{
  font->setItalic(true);
  BOOST_CHECK_EQUAL(true, font->isItalic());
}

BOOST_FIXTURE_TEST_CASE(setUnderline, FontTest)
{
  font->setUnderline(true);
  BOOST_CHECK_EQUAL(true, font->isUnderline());
}

BOOST_FIXTURE_TEST_CASE(setStrikethrough, FontTest)
{
  font->setStrikethrough(true);
  BOOST_CHECK_EQUAL(true, font->isStrikethrough());
}

BOOST_FIXTURE_TEST_CASE(setStyle, FontTest)
{
  font->setStyle(Font::Style::bold);
  BOOST_CHECK_EQUAL(true, font->isBold());
  BOOST_CHECK_EQUAL(false, font->isItalic());

  font->setStyle(Font::Style::italic);
  BOOST_CHECK_EQUAL(false, font->isBold());
  BOOST_CHECK_EQUAL(true, font->isItalic());

  font->setStyle(Font::Style::normal);
  BOOST_CHECK_EQUAL(false, font->isBold());
  BOOST_CHECK_EQUAL(false, font->isItalic());

  font->setStyle(Font::Style::bold | Font::Style::italic);
  BOOST_CHECK_EQUAL(true, font->isBold());
  BOOST_CHECK_EQUAL(true, font->isItalic());
}

BOOST_AUTO_TEST_SUITE_END()