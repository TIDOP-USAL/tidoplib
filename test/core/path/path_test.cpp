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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 

#define BOOST_TEST_MODULE Tidop path test
#include <boost/test/unit_test.hpp>
#include <tidop/core/path.h>


using namespace tl;

BOOST_AUTO_TEST_SUITE(PathTestSuite)


struct PathTest
{
  PathTest()
#if defined WIN32
    : path(new Path("D:\\Desarrollo\\Libs"))
#else
    : path(new Path("/home/user/Libs"))
#endif
  {
  }

  ~PathTest()
  {
    if (path) {
      delete path;
      path = nullptr;
    }
  }

  void setup()
  {
#if defined WIN32
    path_file.setPath("C:\\temp\\file.txt");
    path2.setPath("C:\\temp\\áñ.txt");
#else
    path_file.setPath("/home/user/file.txt");
    path2.setPath("/home/user/áñ.txt");
#endif
  }

  void teardown()
  {

  }

  Path *path;
  Path path_empty;
  Path path_file;
  Path path2;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, PathTest)
{
  BOOST_CHECK_EQUAL(std::string{}, path_empty.toString());
  BOOST_CHECK_EQUAL(Path{}, path_empty.fileName());
  BOOST_CHECK_EQUAL(Path{}, path_empty.baseName());
  BOOST_CHECK_EQUAL(Path{}, path_empty.extension());
  BOOST_CHECK_EQUAL(true, path_empty.empty());
}

BOOST_FIXTURE_TEST_CASE(Constructor, PathTest)
{
#if defined WIN32
  BOOST_CHECK_EQUAL("D:\\Desarrollo\\Libs", path->toString());
#else
  BOOST_CHECK_EQUAL("/home/user/Libs", path->toString());
#endif
  BOOST_CHECK_EQUAL(Path("Libs"), path->fileName());
  BOOST_CHECK_EQUAL(Path("Libs"), path->baseName());
  BOOST_CHECK_EQUAL(Path(""), path->extension());
  BOOST_CHECK_EQUAL(false, path->empty());
}

BOOST_FIXTURE_TEST_CASE(set_path, PathTest)
{
#if defined WIN32
  BOOST_CHECK_EQUAL("C:\\temp\\file.txt", path_file.toString());
#else
  BOOST_CHECK_EQUAL("/home/user/file.txt", path_file.toString());
#endif
  BOOST_CHECK_EQUAL(Path("file.txt"), path_file.fileName());
  BOOST_CHECK_EQUAL(Path("file"), path_file.baseName());
  BOOST_CHECK_EQUAL(Path(".txt"), path_file.extension());
  BOOST_CHECK_EQUAL(false, path_file.empty());

#if defined WIN32
  BOOST_CHECK_EQUAL("C:\\temp\\áñ.txt", path2.toString());
#else
  BOOST_CHECK_EQUAL("/home/user/áñ.txt", path2.toString());
#endif
}

BOOST_FIXTURE_TEST_CASE(replace_extension, PathTest)
{
#if defined WIN32
  Path path("C:\\temp\\file.xyz");
#else
  Path path("/home/user/file.xyz");
#endif
  path.replaceExtension("csv");
#if defined WIN32
  BOOST_CHECK_EQUAL(Path("C:\\temp\\file.csv"), path.toString());
#else
  BOOST_CHECK_EQUAL(Path("/home/user/file.csv"), path.toString());
#endif
  BOOST_CHECK_EQUAL(Path("file.csv"), path.fileName());
  BOOST_CHECK_EQUAL(Path("file"), path.baseName());
  BOOST_CHECK_EQUAL(Path(".csv"), path.extension());
  BOOST_CHECK_EQUAL(false, path.empty());
}

BOOST_FIXTURE_TEST_CASE(replace_file_name, PathTest)
{
#if defined WIN32
  Path path("C:\\temp\\file.xyz");
  path.replaceFileName("file2.txt");
  BOOST_CHECK_EQUAL("C:\\temp\\file2.txt", path.toString());
#else
  Path path("/home/temp/file.xyz");
  path.replaceFileName("file2.txt");
  BOOST_CHECK_EQUAL("/home/temp/file2.txt", path.toString());
#endif
}

BOOST_FIXTURE_TEST_CASE(replace_base_name, PathTest)
{
#if defined WIN32
  Path path("C:\\temp\\file.xyz");
  path.replaceBaseName("file2");
  BOOST_CHECK_EQUAL("C:\\temp\\file2.xyz", path.toString());
#else
  Path path("/home/temp/file.xyz");
  path.replaceBaseName("file2");
  BOOST_CHECK_EQUAL("/home/temp/file2.xyz", path.toString());
#endif
}

BOOST_FIXTURE_TEST_CASE(parent_path, PathTest)
{
#if defined WIN32
  Path path("C:\\temp\\dir1\\dir2");
  Path path1 = path.parentPath();
  BOOST_CHECK_EQUAL("C:\\temp\\dir1", path1.toString());
  Path path2 = path1.parentPath();
  BOOST_CHECK_EQUAL("C:\\temp", path2.toString());
  Path path3 = path2.parentPath();
  BOOST_CHECK_EQUAL("C:\\", path3.toString());
#else
  Path path("/home/temp/dir1/dir2");
  Path path1 = path.parentPath();
  BOOST_CHECK_EQUAL("/home/temp/dir1", path1.toString());
  Path path2 = path1.parentPath();
  BOOST_CHECK_EQUAL("/home/temp", path2.toString());
  Path path3 = path2.parentPath();
  BOOST_CHECK_EQUAL("/home", path3.toString());
#endif
}

BOOST_FIXTURE_TEST_CASE(append, PathTest)
{
#if defined WIN32
  Path path("C:\\temp\\dir1");
  Path path1 = path.append("dir2");
  BOOST_CHECK_EQUAL("C:\\temp\\dir1\\dir2", path1.toString());
#else
  Path path("/home/temp/dir1");
  Path path1 = path.append("dir2");
  BOOST_CHECK_EQUAL("/home/temp/dir1/dir2", path1.toString());
#endif
}

BOOST_FIXTURE_TEST_CASE(clear, PathTest)
{
#if defined WIN32
  Path path("C:\\temp\\dir1");
#else
  Path path("/home/temp/dir1");
#endif
  BOOST_CHECK_EQUAL(false, path.empty());
  path.clear();
  BOOST_CHECK_EQUAL(true, path.empty());
}


BOOST_AUTO_TEST_SUITE_END()
