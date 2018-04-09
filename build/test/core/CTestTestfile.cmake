# CMake generated Testfile for 
# Source directory: C:/Desarrollo/TidopLib/test/core
# Build directory: C:/Desarrollo/TidopLib/build/test/core
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_flag_test "C:/Desarrollo/TidopLib/build/test/core/Release/test_flag_test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_flag_test "C:/Desarrollo/TidopLib/build/test/core/Debug/test_flag_test.exe")
else()
  add_test(test_flag_test NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_mathutils_test "C:/Desarrollo/TidopLib/build/test/core/Release/test_mathutils_test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_mathutils_test "C:/Desarrollo/TidopLib/build/test/core/Debug/test_mathutils_test.exe")
else()
  add_test(test_mathutils_test NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_msg_test "C:/Desarrollo/TidopLib/build/test/core/Release/test_msg_test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_msg_test "C:/Desarrollo/TidopLib/build/test/core/Debug/test_msg_test.exe")
else()
  add_test(test_msg_test NOT_AVAILABLE)
endif()
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_utils_test "C:/Desarrollo/TidopLib/build/test/core/Release/test_utils_test.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_utils_test "C:/Desarrollo/TidopLib/build/test/core/Debug/test_utils_test.exe")
else()
  add_test(test_utils_test NOT_AVAILABLE)
endif()
