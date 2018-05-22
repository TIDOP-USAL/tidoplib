#Modulo graphic de la librería

add_library(tl_graphic ${LIB_TYPE}
  color.cpp
  color.h
  font.cpp
  font.h
  linealgorithms.cpp
  linealgorithms.h
  styles.cpp
  styles.h
  layer.cpp
  layer.h
  canvas.cpp
  canvas.h
  painter.cpp
  painter.h
  entities/entity.cpp
  entities/entity.h
  entities/point.cpp
  entities/point.h
  entities/polygon.cpp
  entities/polygon.h
  entities/linestring.cpp
  entities/linestring.h)
  
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