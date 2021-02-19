# Modulo Core de la librería TIDOP

project(tl_core)

add_library(${PROJECT_NAME} ${LIB_TYPE}
            console.cpp
            console.h
            defs.h
            exception.cpp
            exception.h
            flags.h
            messages.cpp
            messages.h
            log.cpp
            log.h
            utils.cpp
            utils.h
            process.cpp
            process.h
            licence.cpp
            licence.h
            gdalreg.cpp
            gdalreg.h
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET ${PROJECT_NAME}
             PROPERTY FOLDER "modules")

# Instalación
if(BUILD_SHARED_LIBS)

    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
	
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
						  
    target_link_libraries(${PROJECT_NAME} 
                          ${Boost_FILESYSTEM_LIBRARY} 
                          ${Boost_SYSTEM_LIBRARY})
    
    if (HAVE_GDAL)
         target_link_libraries(${PROJECT_NAME} ${GDAL_LIBRARY})
    endif(HAVE_GDAL)

else()

    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)

endif()


install(DIRECTORY .
        DESTINATION include/tidop/core
        FILES_MATCHING PATTERN "*.h")


set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})

