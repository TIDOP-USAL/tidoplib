include_directories(${CMAKE_CURRENT_SOURCE_DIR})


if(BUILD_TL_CORE)
  #directorio con el módulo 'Core' de la librería
  add_subdirectory(tidop/core)
  
  if(BUILD_TL_MATH)
    add_subdirectory(tidop/math)
  endif()
  
  if(BUILD_TL_GEOMETRY)
    add_subdirectory(tidop/geometry)
  endif()
  
  if(BUILD_TL_IMG)
    add_subdirectory(tidop/img)
  endif()
  
  if(BUILD_TL_IMG_PROCESS)
    add_subdirectory(tidop/imgprocess)
  endif()
  
  if(BUILD_TL_FEAT_MATCH)
    add_subdirectory(tidop/featmatch)
  endif(BUILD_TL_FEAT_MATCH)
  
  if(BUILD_TL_GRAPHIC AND BUILD_TL_GEOMETRY)
    add_subdirectory(tidop/graphic)
  endif()
  
  if(BUILD_TL_GEOSPATIAL)
    add_subdirectory(tidop/geospatial)
  endif()

  if(BUILD_TL_VECT)
    add_subdirectory(tidop/vect)
  endif()
   
  if(BUILD_TL_EXPERIMENTAL)
    add_subdirectory(tidop/experimental)
  endif()
     
else()
  # Se deberia mostrar un mensaje de error
endif()

