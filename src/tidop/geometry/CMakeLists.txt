#Modulo geometry de la librería

add_library(tl_geom ${LIB_TYPE}
            operations.cpp
            operations.h
            transform/transform.h
            transform/affine.h
            transform/helmert2d.h
            transform/helmert3d.h
            transform/perspective.h
            transform/polynomial.h
            transform/projective.h
            transform/rotation.h
            transform/scale.h
            transform/translation.h
            rect.h
            size.h
            entities/bbox.h
            entities/entity.h
            entities/linestring.h
            entities/point.h
            entities/polygon.h
            entities/segment.cpp
            entities/segment.h
            entities/window.h
            entities/shapes.h
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_geom 
             PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)

    install(TARGETS tl_geom
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)

    set_target_properties(tl_geom PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)

    target_link_libraries(tl_geom tl_core tl_math)
    if(HAVE_EIGEN)
        target_link_libraries (tl_geom Eigen3::Eigen)
    endif(HAVE_EIGEN)

else()

    install(TARGETS tl_geom 
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)

endif()


install(DIRECTORY . 
        DESTINATION include/tidop/geometry
        FILES_MATCHING PATTERN "*.h")
		
set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} tl_geom)
