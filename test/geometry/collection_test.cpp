/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop Geometry Collection test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/base/GeometryCollection.h>
#include <tidop/geometry/io/wkt/Proxy.h>
#include <tidop/geometry/primitives/LineString.h>
#include <tidop/geometry/primitives/Polygon.h>
#include <tidop/geometry/primitives/MultiPoint.h>
#include <tidop/geometry/primitives/MultiLineString.h>
#include <tidop/geometry/primitives/MultiPolygon.h>

using namespace tl;


struct GeometryCollectionFixture
{
    GeometryCollectionFixture()
    {
        // Crear algunos puntos de prueba
        p1 = Point2d(1.0, 2.0);
        p2 = Point2d(3.0, 4.0);
        p3 = Point2d(5.0, 6.0);
        p4 = Point2d(7.0, 8.0);

        // Crear una LineString de prueba
        linePoints = {Point2d(0.0, 0.0), Point2d(1.0, 1.0), Point2d(2.0, 0.0)};
        lineString = LineString<Point2d>(linePoints);

        // Crear un Polygon de prueba (triángulo)
        ringPoints = {Point2d(0.0, 0.0), Point2d(4.0, 0.0), Point2d(2.0, 3.0), Point2d(0.0, 0.0)};
        polygon = tl::Polygon<Point2d>({ringPoints});

        // Crear un MultiPoint de prueba
        multiPointPoints = {Point2d(0.0, 0.0), Point2d(1.0, 1.0), Point2d(2.0, 2.0)};
        multiPoint = MultiPoint<Point2d>(multiPointPoints);

        // Crear un MultiLineString de prueba
        line1 = LineString<Point2d>({Point2d(0.0, 0.0), Point2d(1.0, 1.0)});
        line2 = LineString<Point2d>({Point2d(2.0, 2.0), Point2d(3.0, 3.0)});
        multiLineString = MultiLineString<Point2d>({line1, line2});

        // Crear un MultiPolygon de prueba
        poly1 = tl::Polygon<Point2d>({
            std::vector<Point2d>{
                Point2d(0.0, 0.0), 
                Point2d(1.0, 0.0),                              
                Point2d(0.5, 1.0), 
                Point2d(0.0, 0.0)}
            });

        poly2 = tl::Polygon<Point2d>({
            std::vector<Point2d>{
                Point2d(2.0, 2.0),
                Point2d(3.0, 2.0),                          
                Point2d(2.5, 3.0), 
                Point2d(2.0, 2.0)}
            });

        multiPolygon = MultiPolygon<Point2d>({poly1, poly2});
    }

    Point2d p1, p2, p3, p4;
    std::vector<Point2d> linePoints, ringPoints, multiPointPoints;
    LineString<Point2d> lineString, line1, line2;
    tl::Polygon<Point2d> polygon, poly1, poly2;
    MultiPoint<Point2d> multiPoint;
    MultiLineString<Point2d> multiLineString;
    MultiPolygon<Point2d> multiPolygon;
};

BOOST_AUTO_TEST_SUITE(GeometryCollectionTestSuite)

// Test 1: Constructor y estado inicial
BOOST_AUTO_TEST_CASE(test_constructor_and_initial_state)
{
    GeometryCollection<Point2d> collection;

    BOOST_CHECK(collection.empty());
    BOOST_CHECK_EQUAL(0, collection.size());
    BOOST_CHECK(!collection.hasPoints());
    BOOST_CHECK_EQUAL(collection.pointCount(), 0);
    BOOST_CHECK(!collection.hasLineStrings());
    BOOST_CHECK_EQUAL(collection.lineStringCount(), 0);
    BOOST_CHECK(!collection.hasPolygons());
    BOOST_CHECK_EQUAL(collection.polygonCount(), 0);
    BOOST_CHECK(!collection.hasMultiPoints());
    BOOST_CHECK_EQUAL(collection.multiPointCount(), 0);
    BOOST_CHECK(!collection.hasMultiLineStrings());
    BOOST_CHECK_EQUAL(collection.multiLineStringCount(), 0);
    BOOST_CHECK(!collection.hasMultiPolygons());
    BOOST_CHECK_EQUAL(collection.multiPolygonCount(), 0);
    BOOST_CHECK(!collection.hasGeometryCollections());
    BOOST_CHECK_EQUAL(collection.geometryCollectionCount(), 0);
}

// Test 2: Añadir puntos (const y move)
BOOST_FIXTURE_TEST_CASE(test_add_point, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    // Añadir punto const
    collection.addPoint(p1);
    BOOST_CHECK_EQUAL(1, collection.size());
    BOOST_CHECK_EQUAL(1, collection.points().size());
    BOOST_CHECK_EQUAL(1, collection.pointCount());
    BOOST_CHECK(collection.points()[0] == p1);
    BOOST_CHECK(collection.hasPoints());

    // Añadir punto move
    Point2d tempPoint(9.0, 10.0);
    collection.addPoint(std::move(tempPoint));
    BOOST_CHECK_EQUAL(2, collection.size());
    BOOST_CHECK_EQUAL(2, collection.points().size());
    BOOST_CHECK_EQUAL(2, collection.pointCount());
    BOOST_CHECK(collection.points()[1] == Point2d(9.0, 10.0));
}

// Test 3: Añadir LineString
BOOST_FIXTURE_TEST_CASE(test_add_linestring, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    // Añadir LineString const
    collection.addLineString(lineString);
    BOOST_CHECK_EQUAL(1,collection.size());
    BOOST_CHECK_EQUAL(1, collection.lineStrings().size());
    BOOST_CHECK_EQUAL(1, collection.lineStringCount());
    BOOST_CHECK_EQUAL(3, collection.lineStrings()[0].size());
    BOOST_CHECK_EQUAL(3, collection.lineStringAt(0).size());
    BOOST_CHECK(collection.hasLineStrings());

    // Añadir LineString move
    LineString<Point2d> tempLine({Point2d(0.0, 0.0), Point2d(10.0, 10.0)});
    collection.addLineString(std::move(tempLine));
    BOOST_CHECK_EQUAL(2, collection.size());
    BOOST_CHECK_EQUAL(2, collection.lineStrings().size());
    BOOST_CHECK_EQUAL(2, collection.lineStringCount());
}

// Test 4: Añadir Polygon
BOOST_FIXTURE_TEST_CASE(test_add_polygon, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addPolygon(polygon);
    BOOST_CHECK_EQUAL(1, collection.size());
    BOOST_CHECK_EQUAL(1, collection.polygons().size());
    BOOST_CHECK_EQUAL(1, collection.polygonCount());
    BOOST_CHECK_EQUAL(4, collection.polygons()[0].outer().size());
    BOOST_CHECK_EQUAL(4, collection.polygonAt(0).outer().size());
    BOOST_CHECK(collection.hasPolygons());
}

// Test 5: Añadir MultiPoint
BOOST_FIXTURE_TEST_CASE(test_add_multipoint, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addMultiPoint(multiPoint);
    BOOST_CHECK_EQUAL(1, collection.size());
    BOOST_CHECK_EQUAL(1, collection.multiPoints().size());
    BOOST_CHECK_EQUAL(1, collection.multiPointCount());
    BOOST_CHECK_EQUAL(3, collection.multiPoints()[0].size());
    BOOST_CHECK_EQUAL(3, collection.multiPointAt(0).size());
    BOOST_CHECK(collection.hasMultiPoints());
}

// Test 6: Añadir MultiLineString
BOOST_FIXTURE_TEST_CASE(test_add_multilinestring, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addMultiLineString(multiLineString);
    BOOST_CHECK_EQUAL(1, collection.size());
    BOOST_CHECK_EQUAL(1, collection.multiLineStrings().size());
    BOOST_CHECK_EQUAL(1, collection.multiLineStringCount());
    BOOST_CHECK_EQUAL(2, collection.multiLineStrings()[0].size());
    BOOST_CHECK_EQUAL(2, collection.multiLineStringAt(0).size());
    BOOST_CHECK(collection.hasMultiLineStrings());
}

// Test 7: Añadir MultiPolygon
BOOST_FIXTURE_TEST_CASE(test_add_multipolygon, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addMultiPolygon(multiPolygon);
    BOOST_CHECK_EQUAL(1, collection.size());
    BOOST_CHECK_EQUAL(1, collection.multiPolygons().size());
    BOOST_CHECK_EQUAL(1, collection.multiPolygonCount());
    BOOST_CHECK_EQUAL(2, collection.multiPolygons()[0].size());
    BOOST_CHECK_EQUAL(2, collection.multiPolygonAt(0).size());
    BOOST_CHECK(collection.hasMultiPolygons());
}

// Test 8: Añadir GeometryCollection (colecciones anidadas)
BOOST_FIXTURE_TEST_CASE(test_add_geometrycollection, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> innerCollection;
    innerCollection.addPoint(p1);
    innerCollection.addLineString(lineString);

    GeometryCollection<Point2d> outerCollection;
    outerCollection.addGeometryCollection(innerCollection);

    BOOST_CHECK_EQUAL(1, outerCollection.size());
    BOOST_CHECK_EQUAL(1, outerCollection.geometryCollections().size());
    BOOST_CHECK_EQUAL(1, outerCollection.geometryCollectionCount());    
    BOOST_CHECK_EQUAL(2, outerCollection.geometryCollections()[0].size());
    BOOST_CHECK_EQUAL(2, outerCollection.geometryCollectionAt(0).size());
    BOOST_CHECK(outerCollection.hasGeometryCollections());
}

// Test 9: Orden de inserción preservado
BOOST_FIXTURE_TEST_CASE(test_insertion_order, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    // Añadir en orden específico
    collection.addPoint(p1);               // Índice 0
    collection.addLineString(lineString);  // Índice 1
    collection.addPolygon(polygon);        // Índice 2
    collection.addMultiPoint(multiPoint);  // Índice 3
    collection.add<Point2d>(p2);           // Índice 4

    BOOST_CHECK_EQUAL(collection.size(), 5);

    // Verificar que operator[] devuelve en el orden correcto
    auto geo0 = collection[0];
    auto geo1 = collection[1];
    auto geo2 = collection[2];
    auto geo3 = collection[3];
    auto geo4 = collection[4];

    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<Point2d>>(geo0));
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<LineString<Point2d>>>(geo1));
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<tl::Polygon<Point2d>>>(geo2));
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<MultiPoint<Point2d>>>(geo3));
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<Point2d>>(geo4));
}

// Test 10: Operador [] con acceso a datos
BOOST_FIXTURE_TEST_CASE(test_operator_access, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;
    collection.addPoint(p1);
    collection.addLineString(lineString);

    // Acceder y modificar punto
    auto geo0 = collection[0];
    auto &pointRef = std::get<std::reference_wrapper<Point2d>>(geo0).get();
    BOOST_CHECK_EQUAL(pointRef.x(), 1.0);
    BOOST_CHECK_EQUAL(pointRef.y(), 2.0);

    // Modificar el punto a través de la referencia
    pointRef = Point2d(100.0, 200.0);
    BOOST_CHECK_EQUAL(collection.points()[0].x(), 100.0);

    // Acceder a LineString
    auto geo1 = collection[1];
    const auto &lineRef = std::get<std::reference_wrapper<LineString<Point2d>>>(geo1).get();
    BOOST_CHECK_EQUAL(lineRef.size(), 3);
}

// Test 11: Operador [] const
BOOST_FIXTURE_TEST_CASE(test_const_operator_access, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;
    collection.addPoint(p1);
    collection.addLineString(lineString);

    const auto &constCollection = collection;

    // Acceso const funciona
    auto geo0 = constCollection[0];
    const auto &pointRef = std::get<std::reference_wrapper<const Point2d>>(geo0).get();
    BOOST_CHECK_EQUAL(pointRef.x(), 1.0);
}

// Test 12: Métodos de acceso a vectores
BOOST_FIXTURE_TEST_CASE(test_vector_access_methods, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addPoint(p1);
    collection.addPoint(p2);
    collection.addLineString(lineString);
    collection.addPolygon(polygon);

    // Verificar que los métodos devuelven referencias a los vectores
    BOOST_CHECK_EQUAL(collection.points().size(), 2);
    BOOST_CHECK_EQUAL(collection.lineStrings().size(), 1);
    BOOST_CHECK_EQUAL(collection.polygons().size(), 1);

    collection.addPoint(p3);
    BOOST_CHECK_EQUAL(collection.points().size(), 3);
    BOOST_CHECK_EQUAL(collection.size(), 5);

    // Podemos modificar puntos existentes
    collection.pointAt(0) = Point2d(100.0, 200.0);
    BOOST_CHECK(collection.pointAt(0) == Point2d(100.0, 200.0));

    // Versiones const
    const auto &constCollection = collection;
    BOOST_CHECK_EQUAL(constCollection.points().size(), 3);
}

// Test 13: Método clear
BOOST_FIXTURE_TEST_CASE(test_clear, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    // Llenar la colección
    collection.addPoint(p1);
    collection.addLineString(lineString);
    collection.addPolygon(polygon);

    BOOST_CHECK(!collection.empty());
    BOOST_CHECK_EQUAL(collection.size(), 3);

    // Limpiar
    collection.clear();

    BOOST_CHECK(collection.empty());
    BOOST_CHECK_EQUAL(collection.size(), 0);
    BOOST_CHECK(collection.points().empty());
    BOOST_CHECK(collection.lineStrings().empty());
    BOOST_CHECK(collection.polygons().empty());
}

// Test 14: Colección heterogénea mixta
BOOST_FIXTURE_TEST_CASE(test_mixed_collection, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    // Añadir uno de cada tipo
    collection.addPoint(p1);
    collection.addLineString(lineString);
    collection.addPolygon(polygon);
    collection.addMultiPoint(multiPoint);
    collection.addMultiLineString(multiLineString);
    collection.addMultiPolygon(multiPolygon);

    // Añadir colección anidada
    GeometryCollection<Point2d> innerCollection;
    innerCollection.addPoint(p2);
    collection.addGeometryCollection(innerCollection);

    BOOST_CHECK_EQUAL(collection.size(), 7);
    BOOST_CHECK_EQUAL(collection.points().size(), 1);
    BOOST_CHECK_EQUAL(collection.lineStrings().size(), 1);
    BOOST_CHECK_EQUAL(collection.polygons().size(), 1);
    BOOST_CHECK_EQUAL(collection.multiPoints().size(), 1);
    BOOST_CHECK_EQUAL(collection.multiLineStrings().size(), 1);
    BOOST_CHECK_EQUAL(collection.multiPolygons().size(), 1);
    BOOST_CHECK_EQUAL(collection.geometryCollections().size(), 1);
}

// Test 15: Iteración sobre la colección
BOOST_FIXTURE_TEST_CASE(test_iteration, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addPoint(p1);
    collection.addLineString(lineString);
    collection.addPolygon(polygon);

    // Contar tipos durante la iteración
    int pointCount = 0, lineCount = 0, polygonCount = 0;

    for (size_t i = 0; i < collection.size(); ++i) {

        auto geometry = collection[i];
        std::visit([&](auto &&arg) {
            using T = std::decay_t<decltype(arg.get())>;
            if constexpr (std::is_same_v<T, Point2d>) {
                ++pointCount;
            } else if constexpr (std::is_same_v<T, LineString<Point2d>>) {
                ++lineCount;
            } else if constexpr (std::is_same_v<T, tl::Polygon<Point2d>>) {
                ++polygonCount;
            }
            }, geometry);
    }

    BOOST_TEST(pointCount == 1);
    BOOST_TEST(lineCount == 1);
    BOOST_TEST(polygonCount == 1);
}

// Test 16: Manejo de excepciones
//BOOST_FIXTURE_TEST_CASE(test_exceptions, GeometryCollectionFixture)
//{
//    GeometryCollection<Point2d> collection;
//
//    // Acceder a índice fuera de rango debería lanzar excepción
//    BOOST_CHECK_THROW(collection[0], std::runtime_error);
//
//    // Añadir un elemento y acceder fuera de rango
//    collection.addPoint(p1);
//    BOOST_CHECK_NO_THROW(collection[0]);
//    BOOST_CHECK_THROW(collection[1], std::runtime_error);
//
//    // Verificar que mOrder esté sincronizado
//    collection.addLineString(lineString);
//    collection.addPolygon(polygon);
//
//    // Manipular mOrder directamente para crear estado inválido
//    // (esto simularía un bug interno)
//    // collection.mOrder.push_back({GeometryType::point, 999}); // No debería ser posible desde fuera
//
//    // Intentar acceder con índice inválido en mOrder
//    // (esto se protegería en operator[])
//}

// Test 17: Copia y movimiento
BOOST_FIXTURE_TEST_CASE(test_copy_and_move, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> original;
    original.addPoint(p1);
    original.addLineString(lineString);
    original.addPolygon(polygon);

    // Test de copia
    GeometryCollection<Point2d> copy = original;
    BOOST_CHECK_EQUAL(copy.size(), 3);
    BOOST_CHECK_EQUAL(copy.points().size(), 1);
    BOOST_CHECK_EQUAL(copy.lineStrings().size(), 1);
    BOOST_CHECK_EQUAL(copy.polygons().size(), 1);

    // Test de movimiento
    GeometryCollection<Point2d> moved = std::move(original);
    BOOST_CHECK_EQUAL(moved.size(), 3);
    BOOST_CHECK(original.empty()); // Original debería estar vacío después del movimiento
}

// Test 18: Colecciones anidadas profundas
BOOST_FIXTURE_TEST_CASE(test_deeply_nested_collections, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> level3;
    level3.addPoint(p1);

    GeometryCollection<Point2d> level2;
    level2.addGeometryCollection(level3);
    level2.addLineString(lineString);

    GeometryCollection<Point2d> level1;
    level1.addGeometryCollection(level2);
    level1.addPolygon(polygon);

    BOOST_CHECK_EQUAL(level1.size(), 2);
    BOOST_CHECK_EQUAL(level1.geometryCollections().size(), 1);

    // Acceder a elementos anidados
    auto geo0 = level1[0];
    auto &nestedRef = std::get<std::reference_wrapper<GeometryCollection<Point2d>>>(geo0).get();
    BOOST_CHECK_EQUAL(nestedRef.size(), 2);
}

// Test 19: Traits de GeometryCollection
BOOST_FIXTURE_TEST_CASE(test_geometry_traits, GeometryCollectionFixture)
{
    using Traits = geometry_traits<GeometryCollection<Point2d>>;

    BOOST_CHECK(Traits::is_geometry);
    BOOST_CHECK(Traits::is_multi); // GeometryCollection es multi
    BOOST_CHECK(Traits::type == GeometryType::collection);
    BOOST_CHECK(Traits::dimension == geometry_traits<Point2d>::dimension);

    // Verificar value_type
    BOOST_CHECK((std::is_same_v<Traits::point_type, Point2d>));
}

// Test 20: Rendimiento/estrés con muchas geometrías
BOOST_FIXTURE_TEST_CASE(test_stress, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;
    const size_t NUM_GEOMETRIES = 1000;

    // Añadir muchas geometrías de diferentes tipos
    for (size_t i = 0; i < NUM_GEOMETRIES; ++i) {
        if (i % 7 == 0) {
            collection.addPoint(Point2d(i * 1.0, i * 2.0));
        } else if (i % 7 == 1) {
            collection.addLineString(LineString<Point2d>({Point2d(0.0, 0.0), Point2d(1.0, 1.0)}));
        } else if (i % 7 == 2) {
            collection.addPolygon(polygon);
        } else if (i % 7 == 3) {
            collection.addMultiPoint(multiPoint);
        } else if (i % 7 == 4) {
            collection.addMultiLineString(multiLineString);
        } else if (i % 7 == 5) {
            collection.addMultiPolygon(multiPolygon);
        } else {
            GeometryCollection<Point2d> inner;
            inner.addPoint(p1);
            collection.addGeometryCollection(inner);
        }
    }

    BOOST_CHECK_EQUAL(collection.size(), NUM_GEOMETRIES);

    // Verificar que todos los elementos son accesibles
    for (size_t i = 0; i < collection.size(); ++i) {
        BOOST_CHECK_NO_THROW(collection[i]);
    }
}

// Test específico para verificar la sincronización de mOrder
BOOST_FIXTURE_TEST_CASE(test_morder_synchronization, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    // Añadir geometrías en un orden específico
    collection.addPoint(p1);                                  // mOrder[0] = {point, 0}
    collection.addLineString(lineString);                     // mOrder[1] = {linestring, 0}
    collection.addPoint(p2);                                  // mOrder[2] = {point, 1}
    collection.addLineString(LineString<Point2d>({p3, p4}));  // mOrder[3] = {linestring, 1}
    collection.addPolygon(polygon);                           // mOrder[4] = {polygon, 0}

    // Verificar índices en mOrder
    BOOST_CHECK_EQUAL(collection.size(), 5);

    // Verificar que cada entrada en mOrder apunta al índice correcto
    // Nota: Esto requeriría acceso a mOrder, que es privado.
    // Podemos verificar indirectamente a través de operator[]

    // Punto en índice 0
    auto geo0 = collection[0];
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<Point2d>>(geo0));
    BOOST_CHECK(std::get<std::reference_wrapper<Point2d>>(geo0).get() == p1);

    // LineString en índice 1
    auto geo1 = collection[1];
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<LineString<Point2d>>>(geo1));

    // Segundo punto en índice 2
    auto geo2 = collection[2];
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<Point2d>>(geo2));
    BOOST_CHECK(std::get<std::reference_wrapper<Point2d>>(geo2).get() == p2);

    // Verificar que clear() limpia mOrder
    collection.clear();
    BOOST_CHECK(collection.empty());
    //BOOST_CHECK_THROW(collection[0], std::runtime_error);
}

// Test para removePoint
BOOST_FIXTURE_TEST_CASE(test_remove_point, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addPoint(p1);
    collection.addPoint(p2);
    collection.addPoint(p3);

    BOOST_TEST(collection.size() == 3);
    BOOST_TEST(collection.points().size() == 3);

    // Eliminar el punto en la posición 1 (p2)
    bool removed = collection.removePoint(1);
    BOOST_TEST(removed == true);
    BOOST_TEST(collection.size() == 2);
    BOOST_TEST(collection.points().size() == 2);

    // Verificar que los puntos restantes son p1 y p3
    BOOST_CHECK(collection.pointAt(0) == p1);
    BOOST_CHECK(collection.pointAt(1) == p3);

    // Eliminar un índice inválido
    BOOST_TEST(collection.removePoint(10) == false);
    BOOST_TEST(collection.size() == 2); // Sin cambios
}

// Test para removeLineString
BOOST_FIXTURE_TEST_CASE(test_remove_linestring, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addLineString(lineString);
    collection.addPoint(p1);
    collection.addLineString(LineString<Point2d>({p2, p3}));

    BOOST_TEST(collection.size() == 3);
    BOOST_TEST(collection.lineStrings().size() == 2);

    // Eliminar la primera LineString
    bool removed = collection.removeLineString(0);
    BOOST_TEST(removed == true);
    BOOST_TEST(collection.size() == 2);
    BOOST_TEST(collection.lineStrings().size() == 1);

    // Verificar que la línea restante es la segunda
    BOOST_TEST(collection.lineStringAt(0).size() == 2);
    BOOST_CHECK(collection.lineStringAt(0)[0] == p2);
    BOOST_CHECK(collection.lineStringAt(0)[1] == p3);

    // Verificar que el punto sigue ahí
    BOOST_TEST(collection.points().size() == 1);
    BOOST_CHECK(collection.pointAt(0) == p1);
}

// Test para removePolygon
BOOST_FIXTURE_TEST_CASE(test_remove_polygon, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addPolygon(polygon);
    collection.addPoint(p1);
    collection.addPolygon(poly1);

    BOOST_TEST(collection.size() == 3);
    BOOST_TEST(collection.polygons().size() == 2);

    // Eliminar el primer polígono
    bool removed = collection.removePolygon(0);
    BOOST_TEST(removed == true);
    BOOST_TEST(collection.size() == 2);
    BOOST_TEST(collection.polygons().size() == 1);

    // Verificar que el polígono restante es poly1
    BOOST_TEST(collection.polygonAt(0).outer().size() == 4);
    BOOST_CHECK(collection.polygonAt(0).outer()[0] == Point2d(0.0, 0.0));
}

// Test para removeMultiPoint
BOOST_FIXTURE_TEST_CASE(test_remove_multipoint, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addMultiPoint(multiPoint);
    collection.addPoint(p1);

    BOOST_TEST(collection.size() == 2);
    BOOST_TEST(collection.multiPoints().size() == 1);

    bool removed = collection.removeMultiPoint(0);
    BOOST_TEST(removed == true);
    BOOST_TEST(collection.size() == 1);
    BOOST_TEST(collection.multiPoints().size() == 0);
    BOOST_TEST(collection.points().size() == 1);
}

// Test para removeMultiLineString
BOOST_FIXTURE_TEST_CASE(test_remove_multilinestring, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addMultiLineString(multiLineString);
    collection.addLineString(lineString);

    BOOST_TEST(collection.size() == 2);
    BOOST_TEST(collection.multiLineStrings().size() == 1);

    bool removed = collection.removeMultiLineString(0);
    BOOST_TEST(removed == true);
    BOOST_TEST(collection.size() == 1);
    BOOST_TEST(collection.multiLineStrings().size() == 0);
    BOOST_TEST(collection.lineStrings().size() == 1);
}

// Test para removeMultiPolygon
BOOST_FIXTURE_TEST_CASE(test_remove_multipolygon, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addMultiPolygon(multiPolygon);
    collection.addPolygon(polygon);

    BOOST_TEST(collection.size() == 2);
    BOOST_TEST(collection.multiPolygons().size() == 1);

    bool removed = collection.removeMultiPolygon(0);
    BOOST_TEST(removed == true);
    BOOST_TEST(collection.size() == 1);
    BOOST_TEST(collection.multiPolygons().size() == 0);
    BOOST_TEST(collection.polygons().size() == 1);
}

// Test para removeGeometryCollection
BOOST_FIXTURE_TEST_CASE(test_remove_geometrycollection, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> innerCollection;
    innerCollection.addPoint(p1);
    innerCollection.addLineString(lineString);

    GeometryCollection<Point2d> collection;
    collection.addGeometryCollection(innerCollection);
    collection.addPoint(p2);

    BOOST_TEST(collection.size() == 2);
    BOOST_TEST(collection.geometryCollections().size() == 1);

    bool removed = collection.removeGeometryCollection(0);
    BOOST_TEST(removed == true);
    BOOST_TEST(collection.size() == 1);
    BOOST_TEST(collection.geometryCollections().size() == 0);
    BOOST_TEST(collection.points().size() == 1);
}

// Test para removeAt (eliminación por índice en la colección)
BOOST_FIXTURE_TEST_CASE(test_remove_at, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    // Añadir en orden específico
    collection.addPoint(p1);           // Índice 0 en colección
    collection.addLineString(lineString); // Índice 1 en colección
    collection.addPolygon(polygon);    // Índice 2 en colección

    BOOST_TEST(collection.size() == 3);

    // Eliminar la línea (índice 1)
    bool removed = collection.removeAt(1);
    BOOST_TEST(removed == true);
    BOOST_TEST(collection.size() == 2);
    BOOST_TEST(collection.lineStrings().size() == 0);
    BOOST_TEST(collection.points().size() == 1);
    BOOST_TEST(collection.polygons().size() == 1);

    // Verificar orden restante
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<Point2d>>(collection[0]));
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<tl::Polygon<Point2d>>>(collection[1]));

    // Eliminar con índice inválido
    BOOST_TEST(collection.removeAt(10) == false);
    BOOST_TEST(collection.size() == 2); // Sin cambios
}

// Test de consistencia después de eliminaciones múltiples
BOOST_FIXTURE_TEST_CASE(test_multiple_removals_consistency, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    // Añadir varios elementos de diferentes tipos
    collection.addPoint(p1);              // 0: Punto
    collection.addLineString(lineString); // 1: Línea
    collection.addPoint(p2);              // 2: Punto
    collection.addPolygon(polygon);       // 3: Polígono
    collection.addPoint(p3);              // 4: Punto
    collection.addLineString(line1);      // 5: Línea
    collection.addPoint(p4);              // 6: Punto

    BOOST_TEST(collection.size() == 7);

    // Eliminar varios elementos
    bool is_removed = collection.removePoint(1); // Elimina p2 (índice 1 en puntos, pero es el punto en posición 2 en colección)
    BOOST_TEST(collection.size() == 6);
    BOOST_TEST(is_removed);

    collection.removeLineString(0); // Elimina la primera línea
    BOOST_TEST(collection.size() == 5);

    collection.removeAt(1); // Elimina el polígono
    BOOST_TEST(collection.size() == 4);

    // Verificar que la colección sigue siendo consistente
    BOOST_TEST(collection.points().size() == 3); // p1, p3, p4
    BOOST_TEST(collection.lineStrings().size() == 1); // line1
    BOOST_TEST(collection.polygons().size() == 0);

    // Verificar que todos los elementos son accesibles
    for (size_t i = 0; i < collection.size(); ++i) {
        BOOST_CHECK_NO_THROW(collection[i]);
    }

    // Verificar contenido
    BOOST_CHECK(collection.pointAt(0) == p1);
    BOOST_CHECK(collection.pointAt(1) == p3);
    BOOST_CHECK(collection.pointAt(2) == p4);
    BOOST_CHECK(collection.lineStringAt(0) == line1);
}

// Test de eliminación en colecciones anidadas
BOOST_FIXTURE_TEST_CASE(test_nested_collection_removal, GeometryCollectionFixture)
{
    // Crear una colección anidada
    GeometryCollection<Point2d> level3;
    level3.addPoint(p1);

    GeometryCollection<Point2d> level2;
    level2.addGeometryCollection(level3);
    level2.addLineString(lineString);

    GeometryCollection<Point2d> level1;
    level1.addGeometryCollection(level2);
    level1.addPolygon(polygon);

    BOOST_TEST(level1.size() == 2);

    // Eliminar la colección anidada
    bool removed = level1.removeGeometryCollection(0);
    BOOST_TEST(removed == true);
    BOOST_TEST(level1.size() == 1);
    BOOST_TEST(level1.geometryCollections().size() == 0);
    BOOST_TEST(level1.polygons().size() == 1);
}

// Test de limpieza completa después de eliminaciones
BOOST_FIXTURE_TEST_CASE(test_clear_after_removals, GeometryCollectionFixture)
{
    GeometryCollection<Point2d> collection;

    collection.addPoint(p1);
    collection.addLineString(lineString);
    collection.addPolygon(polygon);

    // Eliminar todo uno por uno
    collection.removePoint(0);
    collection.removeLineString(0);
    collection.removePolygon(0);

    BOOST_TEST(collection.empty());
    BOOST_TEST(collection.size() == 0);
    BOOST_TEST(collection.points().empty());
    BOOST_TEST(collection.lineStrings().empty());
    BOOST_TEST(collection.polygons().empty());
    BOOST_TEST(!collection.hasPoints());
    BOOST_TEST(!collection.hasLineStrings());
    BOOST_TEST(!collection.hasPolygons());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(test_geometry_collection_complex_wkt)
{
    GeometryCollection<Point3dm> collection;

    // 1. Añadimos un Punto
    collection.addPoint(Point3dm(1, 2, 3, 4));

    // 2. Añadimos una Línea
    LineString<Point3dm> line = {
        Point3dm(0, 0, 0, 0),
        Point3dm(1, 1, 1, 1)
    };
    collection.addLineString(line);

    std::stringstream ss;
    ss << wkt(collection);
    std::string result = ss.str();

    // Verificaciones
    BOOST_CHECK(result.find("GEOMETRYCOLLECTION ZM") != std::string::npos);
    BOOST_CHECK(result.find("POINT ZM (1 2 3 4)") != std::string::npos);
    BOOST_CHECK(result.find("LINESTRING ZM (0 0 0 0, 1 1 1 1)") != std::string::npos);
    
    // Verificar que el acceso por índice funciona (retorna variant)
    auto item = collection[0];
    BOOST_CHECK(std::holds_alternative<std::reference_wrapper<Point3dm>>(item));

    if (const auto val = std::get_if<std::reference_wrapper<Point3dm>>(&item)) {
        const Point3dm p = val->get();
        BOOST_CHECK_EQUAL(p.x(), 1.0);
    }
}