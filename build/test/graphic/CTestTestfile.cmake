# CMake generated Testfile for 
# Source directory: C:/Desarrollo/TidopLib/test/graphic
# Build directory: C:/Desarrollo/TidopLib/build/test/graphic
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_color_test "C:/Desarrollo/TidopLib/build/test/graphic/Release/test_color_test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_color_test "C:/Desarrollo/TidopLib/build/test/graphic/Debug/test_color_test.exe")
else()
  add_test(test_color_test NOT_AVAILABLE)
endif()
