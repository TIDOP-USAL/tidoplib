##########################################################################
#                                                                        #
# Copyright (C) 2026 by Tidop Research Group                             #
#                                                                        #
# This file is part of TidopLib                                          #
#                                                                        #
##########################################################################

include(TidopUtils)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(test_filename dataframe_test.cpp)
get_filename_component(test_name ${test_filename} NAME_WE)

project(${test_name} LANGUAGES CXX)

add_executable(${PROJECT_NAME} 
               ${test_filename})

target_link_libraries(${PROJECT_NAME}
                      TidopLib::Core
                      TidopLib::Math
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

set_target_properties(${PROJECT_NAME} PROPERTIES
                      OUTPUT_NAME ${PROJECT_NAME}
                      PROJECT_LABEL "(TEST) ${PROJECT_NAME}")

set_target_properties(${PROJECT_NAME} PROPERTIES 
                      FOLDER "test/math")

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

if(MSVC)
    set_user_enviroment_path(${PROJECT_NAME} "${ENV_VARS_LIST}")
    set_tests_properties(${PROJECT_NAME} PROPERTIES ENVIRONMENT "PATH=$ENV{PATH}")
endif(MSVC)
