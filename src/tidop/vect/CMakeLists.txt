#Modulo graphic de la librer�a

add_library(tl_vect ${LIB_TYPE}
  vectio.cpp
  vectio.h
  formats.cpp
  formats.h)
  
# A�ade el proyecto a la carpeta 'modules'
set_property(TARGET tl_vect PROPERTY FOLDER "modules")

target_link_libraries(tl_vect tl_graphic)

#instalaci�n
if(BUILD_SHARED_LIBS)
  install(TARGETS tl_vect
    EXPORT "${TARGETS_EXPORT_NAME}"
          RUNTIME DESTINATION bin
		  LIBRARY DESTINATION lib) 
else()
  install(TARGETS tl_vect 
    EXPORT "${TARGETS_EXPORT_NAME}"
          LIBRARY DESTINATION lib 
          ARCHIVE DESTINATION lib)
endif()

install(DIRECTORY . DESTINATION include/tidop/vect
        FILES_MATCHING PATTERN "*.h")