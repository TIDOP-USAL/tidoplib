# Modulo Core de la librería

add_library(tl_core ${LIB_TYPE}
            console.cpp
            console.h
            defs.h
            exception.cpp
            exception.h
            flags.h
            messages.cpp
            messages.h
            utils.cpp
            utils.h
            process.cpp
            process.h
            licence.cpp
            licence.h
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_core
             PROPERTY FOLDER "modules")

# Instalación
if(BUILD_SHARED_LIBS)
    install(TARGETS tl_core
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
	
  set_target_properties(tl_core PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
    target_link_libraries(tl_core ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
    if (HAVE_GDAL)
        target_link_libraries(tl_core ${GDAL_LIBRARY})
    endif(HAVE_GDAL)
else()
  install(TARGETS tl_core
          EXPORT "${TARGETS_EXPORT_NAME}"
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

#target_include_directories(
#    tl_core PUBLIC
#    "$<INSTALL_INTERFACE:include>"
#)

install(DIRECTORY .
        DESTINATION include/tidop/core
        FILES_MATCHING PATTERN "*.h")


set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} tl_core)

