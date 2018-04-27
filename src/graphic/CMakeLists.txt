#Modulo graphic de la librería

add_library(tl_graphic ${LIB_TYPE}
  color.cpp
  color.h
  entities.cpp
  entities.h
  linealgorithms.cpp
  linealgorithms.h)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_graphic PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
  install(TARGETS tl_graphic
          RUNTIME DESTINATION bin
		  LIBRARY DESTINATION lib) 
else()
  install(TARGETS tl_graphic 
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/graphic
        FILES_MATCHING PATTERN "*.h")