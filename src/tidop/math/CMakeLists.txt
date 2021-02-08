#Modulo Math de la librería TIDOP

if(MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
endif(MSVC)

project(tl_math)

add_library(${PROJECT_NAME} ${LIB_TYPE}
            angles.cpp
            angles.h
            statistics.cpp
            statistics.h
            mathutils.cpp
            mathutils.h
            #numericalmethods.h
            algebra/quaternion.h
            algebra/euler_angles.h
            algebra/rotations.h
            algebra/rotation_matrix.h
            algebra/rotation_convert.h
            algebra/axis_angle.h
            algebra/vectors.h
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

if(HAVE_LAPACKE)
    add_definitions(-DHAVE_LAPACK_CONFIG_H)
    add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
endif(HAVE_LAPACKE)

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
  
    if(HAVE_LAPACKE)
        target_link_libraries(${PROJECT_NAME} ${LAPACKE_LIBRARIES})
    endif(HAVE_LAPACKE)
  
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
