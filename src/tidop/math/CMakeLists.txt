##########################################################################
#                                                                        #
# Copyright (C) 2021 by Tidop Research Group                             #
# Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       #
#                                                                        #
# This file is part of TidopLib                                          #
#                                                                        #
# TidopLib is free software: you can redistribute it and/or modify       #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, either version 3 of the     #
# License, or (at your option) any later version.                        #
#                                                                        #
# TidopLib is distributed in the hope that it will be useful,            #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          #
# GNU Lesser General Public License for more details.                    #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with Foobar. If not, see <http://www.gnu.org/licenses/>. #
#                                                                        #
# @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         #
#                                                                        #
##########################################################################

#Modulo Math de la librería TIDOP

if (HAVE_TL_CORE)

  if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
  endif(MSVC)
  
  project(tl_math)
  
  add_library(${PROJECT_NAME} ${LIB_TYPE}
              math.h
              angles.cpp
              angles.h
              statistics.h
              mathutils.cpp
              mathutils.h
              algebra/quaternion.h
              algebra/euler_angles.h
              algebra/rotations.h
              algebra/rotation_matrix.h
              algebra/rotation_convert.h
              algebra/axis_angle.h
              #algebra/vectors.h
              algebra/matrix.h
              algebra/vector.h
              algebra/svd.h
              algebra/lu.h
              algebra/qr.h
              algebra/cholesky.h
  )
  
  # Añade el proyecto a la carpeta 'modules'
  set_property(TARGET ${PROJECT_NAME} 
               PROPERTY FOLDER "modules")
  
  if(HAVE_EIGEN)
      target_link_libraries (${PROJECT_NAME} Eigen3::Eigen)
  endif(HAVE_EIGEN)
  
  if(HAVE_OPENBLAS)
      add_definitions(-DHAVE_LAPACK_CONFIG_H)
      add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
  endif(HAVE_OPENBLAS)
  
  #instalación
  if(BUILD_SHARED_LIBS)
  
      install(TARGETS ${PROJECT_NAME}
              EXPORT "${TARGETS_EXPORT_NAME}"
              RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
              LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
              ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
  			
      set_target_properties(${PROJECT_NAME} PROPERTIES
                            DEFINE_SYMBOL TIDOPLIB_EXPORTS)
  
      target_link_libraries(${PROJECT_NAME} tl_core)
    
      if(HAVE_OPENBLAS)
          target_link_libraries(${PROJECT_NAME} ${LAPACKE_LIBRARIES})
      endif(HAVE_OPENBLAS)
    
      if(HAVE_OPENCV)
          target_link_libraries(${PROJECT_NAME} 
                                ${OpenCV_LIBS})
      endif(HAVE_OPENCV)
  
  else()
    install(TARGETS ${PROJECT_NAME}
  			EXPORT "${TARGETS_EXPORT_NAME}"
  			LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
  			ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
  endif()
  
  install(DIRECTORY . 
          DESTINATION include/tidop/math
          FILES_MATCHING PATTERN "*.h")
          
  set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
  
  set(HAVE_TL_MATH YES CACHE INTERNAL "")
  message(STATUS "TidopLib::tl_math enabled")

else()

  message(STATUS "TidopLib::tl_math disabled")
  
endif()