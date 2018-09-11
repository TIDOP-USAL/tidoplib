include_directories(${CMAKE_CURRENT_SOURCE_DIR})


if(BUILD_TL_CORE)
  #directorio con el módulo 'Core' de la librería
  add_subdirectory(core)
  
  if(BUILD_TL_MATH)
    add_subdirectory(math)
  endif()
  
  if(BUILD_TL_GEOMETRY)
    add_subdirectory(geometry)
  endif()
  
  if(BUILD_TL_IMG)
    add_subdirectory(img)
  endif()
  
  if(BUILD_TL_IMG_PROCESS)
    add_subdirectory(img_process)
  endif()
  
  if(BUILD_TL_GRAPHIC AND BUILD_TL_GEOMETRY)
    add_subdirectory(graphic)
  endif()
  
  if(BUILD_TL_GEOSPATIAL)
    add_subdirectory(geospatial)
  endif()

  if(BUILD_TL_VECT)
    add_subdirectory(vect)
  endif()
   
  if(BUILD_TL_EXPERIMENTAL)
    add_subdirectory(experimental)
  endif()
     
else()
  # Se deberia mostrar un mensaje de error
endif()

