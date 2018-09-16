#Modulo Math de la librería

add_library(tl_math ${LIB_TYPE}
            angles.cpp
            angles.h
            matrix.h
            statistics.cpp
            statistics.h
            mathutils.cpp
            mathutils.h
            algebra/quaternion.h
            algebra/euler_angles.h
            algebra/rotations.h
            algebra/rotation_matrix.h
            algebra/vectors.h
)

# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_math PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
  install(TARGETS tl_math
          RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib)
else()
  install(TARGETS tl_math
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/tidop/math
        FILES_MATCHING PATTERN "*.h")
