#define BOOST_TEST_MODULE Tidop Size test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/size.h>

using namespace tl;


// Constructor por defecto

BOOST_AUTO_TEST_CASE(Size_default_constructor)
{
  SizeI size;
  BOOST_CHECK_EQUAL(0, size.width);
  BOOST_CHECK_EQUAL(0, size.height);
}

BOOST_AUTO_TEST_CASE(SizeD_default_constructor)
{
  SizeD size;
  BOOST_CHECK_EQUAL(0., size.width);
  BOOST_CHECK_EQUAL(0., size.height);
}

BOOST_AUTO_TEST_CASE(SizeF_default_constructor)
{
  SizeF size;
  BOOST_CHECK_EQUAL(0.f, size.width);
  BOOST_CHECK_EQUAL(0.f, size.height);
}

BOOST_AUTO_TEST_CASE(SizeI_constructor)
{
  SizeI size(100, 100);
  BOOST_CHECK_EQUAL(100, size.width);
  BOOST_CHECK_EQUAL(100, size.height);
}

// Constructor de copia

BOOST_AUTO_TEST_CASE(SizeI_copy_constructor)
{
  SizeI size(100, 100);
  SizeI size2(size);
  BOOST_CHECK_EQUAL(100, size2.width);
  BOOST_CHECK_EQUAL(100, size2.height);
}

// Constructor de movimiento

BOOST_AUTO_TEST_CASE(SizeI_move_constructor)
{
  SizeI size(100, 100);
  SizeI size2(std::move(size));
  BOOST_CHECK_EQUAL(100, size2.width);
  BOOST_CHECK_EQUAL(100, size2.height);
}

BOOST_AUTO_TEST_CASE(Size_isEmpty)
{
  SizeI size(100, 100);
  BOOST_CHECK(size.isEmpty() == false);

  SizeI size1;
  BOOST_CHECK(size1.isEmpty());
}

BOOST_AUTO_TEST_CASE(Size_convertType)
{
  // Tamaño par
  SizeI size(100, 100);
  SizeD size_d = static_cast<SizeD>(size);
  BOOST_CHECK_EQUAL(100., size_d.width);
  BOOST_CHECK_EQUAL(100., size_d.height);
}
