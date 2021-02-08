# Modulo de procesamiento de imágenes

project(tl_imgprocess)

add_library(${PROJECT_NAME} ${LIB_TYPE}
            anaglyph.cpp
            anaglyph.h
            colorconvert.cpp
            colorconvert.h
            correlation.h
            filters.cpp
            filters.h
            fourier.cpp
            fourier.h
            imgprocess.cpp
            imgprocess.h
            imgtransform.cpp
            imgtransform.h
            linedetector.cpp
            linedetector.h
            shapedetection.cpp
            shapedetection.h
            skeleton.cpp
            skeleton.h
            whitebalance.cpp
            whitebalance.h
            matching.cpp
            matching.h
            morphologicaloper.cpp
            morphologicaloper.h)
  
# Creates folder "libraries" and adds target project (math.vcproj)
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
                          tl_core
                          tl_graphic)
	
	if(HAVE_OPENCV)			
        target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})	
    endif(HAVE_OPENCV)
	
else()
    install(TARGETS ${PROJECT_NAME} 
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()

install(DIRECTORY . 
        DESTINATION include/tidop/imgprocess
        FILES_MATCHING PATTERN "*.h")
        
set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})       
