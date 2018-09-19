#Modulo geom de la librería

add_library(tl_geom ${LIB_TYPE}
            operations.cpp
            operations.h
            transform.h
            entities/bbox.h
            entities/curves.h
            entities/entity.h
            entities/linestring.h
            entities/point.h
            entities/polygon.h
            entities/segment.cpp
            entities/segment.h
            entities/window.h
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_geom PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)

    install(TARGETS tl_geom
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
	
    set_target_properties(tl_geom PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
	
    target_link_libraries(tl_geom tl_core)
	
else()

    install(TARGETS tl_geom 
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
		  
endif()


install(DIRECTORY . DESTINATION include/tidop/geometry
        FILES_MATCHING PATTERN "*.h")
		
#list(APPEND ${TIDOP_LIBS} tl_geom)
