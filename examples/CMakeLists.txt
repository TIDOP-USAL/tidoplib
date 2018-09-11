# Ejemplos
#TODO: http://www.jamesebutler.com/setting-the-visual-studio-debugger-path-using-cmake/

if(BUILD_EXAMPLES)

  add_subdirectory(core)
  #add_subdirectory(geometry)
  add_subdirectory(geospatial)
  #add_subdirectory(graphic)
  add_subdirectory(img)
  add_subdirectory(imgproces)
  add_subdirectory(vect)
  add_subdirectory(math)
  #add_subdirectory(experimental)

endif()
