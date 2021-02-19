#Modulo geospatial de la librería

project(tl_geospatial)

add_library(${PROJECT_NAME} ${LIB_TYPE}
            crs.cpp
            crs.h
            crscache.cpp
            crscache.h
            crstransf.h
            #dtm.cpp
            #dtm.h
            util.cpp
            util.h
            ortho.cpp
            ortho.h
            footprint.cpp
            footprint.h
            diffrect.cpp
            diffrect.h
            camera.cpp
            camera.h
            photo.cpp
            photo.h
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET ${PROJECT_NAME} 
             PROPERTY FOLDER "modules")

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
	
    target_link_libraries(${PROJECT_NAME} 
                          tl_core 
                          tl_math 
                          tl_geom
                          tl_vect
                          tl_img)
	
	  if (HAVE_GDAL)
        target_link_libraries(${PROJECT_NAME} ${GDAL_LIBRARY})
    endif(HAVE_GDAL)

    if(HAVE_PROJ4)
        target_link_libraries(${PROJECT_NAME} 
                              ${PROJ4_LIBRARY})
    endif(HAVE_PROJ4)

    if(HAVE_LAPACKE)
        target_link_libraries(${PROJECT_NAME} ${LAPACKE_LIBRARIES})
    endif(HAVE_LAPACKE)

else()

    install(TARGETS ${PROJECT_NAME} 
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)

endif()

install(DIRECTORY . 
        DESTINATION include/tidop/geospatial
        FILES_MATCHING PATTERN "*.h")
        
set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
