#Modulo geom de la librería

add_library(tl_geom ${LIB_TYPE}
  operations.cpp
  operations.h
  transform.cpp
  transform.h
  entities/bbox.h
  entities/curves.h
  entities/entity.h
  entities/lineString.h
  entities/point.h
  entities/polygon.h
  entities/segment.cpp
  entities/segment.h
  entities/window.h)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_geom PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
  install(TARGETS tl_geom
          RUNTIME DESTINATION bin
		  LIBRARY DESTINATION lib) 
else()
  install(TARGETS tl_geom 
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/geometry
        FILES_MATCHING PATTERN "*.h")