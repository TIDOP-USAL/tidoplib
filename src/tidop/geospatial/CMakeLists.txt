#Modulo geospatial de la librería

add_library(tl_geospatial ${LIB_TYPE}
            crs.cpp
            crs.h
            dtm.cpp
            dtm.h
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_geospatial PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
    install(TARGETS tl_geospatial
      EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
		    LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
			ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
	
	set_target_properties(tl_geospatial PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
	
	target_link_libraries(tl_geospatial tl_core tl_geom)
	
	if (HAVE_GDAL)
        target_link_libraries(tl_geospatial ${GDAL_LIBRARY})
    endif(HAVE_GDAL)
	
else()
  install(TARGETS tl_geospatial 
    EXPORT "${TARGETS_EXPORT_NAME}"
          LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
          ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()

install(DIRECTORY . DESTINATION include/tidop/geospatial
        FILES_MATCHING PATTERN "*.h")
