#Modulo img de la librería

add_library(tl_img ${LIB_TYPE}
  imgio.cpp
  imgio.h
  formats.cpp
  formats.h
  metadata.h
  metadata.cpp)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_img PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
  install(TARGETS tl_img
          RUNTIME DESTINATION bin
		  LIBRARY DESTINATION lib) 
else()
  install(TARGETS tl_img 
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/tidop/img
        FILES_MATCHING PATTERN "*.h")
