#define BOOST_TEST_MODULE Tidop vector test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/vector.h>

using namespace tl::math;

//BOOST_AUTO_TEST_SUITE(VectorTestSuite)
//
//struct VectorTest
//{
//
//  VectorTest() {}
//  ~VectorTest() {}
//
//  void setup()
//  {
//    _vect_2_d[0] = 1.1;
//    _vect_2_d[1] = 3.5;
//
//    _vect_3_d[0] = 3.4;
//    _vect_3_d[1] = 5.7;
//    _vect_3_d[2] = -3.4;
//
//    _vect_4_d[0] = 3.4;
//    _vect_4_d[1] = 5.7;
//    _vect_4_d[2] = -3.4;
//    _vect_4_d[3] = 5.4;
//
//  }
//
//  void teardown()
//  {
//    
//  }
//
//  Vector<3> _vect_def;
//  Vector<2> _vect_2_d;
//  Vector<3> _vect_3_d;
//  Vector<4> _vect_4_d;
//};
//
//
//BOOST_FIXTURE_TEST_CASE(default_constructor, VectorTest)
//{
//  double ini_value = -std::numeric_limits<double>().max();
//
//  for (size_t i = 0; i < _vect_def.size(); i++){
//    BOOST_CHECK_EQUAL(ini_value, _vect_def[i]);
//  }
//}
//
//BOOST_FIXTURE_TEST_CASE(copy_constructor, VectorTest)
//{
//  Vector<3, double> copy(_vect_3_d);
//  for (size_t i = 0; i < _vect_3_d.size(); i++){
//    BOOST_CHECK_EQUAL(_vect_3_d[i], copy[i]);
//  }
//}
//
//BOOST_FIXTURE_TEST_CASE(move_cnstructor, VectorTest)
//{
//  Vector<3, double> move(Vector<3, double>({1.,2.,3.}));
//  BOOST_CHECK_EQUAL(1., move[0]);
//  BOOST_CHECK_EQUAL(2., move[1]);
//  BOOST_CHECK_EQUAL(3., move[2]);
//}
//
//BOOST_FIXTURE_TEST_CASE(initializer_list, VectorTest)
//{
//  Vector<4, double> vect{{1.,2.,3.,4.}};
//  BOOST_CHECK_EQUAL(1., vect[0]);
//  BOOST_CHECK_EQUAL(2., vect[1]);
//  BOOST_CHECK_EQUAL(3., vect[2]);
//  BOOST_CHECK_EQUAL(4., vect[3]);
//}
//
//BOOST_FIXTURE_TEST_CASE(size, VectorTest)
//{
//  BOOST_CHECK_EQUAL(3, _vect_def.size());
//  BOOST_CHECK_EQUAL(2, _vect_2_d.size());
//}
//
//BOOST_FIXTURE_TEST_CASE(iterator, VectorTest)
//{
//  Vector<4>::iterator it = _vect_4_d.begin();
//  BOOST_CHECK_EQUAL( 3.4, *it++);
//  BOOST_CHECK_EQUAL( 5.7, *it++);
//  BOOST_CHECK_EQUAL(-3.4, *it++);
//  BOOST_CHECK_EQUAL( 5.4, *it++);
//  BOOST_CHECK(it == _vect_4_d.end());
//}
//
//BOOST_FIXTURE_TEST_CASE(value_at, VectorTest)
//{
//  BOOST_CHECK_EQUAL(1.1, _vect_2_d[0]);
//  BOOST_CHECK_EQUAL(3.5, _vect_2_d[1]);
//
//  BOOST_CHECK_EQUAL(1.1, _vect_2_d.at(0));
//  BOOST_CHECK_EQUAL(3.5, _vect_2_d.at(1));
//}
//
//BOOST_FIXTURE_TEST_CASE(module, VectorTest)
//{
//  ///TODO
//}
//
//BOOST_FIXTURE_TEST_CASE(normalize, VectorTest)
//{
//  ///TODO
//}
//
//BOOST_FIXTURE_TEST_CASE(zero, VectorTest)
//{
//  Vector<3, int> _zero = Vector<3, int>::zero();
//
//  BOOST_CHECK_EQUAL(0, _zero[0]);
//  BOOST_CHECK_EQUAL(0, _zero[1]);
//  BOOST_CHECK_EQUAL(0, _zero[2]);
//}
//
//
///* Operaciones unarias */
//
//BOOST_FIXTURE_TEST_CASE(plus, VectorTest)
//{
//  Vector<4> v4 = +_vect_4_d;
//
//  BOOST_CHECK_EQUAL( 3.4, v4[0]);
//  BOOST_CHECK_EQUAL( 5.7, v4[1]);
//  BOOST_CHECK_EQUAL(-3.4, v4[2]);
//  BOOST_CHECK_EQUAL( 5.4, v4[3]);
//}
//
//BOOST_FIXTURE_TEST_CASE(minus, VectorTest)
//{
//  Vector<4> v4 = -_vect_4_d;
//
//  BOOST_CHECK_EQUAL(-3.4, v4[0]);
//  BOOST_CHECK_EQUAL(-5.7, v4[1]);
//  BOOST_CHECK_EQUAL( 3.4, v4[2]);
//  BOOST_CHECK_EQUAL(-5.4, v4[3]);
//}
//
///* Operaciones binarias entre vectores */
//
///// Suma de vectores
//
//BOOST_FIXTURE_TEST_CASE(addition, VectorTest)
//{
//  Vector<3, int> v1 = {1, 0, 3};
//  Vector<3, int> v2 = {-1, 4, 2};
//  Vector<3, int> v3 = v1 + v2;
//
//  BOOST_CHECK_EQUAL(0, v3[0]);
//  BOOST_CHECK_EQUAL(4, v3[1]);
//  BOOST_CHECK_EQUAL(5, v3[2]);
//
//  v1 += v2;
//
//  BOOST_CHECK_EQUAL(0, v1[0]);
//  BOOST_CHECK_EQUAL(4, v1[1]);
//  BOOST_CHECK_EQUAL(5, v1[2]);
//}
//
//BOOST_FIXTURE_TEST_CASE(subtraction, VectorTest)
//{
//  Vector<3, int> v1 = {1, 0, 3};
//  Vector<3, int> v2 = {-1, 4, 2};
//  Vector<3, int> v3 = v1 - v2;
//
//  BOOST_CHECK_EQUAL( 2, v3[0]);
//  BOOST_CHECK_EQUAL(-4, v3[1]);
//  BOOST_CHECK_EQUAL( 1, v3[2]);
//
//  v1 -= v2;
//
//  BOOST_CHECK_EQUAL( 2, v1[0]);
//  BOOST_CHECK_EQUAL(-4, v1[1]);
//  BOOST_CHECK_EQUAL( 1, v1[2]);
//}
//
///// Producto
//
//BOOST_FIXTURE_TEST_CASE(multiplication, VectorTest)
//{
//  Vector<3, int> v1 = {1, 0, 3};
//  Vector<3, int> v2 = {-1, 4, 2};
//  Vector<3, int> v3 = v1 * v2;
//
//  BOOST_CHECK_EQUAL(-1, v3[0]);
//  BOOST_CHECK_EQUAL( 0, v3[1]);
//  BOOST_CHECK_EQUAL( 6, v3[2]);
//
//  v1 *= v2;
//
//  BOOST_CHECK_EQUAL(-1, v1[0]);
//  BOOST_CHECK_EQUAL( 0, v1[1]);
//  BOOST_CHECK_EQUAL( 6, v1[2]);
//
//}
//
//
//BOOST_FIXTURE_TEST_CASE(VectorScalar, VectorTest)
//{
//  Vector<3, int> v1 = {1, 0, 3};
//  Vector<3, int> v2 = v1 * 10;
//
//  BOOST_CHECK_EQUAL(10, v2[0]);
//  BOOST_CHECK_EQUAL( 0, v2[1]);
//  BOOST_CHECK_EQUAL(30, v2[2]);
//
//  v1 *= 10;
//
//  BOOST_CHECK_EQUAL(10, v1[0]);
//  BOOST_CHECK_EQUAL( 0, v1[1]);
//  BOOST_CHECK_EQUAL(30, v1[2]);
//}
//
//
//BOOST_FIXTURE_TEST_CASE(ScalarVector, VectorTest)
//{
//  Vector<3, int> v1 = {1, 0, 3};
//  Vector<3, int> v2 = 10 * v1;
//
//  BOOST_CHECK_EQUAL(10, v2[0]);
//  BOOST_CHECK_EQUAL( 0, v2[1]);
//  BOOST_CHECK_EQUAL(30, v2[2]);
//
//}
//
//BOOST_FIXTURE_TEST_CASE(dotProduct, VectorTest)
//{
//
//}
//
//BOOST_AUTO_TEST_SUITE_END()
//
//
//
//
//BOOST_AUTO_TEST_SUITE(VectorDynTestSuite)
//
//struct VectorDynTest
//{
//
//  VectorDynTest() {}
//  ~VectorDynTest() 
//  {
//    delete _vect_def;
//    delete _vect_3_d;
//    delete _vect_4_d;
//  }
//
//  void setup()
//  {
//    _vect_def = new VectorDyn<double>();
//
//    _vect_2_d = new VectorDyn<double>(2);
//    (*_vect_2_d)[0] = 1.1;
//    (*_vect_2_d)[1] = 3.5;
//
//    _vect_3_d = new VectorDyn<double>(3);
//    (*_vect_3_d)[0] = 3.4;
//    (*_vect_3_d)[1] = 5.7;
//    (*_vect_3_d)[2] = -3.4;
//
//    _vect_4_d = new VectorDyn<double>(4);
//    (*_vect_4_d)[0] = 3.4;
//    (*_vect_4_d)[1] = 5.7;
//    (*_vect_4_d)[2] = -3.4;
//    (*_vect_4_d)[3] = 5.4;
//
//    _vect_.resize(4);
//    _vect_[0] = 3.4;
//    _vect_[1] = 5.7;
//    _vect_[2] = -3.4;
//    _vect_[3] = 5.4;
//  }
//
//  void teardown()
//  {
//    
//  }
//
//  VectorDyn<double> *_vect_def;
//  VectorDyn<double> *_vect_2_d;
//  VectorDyn<double> *_vect_3_d;
//  VectorDyn<double> *_vect_4_d;
//
//  VectorDyn<double> _vect_;
//};
//
//
//BOOST_FIXTURE_TEST_CASE(copy_constructor, VectorDynTest)
//{
//  VectorDyn<double> copy(*_vect_3_d);
//  for (size_t i = 0; i < _vect_3_d->size(); i++){
//    BOOST_CHECK_EQUAL(_vect_3_d->at(i), copy[i]);
//  }
//}
//
//BOOST_FIXTURE_TEST_CASE(move_cnstructor, VectorDynTest)
//{
//  VectorDyn<double> to_move(*_vect_3_d);
//  VectorDyn<double> move(std::move(to_move));
//  BOOST_CHECK_EQUAL(3, move.size());
//  BOOST_CHECK_EQUAL(0, to_move.size());
//  BOOST_CHECK_EQUAL(3.4, move[0]);
//  BOOST_CHECK_EQUAL(5.7, move[1]);
//  BOOST_CHECK_EQUAL(-3.4, move[2]);
//}
//
//BOOST_FIXTURE_TEST_CASE(initializer_list, VectorDynTest)
//{
//  VectorDyn<double> vect{{1.,2.,3.,4.}};
//  BOOST_CHECK_EQUAL(1., vect[0]);
//  BOOST_CHECK_EQUAL(2., vect[1]);
//  BOOST_CHECK_EQUAL(3., vect[2]);
//  BOOST_CHECK_EQUAL(4., vect[3]);
//}
//
//BOOST_FIXTURE_TEST_CASE(size, VectorDynTest)
//{
//  BOOST_CHECK_EQUAL(0, _vect_def->size());
//  BOOST_CHECK_EQUAL(2, _vect_2_d->size());
//}
//
//BOOST_FIXTURE_TEST_CASE(iterator, VectorDynTest)
//{
//  VectorDyn<double>::iterator it = _vect_4_d->begin();
//  BOOST_CHECK_EQUAL( 3.4, *it++);
//  BOOST_CHECK_EQUAL( 5.7, *it++);
//  BOOST_CHECK_EQUAL(-3.4, *it++);
//  BOOST_CHECK_EQUAL( 5.4, *it++);
//  BOOST_CHECK(it == _vect_4_d->end());
//}
//
//BOOST_FIXTURE_TEST_CASE(value_at, VectorDynTest)
//{
//  BOOST_CHECK_EQUAL(1.1, (*_vect_2_d)[0]);
//  BOOST_CHECK_EQUAL(3.5, (*_vect_2_d)[1]);
//
//  BOOST_CHECK_EQUAL(1.1, _vect_2_d->at(0));
//  BOOST_CHECK_EQUAL(3.5, _vect_2_d->at(1));
//}
//
////BOOST_FIXTURE_TEST_CASE(module, VectorDynTest)
////{
////  ///TODO
////}
////
////BOOST_FIXTURE_TEST_CASE(normalize, VectorDynTest)
////{
////  ///TODO
////}
//
//BOOST_FIXTURE_TEST_CASE(zero, VectorDynTest)
//{
//  VectorDyn<int> _zero = VectorDyn<int>::zero(3);
//
//  BOOST_CHECK_EQUAL(0, _zero[0]);
//  BOOST_CHECK_EQUAL(0, _zero[1]);
//  BOOST_CHECK_EQUAL(0, _zero[2]);
//}
//
//
///* Operaciones unarias */
//
//BOOST_FIXTURE_TEST_CASE(plus, VectorDynTest)
//{
//  VectorDyn<double> v4 = +_vect_;
//
//  BOOST_CHECK_EQUAL( 3.4, v4[0]);
//  BOOST_CHECK_EQUAL( 5.7, v4[1]);
//  BOOST_CHECK_EQUAL(-3.4, v4[2]);
//  BOOST_CHECK_EQUAL( 5.4, v4[3]);
//}
//
//BOOST_FIXTURE_TEST_CASE(minus, VectorDynTest)
//{
//  VectorDyn<double> v4 = -_vect_;
//
//  BOOST_CHECK_EQUAL(-3.4, v4[0]);
//  BOOST_CHECK_EQUAL(-5.7, v4[1]);
//  BOOST_CHECK_EQUAL( 3.4, v4[2]);
//  BOOST_CHECK_EQUAL(-5.4, v4[3]);
//}
//
///* Operaciones binarias entre vectores */
//
///// Suma de vectores
//
//BOOST_FIXTURE_TEST_CASE(addition, VectorDynTest)
//{
//  VectorDyn<int> v1 = {1, 0, 3};
//  VectorDyn<int> v2 = {-1, 4, 2};
//  VectorDyn<int> v3 = v1 + v2;
//
//  BOOST_CHECK_EQUAL(0, v3[0]);
//  BOOST_CHECK_EQUAL(4, v3[1]);
//  BOOST_CHECK_EQUAL(5, v3[2]);
//
//  v1 += v2;
//
//  BOOST_CHECK_EQUAL(0, v1[0]);
//  BOOST_CHECK_EQUAL(4, v1[1]);
//  BOOST_CHECK_EQUAL(5, v1[2]);
//}
//
//BOOST_FIXTURE_TEST_CASE(subtraction, VectorDynTest)
//{
//  VectorDyn<int> v1 = {1, 0, 3};
//  VectorDyn<int> v2 = {-1, 4, 2};
//  VectorDyn<int> v3 = v1 - v2;
//
//  BOOST_CHECK_EQUAL( 2, v3[0]);
//  BOOST_CHECK_EQUAL(-4, v3[1]);
//  BOOST_CHECK_EQUAL( 1, v3[2]);
//
//  v1 -= v2;
//
//  BOOST_CHECK_EQUAL( 2, v1[0]);
//  BOOST_CHECK_EQUAL(-4, v1[1]);
//  BOOST_CHECK_EQUAL( 1, v1[2]);
//}
//
///// Producto
//
//BOOST_FIXTURE_TEST_CASE(multiplication, VectorDynTest)
//{
//  VectorDyn<int> v1 = {1, 0, 3};
//  VectorDyn<int> v2 = {-1, 4, 2};
//  VectorDyn<int> v3 = v1 * v2;
//
//  BOOST_CHECK_EQUAL(-1, v3[0]);
//  BOOST_CHECK_EQUAL( 0, v3[1]);
//  BOOST_CHECK_EQUAL( 6, v3[2]);
//
//  v1 *= v2;
//
//  BOOST_CHECK_EQUAL(-1, v1[0]);
//  BOOST_CHECK_EQUAL( 0, v1[1]);
//  BOOST_CHECK_EQUAL( 6, v1[2]);
//
//}
//
//
//BOOST_FIXTURE_TEST_CASE(VectorScalar, VectorDynTest)
//{
//  VectorDyn<int> v1 = {1, 0, 3};
//  VectorDyn<int> v2 = v1 * 10;
//
//  BOOST_CHECK_EQUAL(10, v2[0]);
//  BOOST_CHECK_EQUAL( 0, v2[1]);
//  BOOST_CHECK_EQUAL(30, v2[2]);
//
//  v1 *= 10;
//
//  BOOST_CHECK_EQUAL(10, v1[0]);
//  BOOST_CHECK_EQUAL( 0, v1[1]);
//  BOOST_CHECK_EQUAL(30, v1[2]);
//}
//
//
//BOOST_FIXTURE_TEST_CASE(ScalarVector, VectorDynTest)
//{
//  VectorDyn<int> v1 = {1, 0, 3};
//  VectorDyn<int> v2 = 10 * v1;
//
//  BOOST_CHECK_EQUAL(10, v2[0]);
//  BOOST_CHECK_EQUAL( 0, v2[1]);
//  BOOST_CHECK_EQUAL(30, v2[2]);
//
//}
//
//BOOST_FIXTURE_TEST_CASE(dotProduct, VectorDynTest)
//{
//
//}
//
//BOOST_AUTO_TEST_SUITE_END()
//

BOOST_AUTO_TEST_SUITE(VectorTestSuite)

struct VectorTest
{

  VectorTest()
  : _vect_dynamic_def(new Vector<double>()),
    _vect_dynamic_def_4(new Vector<double>(4)),
    _vect_dynamic_2_d(new Vector<double>(2)),
    _vect_dynamic_3_d(new Vector<double>(3)),
    _vect_dynamic_4_d(new Vector<double>(4))
  {}
  ~VectorTest()
  {
    delete _vect_dynamic_def;
    delete _vect_dynamic_def_4;

    delete _vect_dynamic_2_d;
    delete _vect_dynamic_3_d;
    delete _vect_dynamic_4_d;
  }

  void setup()
  {
    _vect_2_d[0] = 1.1;
    _vect_2_d[1] = 3.5;

    (*_vect_dynamic_2_d)[0] = 1.1;
    (*_vect_dynamic_2_d)[1] = 3.5;

    _vect_3_d[0] = 3.4;
    _vect_3_d[1] = 5.7;
    _vect_3_d[2] = -3.4;

    (*_vect_dynamic_3_d)[0] = 3.4;
    (*_vect_dynamic_3_d)[1] = 5.7;
    (*_vect_dynamic_3_d)[2] = -3.4;

    _vect_4_d[0] = 3.4;
    _vect_4_d[1] = 5.7;
    _vect_4_d[2] = -3.4;
    _vect_4_d[3] = 5.4;
        
    (*_vect_dynamic_4_d)[0] = 3.4;
    (*_vect_dynamic_4_d)[1] = 5.7;
    (*_vect_dynamic_4_d)[2] = -3.4;
    (*_vect_dynamic_4_d)[3] = 5.4;
  }

  void teardown()
  {
    
  }

  Vector<double, 3> _vect_def_3;
  Vector<double> *_vect_dynamic_def;
  Vector<double> *_vect_dynamic_def_4;
  Vector<double, 2> _vect_2_d;
  Vector<double> *_vect_dynamic_2_d;
  Vector<double, 3> _vect_3_d;
  Vector<double> *_vect_dynamic_3_d;
  Vector<double, 4> _vect_4_d;
  Vector<double> *_vect_dynamic_4_d;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, VectorTest)
{
  double ini_value = -std::numeric_limits<double>().max();

  for (size_t i = 0; i < _vect_def_3.size(); i++){
    BOOST_CHECK_EQUAL(ini_value, _vect_def_3[i]);
  }

  for (size_t i = 0; i < _vect_dynamic_def_4->size(); i++){
    BOOST_CHECK_EQUAL(ini_value, _vect_dynamic_def_4->at(i));
  }
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, VectorTest)
{
  Vector<double, 3> copy(_vect_3_d);
  for (size_t i = 0; i < _vect_3_d.size(); i++){
    BOOST_CHECK_EQUAL(_vect_3_d[i], copy[i]);
  }

  Vector<double> copy_dyn(*_vect_dynamic_3_d);
  for (size_t i = 0; i < _vect_dynamic_3_d->size(); i++){
    BOOST_CHECK_EQUAL(_vect_dynamic_3_d->at(i), copy_dyn[i]);
  }
}

BOOST_FIXTURE_TEST_CASE(move_cnstructor, VectorTest)
{
  Vector<double, 3> to_move;
  to_move[0] = 1.;
  to_move[1] = 2.;
  to_move[2] = 3.;

  Vector<double, 3> move(std::move(to_move));
  BOOST_CHECK_EQUAL(1., move[0]);
  BOOST_CHECK_EQUAL(2., move[1]);
  BOOST_CHECK_EQUAL(3., move[2]);
  BOOST_CHECK_EQUAL(3, to_move.size());

  Vector<double> to_move2(3);
  to_move2[0] = 1.;
  to_move2[1] = 2.;
  to_move2[2] = 3.;

  Vector<double> move2(std::move(to_move2));
  BOOST_CHECK_EQUAL(1., move2[0]);
  BOOST_CHECK_EQUAL(2., move2[1]);
  BOOST_CHECK_EQUAL(3., move2[2]);
  BOOST_CHECK_EQUAL(0, to_move2.size());
}

BOOST_FIXTURE_TEST_CASE(initializer_list, VectorTest)
{
  Vector<double, 4> vect{{1.,2.,3.,4.}};
  BOOST_CHECK_EQUAL(1., vect[0]);
  BOOST_CHECK_EQUAL(2., vect[1]);
  BOOST_CHECK_EQUAL(3., vect[2]);
  BOOST_CHECK_EQUAL(4., vect[3]);

  Vector<double> vect2{{1.,2.,3.,4.}};
  BOOST_CHECK_EQUAL(1., vect2[0]);
  BOOST_CHECK_EQUAL(2., vect2[1]);
  BOOST_CHECK_EQUAL(3., vect2[2]);
  BOOST_CHECK_EQUAL(4., vect2[3]);
}

BOOST_FIXTURE_TEST_CASE(size, VectorTest)
{
  BOOST_CHECK_EQUAL(3, _vect_def_3.size());
  BOOST_CHECK_EQUAL(0, _vect_dynamic_def->size());
  BOOST_CHECK_EQUAL(4, _vect_dynamic_def_4->size());
}

BOOST_FIXTURE_TEST_CASE(iterator, VectorTest)
{
  Vector<double, 4>::iterator it = _vect_4_d.begin();
  BOOST_CHECK_EQUAL( 3.4, *it++);
  BOOST_CHECK_EQUAL( 5.7, *it++);
  BOOST_CHECK_EQUAL(-3.4, *it++);
  BOOST_CHECK_EQUAL( 5.4, *it++);
  BOOST_CHECK(it == _vect_4_d.end());
}

BOOST_FIXTURE_TEST_CASE(value_at, VectorTest)
{
  BOOST_CHECK_EQUAL(1.1, _vect_2_d[0]);
  BOOST_CHECK_EQUAL(3.5, _vect_2_d[1]);

  BOOST_CHECK_EQUAL(1.1, _vect_dynamic_2_d->at(0));
  BOOST_CHECK_EQUAL(3.5, _vect_dynamic_2_d->at(1));
}

//BOOST_FIXTURE_TEST_CASE(module, VectorTest)
//{
//  ///TODO
//}
//
//BOOST_FIXTURE_TEST_CASE(normalize, VectorTest)
//{
//  ///TODO
//}

BOOST_FIXTURE_TEST_CASE(zero, VectorTest)
{
  Vector<int, 3> _zero = Vector<int, 3>::zero();

  BOOST_CHECK_EQUAL(0, _zero[0]);
  BOOST_CHECK_EQUAL(0, _zero[1]);
  BOOST_CHECK_EQUAL(0, _zero[2]);
  BOOST_CHECK_EQUAL(3, _zero.size());

  Vector<int> _zero2 = Vector<int>::zero(3);

  BOOST_CHECK_EQUAL(0, _zero2[0]);
  BOOST_CHECK_EQUAL(0, _zero2[1]);
  BOOST_CHECK_EQUAL(0, _zero2[2]);
  BOOST_CHECK_EQUAL(3, _zero2.size());
}


/* Operaciones unarias */

BOOST_FIXTURE_TEST_CASE(plus, VectorTest)
{
  Vector<double, 4> v4 = +_vect_4_d;

  BOOST_CHECK_EQUAL( 3.4, v4[0]);
  BOOST_CHECK_EQUAL( 5.7, v4[1]);
  BOOST_CHECK_EQUAL(-3.4, v4[2]);
  BOOST_CHECK_EQUAL( 5.4, v4[3]);

  Vector<double> v4_dyn = +(*_vect_dynamic_4_d);

  BOOST_CHECK_EQUAL( 3.4, v4_dyn[0]);
  BOOST_CHECK_EQUAL( 5.7, v4_dyn[1]);
  BOOST_CHECK_EQUAL(-3.4, v4_dyn[2]);
  BOOST_CHECK_EQUAL( 5.4, v4_dyn[3]);
}

BOOST_FIXTURE_TEST_CASE(minus, VectorTest)
{
  Vector<double, 4> v4 = -_vect_4_d;

  BOOST_CHECK_EQUAL(-3.4, v4[0]);
  BOOST_CHECK_EQUAL(-5.7, v4[1]);
  BOOST_CHECK_EQUAL( 3.4, v4[2]);
  BOOST_CHECK_EQUAL(-5.4, v4[3]);

  Vector<double> v4_dyn = -(*_vect_dynamic_4_d);

  BOOST_CHECK_EQUAL(-3.4, v4_dyn[0]);
  BOOST_CHECK_EQUAL(-5.7, v4_dyn[1]);
  BOOST_CHECK_EQUAL( 3.4, v4_dyn[2]);
  BOOST_CHECK_EQUAL(-5.4, v4_dyn[3]);
}

/* Operaciones binarias entre vectores */

/// Suma de vectores

BOOST_FIXTURE_TEST_CASE(addition, VectorTest)
{
  Vector<int, 3> v1 = {1, 0, 3};
  Vector<int, 3> v2 = {-1, 4, 2};
  Vector<int, 3> v3 = v1 + v2;

  BOOST_CHECK_EQUAL(0, v3[0]);
  BOOST_CHECK_EQUAL(4, v3[1]);
  BOOST_CHECK_EQUAL(5, v3[2]);

  v1 += v2;

  BOOST_CHECK_EQUAL(0, v1[0]);
  BOOST_CHECK_EQUAL(4, v1[1]);
  BOOST_CHECK_EQUAL(5, v1[2]);


  Vector<int> v1_dyn = {1, 0, 3};
  Vector<int> v2_dyn = {-1, 4, 2};
  Vector<int> v3_dyn = v1_dyn + v2_dyn;

  BOOST_CHECK_EQUAL(0, v3_dyn[0]);
  BOOST_CHECK_EQUAL(4, v3_dyn[1]);
  BOOST_CHECK_EQUAL(5, v3_dyn[2]);

  v1_dyn += v2_dyn;

  BOOST_CHECK_EQUAL(0, v1_dyn[0]);
  BOOST_CHECK_EQUAL(4, v1_dyn[1]);
  BOOST_CHECK_EQUAL(5, v1_dyn[2]);
}

BOOST_FIXTURE_TEST_CASE(subtraction, VectorTest)
{
  Vector<int, 3> v1 = {1, 0, 3};
  Vector<int, 3> v2 = {-1, 4, 2};
  Vector<int, 3> v3 = v1 - v2;

  BOOST_CHECK_EQUAL( 2, v3[0]);
  BOOST_CHECK_EQUAL(-4, v3[1]);
  BOOST_CHECK_EQUAL( 1, v3[2]);

  v1 -= v2;

  BOOST_CHECK_EQUAL( 2, v1[0]);
  BOOST_CHECK_EQUAL(-4, v1[1]);
  BOOST_CHECK_EQUAL( 1, v1[2]);


  Vector<int> v1_dyn = {1, 0, 3};
  Vector<int> v2_dyn = {-1, 4, 2};
  Vector<int> v3_dyn = v1_dyn - v2_dyn;

  BOOST_CHECK_EQUAL( 2, v3_dyn[0]);
  BOOST_CHECK_EQUAL(-4, v3_dyn[1]);
  BOOST_CHECK_EQUAL( 1, v3_dyn[2]);

  v1_dyn -= v2_dyn;

  BOOST_CHECK_EQUAL( 2, v1_dyn[0]);
  BOOST_CHECK_EQUAL(-4, v1_dyn[1]);
  BOOST_CHECK_EQUAL( 1, v1_dyn[2]);
}

/// Producto

BOOST_FIXTURE_TEST_CASE(multiplication, VectorTest)
{
  Vector<int, 3> v1 = {1, 0, 3};
  Vector<int, 3> v2 = {-1, 4, 2};
  Vector<int, 3> v3 = v1 * v2;

  BOOST_CHECK_EQUAL(-1, v3[0]);
  BOOST_CHECK_EQUAL( 0, v3[1]);
  BOOST_CHECK_EQUAL( 6, v3[2]);

  v1 *= v2;

  BOOST_CHECK_EQUAL(-1, v1[0]);
  BOOST_CHECK_EQUAL( 0, v1[1]);
  BOOST_CHECK_EQUAL( 6, v1[2]);

  Vector<int> v1_dyn = {1, 0, 3};
  Vector<int> v2_dyn = {-1, 4, 2};
  Vector<int> v3_dyn = v1_dyn * v2_dyn;

  BOOST_CHECK_EQUAL(-1, v3_dyn[0]);
  BOOST_CHECK_EQUAL( 0, v3_dyn[1]);
  BOOST_CHECK_EQUAL( 6, v3_dyn[2]);

  v1_dyn *= v2_dyn;

  BOOST_CHECK_EQUAL(-1, v1_dyn[0]);
  BOOST_CHECK_EQUAL( 0, v1_dyn[1]);
  BOOST_CHECK_EQUAL( 6, v1_dyn[2]);
}

/// División

BOOST_FIXTURE_TEST_CASE(division, VectorTest)
{
  Vector<double, 3> v1 = {1., 0., 3.};
  Vector<double, 3> v2 = {-1., 4., 2.};
  Vector<double, 3> v3 = v1 / v2;

  BOOST_CHECK_EQUAL(-1., v3[0]);
  BOOST_CHECK_EQUAL( 0., v3[1]);
  BOOST_CHECK_EQUAL( 1.5, v3[2]);

  v1 /= v2;

  BOOST_CHECK_EQUAL(-1., v1[0]);
  BOOST_CHECK_EQUAL( 0., v1[1]);
  BOOST_CHECK_EQUAL( 1.5, v1[2]);

  Vector<double> v1_dyn = {1., 0., 3.};
  Vector<double> v2_dyn = {-1., 4., 2.};
  Vector<double> v3_dyn = v1_dyn / v2_dyn;

  BOOST_CHECK_EQUAL(-1, v3_dyn[0]);
  BOOST_CHECK_EQUAL( 0, v3_dyn[1]);
  BOOST_CHECK_EQUAL( 1.5, v3_dyn[2]);

  v1_dyn /= v2_dyn;

  BOOST_CHECK_EQUAL(-1, v1_dyn[0]);
  BOOST_CHECK_EQUAL( 0, v1_dyn[1]);
  BOOST_CHECK_EQUAL( 1.5, v1_dyn[2]);
}


BOOST_FIXTURE_TEST_CASE(VectorScalar, VectorTest)
{
  Vector<int, 3> v1 = {1, 0, 3};
  Vector<int, 3> v2 = v1 * 10;

  BOOST_CHECK_EQUAL(10, v2[0]);
  BOOST_CHECK_EQUAL( 0, v2[1]);
  BOOST_CHECK_EQUAL(30, v2[2]);

  v1 *= 10;

  BOOST_CHECK_EQUAL(10, v1[0]);
  BOOST_CHECK_EQUAL( 0, v1[1]);
  BOOST_CHECK_EQUAL(30, v1[2]);


  Vector<int> v1_dyn = {1, 0, 3};
  Vector<int> v2_dyn = v1_dyn * 10;

  BOOST_CHECK_EQUAL(10, v2_dyn[0]);
  BOOST_CHECK_EQUAL( 0, v2_dyn[1]);
  BOOST_CHECK_EQUAL(30, v2_dyn[2]);

  v1_dyn *= 10;

  BOOST_CHECK_EQUAL(10, v1_dyn[0]);
  BOOST_CHECK_EQUAL( 0, v1_dyn[1]);
  BOOST_CHECK_EQUAL(30, v1_dyn[2]);
}


BOOST_FIXTURE_TEST_CASE(ScalarVector, VectorTest)
{
  Vector<int, 3> v1 = {1, 0, 3};
  Vector<int, 3> v2 = 10 * v1;

  BOOST_CHECK_EQUAL(10, v2[0]);
  BOOST_CHECK_EQUAL( 0, v2[1]);
  BOOST_CHECK_EQUAL(30, v2[2]);

  Vector<int> v1_dyn = {1, 0, 3};
  Vector<int> v2_dyn = 10 * v1_dyn;

  BOOST_CHECK_EQUAL(10, v2_dyn[0]);
  BOOST_CHECK_EQUAL( 0, v2_dyn[1]);
  BOOST_CHECK_EQUAL(30, v2_dyn[2]);
}

BOOST_FIXTURE_TEST_CASE(dotProduct, VectorTest)
{

}

BOOST_AUTO_TEST_SUITE_END()