#Modulo img de la librería

add_library(tl_img ${LIB_TYPE}
            imgio.cpp
            imgio.h
            formats.cpp
            formats.h
            #metadata.h
            #metadata.cpp
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_img PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
  install(TARGETS tl_img
            RUNTIME DESTINATION bin
		    LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib)
	
	set_target_properties(tl_img PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
	
	target_link_libraries(tl_img 
	                      tl_core tl_geom tl_geospatial ${GDAL_LIBRARY} )
else()
  install(TARGETS tl_img 
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/img
        FILES_MATCHING PATTERN "*.h")