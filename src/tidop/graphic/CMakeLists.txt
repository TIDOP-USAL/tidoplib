#Modulo graphic de la librería

project(tl_graphic)

add_library(${PROJECT_NAME} ${LIB_TYPE}
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
            datamodel.cpp
            datamodel.h
            entities/entity.cpp
            entities/entity.h
            entities/point.cpp
            entities/point.h
            entities/polygon.cpp
            entities/polygon.h
            entities/linestring.cpp
            entities/linestring.h
)
 
 
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET ${PROJECT_NAME} 
             PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
	
	set_target_properties(${PROJECT_NAME} PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
	
	target_link_libraries(${PROJECT_NAME} 
	                      tl_core tl_geom)
else()
    install(TARGETS ${PROJECT_NAME} 
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()

install(DIRECTORY . 
        DESTINATION include/tidop/graphic
        FILES_MATCHING PATTERN "*.h")
        
set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
