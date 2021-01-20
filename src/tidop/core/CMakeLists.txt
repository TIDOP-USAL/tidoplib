# Modulo Core de la librería TIDOP

add_library(tl_core ${LIB_TYPE}
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
set_property(TARGET tl_core
             PROPERTY FOLDER "modules")

# Instalación
if(BUILD_SHARED_LIBS)
    install(TARGETS tl_core
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
	
    set_target_properties(tl_core PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
						  
    target_link_libraries(tl_core ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
    
    if (HAVE_GDAL)
         target_link_libraries(tl_core ${GDAL_LIBRARY})
    endif(HAVE_GDAL)

else()
    install(TARGETS tl_core
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()


install(DIRECTORY .
        DESTINATION include/tidop/core
        FILES_MATCHING PATTERN "*.h")


set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} tl_core)

