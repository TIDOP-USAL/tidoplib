#Modulo geospatial de la librería

add_library(tl_geospatial ${LIB_TYPE}
            crs.cpp
            crs.h
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_geospatial PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
    install(TARGETS tl_geospatial
            RUNTIME DESTINATION bin
		    LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib)
	
	set_target_properties(tl_geospatial PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
	
	target_link_libraries(tl_geospatial tl_core tl_geom ${GDAL_LIBRARY})
else()
  install(TARGETS tl_geospatial 
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/tidop/geospatial
        FILES_MATCHING PATTERN "*.h")
