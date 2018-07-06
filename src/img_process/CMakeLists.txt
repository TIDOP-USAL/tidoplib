#Modulo Core de la librería

add_library(tl_imgprocess ${LIB_TYPE}
  anaglyph.cpp
  anaglyph.h
  color_convert.cpp
  color_convert.h
  correlation.h
  filters.cpp
  filters.h
  fourier.cpp
  fourier.h
  img_processing.cpp
  img_processing.h
  img_transform.cpp
  img_transform.h
  linedetector.cpp
  linedetector.h
  shapedetection.cpp
  shapedetection.h
  skeleton.cpp
  skeleton.h
  white_balance.cpp
  white_balance.h
  matching.cpp
  matching.h)
  
# Creates folder "libraries" and adds target project (math.vcproj)
set_property(TARGET tl_imgprocess PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
  install(TARGETS tl_imgprocess
          RUNTIME DESTINATION bin
		  LIBRARY DESTINATION lib) 
else()
  install(TARGETS tl_imgprocess 
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/imgprocess
        FILES_MATCHING PATTERN "*.h")