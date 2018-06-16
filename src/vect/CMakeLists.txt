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
          RUNTIME DESTINATION bin
		  LIBRARY DESTINATION lib) 
else()
  install(TARGETS tl_vect 
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/vect
        FILES_MATCHING PATTERN "*.h")
