# CMake generated Testfile for 
# Source directory: C:/Desarrollo/TidopLib/test/geometry
# Build directory: C:/Desarrollo/TidopLib/build/test/geometry
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_lineString_test "C:/Desarrollo/TidopLib/build/test/geometry/Release/cpp-test-lineString_test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_lineString_test "C:/Desarrollo/TidopLib/build/test/geometry/Debug/cpp-test-lineString_test.exe")
else()
  add_test(test_lineString_test NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_segment_test "C:/Desarrollo/TidopLib/build/test/geometry/Release/cpp-test-segment_test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_segment_test "C:/Desarrollo/TidopLib/build/test/geometry/Debug/cpp-test-segment_test.exe")
else()
  add_test(test_segment_test NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_transform_test "C:/Desarrollo/TidopLib/build/test/geometry/Release/cpp-test-transform_test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_transform_test "C:/Desarrollo/TidopLib/build/test/geometry/Debug/cpp-test-transform_test.exe")
else()
  add_test(test_transform_test NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_window_test "C:/Desarrollo/TidopLib/build/test/geometry/Release/cpp-test-window_test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_window_test "C:/Desarrollo/TidopLib/build/test/geometry/Debug/cpp-test-window_test.exe")
else()
  add_test(test_window_test NOT_AVAILABLE)
endif()
