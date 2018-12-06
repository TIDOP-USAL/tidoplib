#Modulo Core de la librería

add_library(tl_experimental ${LIB_TYPE}
            desingpatterns.cpp
            designpatterns.h)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_experimental PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
  install(TARGETS tl_experimental
          RUNTIME DESTINATION bin
		  LIBRARY DESTINATION lib) 
else()
  install(TARGETS tl_experimental 
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/experimental
        FILES_MATCHING PATTERN "*.h")
