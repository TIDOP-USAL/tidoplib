#Modulo graphic de la librería

add_library(tl_vect ${LIB_TYPE}
  vectio.cpp
  vectio.h
  formats.cpp
  formats.h)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_vect PROPERTY FOLDER "modules")

target_link_libraries(tl_vect tl_graphic)

#instalación
if(BUILD_SHARED_LIBS)
    install(TARGETS tl_vect
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
		    LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
		    ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib) 
			
	target_link_libraries(tl_vect tl_core)	
	
    if (HAVE_GDAL)
        target_link_libraries(tl_vect ${GDAL_LIBRARY})
    endif(HAVE_GDAL)

else()
    install(TARGETS tl_vect 
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()

install(DIRECTORY . DESTINATION include/tidop/vect
        FILES_MATCHING PATTERN "*.h")
