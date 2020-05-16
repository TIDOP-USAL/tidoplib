# Modulo de procesamiento de imágenes

add_library(tl_imgprocess ${LIB_TYPE}
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
            matching.h)
  
# Creates folder "libraries" and adds target project (math.vcproj)
set_property(TARGET tl_imgprocess PROPERTY FOLDER "modules")

#instalación
if(BUILD_SHARED_LIBS)
    install(TARGETS tl_imgprocess
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
			ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib) 
			
    set_target_properties(tl_imgprocess PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)

    target_link_libraries(tl_imgprocess tl_core)
	
	if(HAVE_OPENCV)			
        target_link_libraries(tl_imgprocess ${OpenCV_LIBS})	
    endif(HAVE_OPENCV)
	
else()
    install(TARGETS tl_imgprocess 
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()

install(DIRECTORY . 
        DESTINATION include/tidop/imgprocess
        FILES_MATCHING PATTERN "*.h")
