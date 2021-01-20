#Modulo Experimental de la librería

add_library(tl_experimental ${LIB_TYPE}
            desingpatterns.cpp
            designpatterns.h
            datamodel.cpp
            datamodel.h
            video/videoio.cpp
            video/videoio.h
            camera.cpp
            camera.h
            photo.cpp
            photo.h)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_experimental PROPERTY FOLDER "modules")

if(HAVE_LAPACKE)
    add_definitions(-DHAVE_LAPACK_CONFIG_H)
    add_definitions(-DLAPACK_COMPLEX_STRUCTURE)
endif(HAVE_LAPACKE)

#instalación
if(BUILD_SHARED_LIBS)
    install(TARGETS tl_experimental
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
    	  	ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib) 
		  
    set_target_properties(tl_experimental PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
						
    target_link_libraries(tl_experimental tl_core)	
						
  if(HAVE_LAPACKE)
    target_link_libraries(tl_experimental ${LAPACKE_LIBRARIES})
  endif(HAVE_LAPACKE)

else()
  install(TARGETS tl_experimental 
          EXPORT "${TARGETS_EXPORT_NAME}"
          LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
          ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()

install(DIRECTORY . 
        DESTINATION include/tidop/experimental
        FILES_MATCHING PATTERN "*.h")

set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} tl_experimental)