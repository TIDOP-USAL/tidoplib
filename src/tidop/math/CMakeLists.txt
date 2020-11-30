#Modulo Math de la librería

add_library(tl_math ${LIB_TYPE}
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
set_property(TARGET tl_math 
             PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
	install(TARGETS tl_math
			EXPORT "${TARGETS_EXPORT_NAME}"
			RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
			LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
			ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
			
	target_link_libraries(tl_math tl_core)
  
  if(HAVE_LAPACKE)
    target_link_libraries(tl_math ${LAPACKE_LIBRARIES})
  endif(HAVE_LAPACKE)
  
else()
  install(TARGETS tl_math
			EXPORT "${TARGETS_EXPORT_NAME}"
			LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
			ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()

install(DIRECTORY . 
        DESTINATION include/tidop/math
        FILES_MATCHING PATTERN "*.h")
        
set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} tl_math)