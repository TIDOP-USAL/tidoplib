#Modulo img de la librería

add_library(tl_img ${LIB_TYPE}
            img.h
            imghandler.cpp
            imghandler.h
            imgio.cpp
            imgio.h
            formats.cpp
            formats.h
            metadata.h
            metadata.cpp
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_img PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
  install(TARGETS tl_img
    EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
		    LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
			ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
	
	set_target_properties(tl_img PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
	
	target_link_libraries(tl_img 
	                      tl_core tl_geom tl_geospatial)
	if(HAVE_OPENCV)			
        target_link_libraries(tl_img ${OpenCV_LIBS})	
    endif(HAVE_OPENCV)
	
    if (HAVE_GDAL)
        target_link_libraries(tl_img ${GDAL_LIBRARY})
    endif(HAVE_GDAL)
	
else()
    install(TARGETS tl_img 
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()

install(DIRECTORY . 
        DESTINATION include/tidop/img
        FILES_MATCHING PATTERN "*.h")
