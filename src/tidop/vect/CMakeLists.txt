# Modulo vect de la librería
# Lectura y escritura de formatos vectoriales

project(tl_vect)

add_library(${PROJECT_NAME} ${LIB_TYPE}
            vect.h
            vectreader.cpp
            vectreader.h
            vectwriter.cpp
            vectwriter.h			
            formats.cpp
            formats.h)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "modules")



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
                          tl_graphic)	

    if (HAVE_GDAL)
        target_link_libraries(tl_vect ${GDAL_LIBRARY})
    endif(HAVE_GDAL)

else()

    install(TARGETS tl_vect 
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)

endif()

install(DIRECTORY . 
        DESTINATION include/tidop/vect
        FILES_MATCHING PATTERN "*.h")
        
set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
