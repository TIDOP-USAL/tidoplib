# Modulo featmatch de la librería TIDOP que incluye detectores/descriptores y matching

if(HAVE_OPENCV)


project(tl_featmatch)

add_library(${PROJECT_NAME} ${LIB_TYPE}
            agast.cpp
            agast.h
            akaze.cpp
            akaze.h
            boost.cpp
            boost.h
            brief.cpp
            brief.h
            brisk.cpp
            brisk.h
            daisy.cpp
            daisy.h
            evaluation.h
            fast.cpp
            fast.h
            features.h
            featio.h
            featio.cpp
            freak.cpp
            freak.h
            gftt.cpp
            gftt.h
            hog.cpp
            hog.h
            kaze.cpp
            kaze.h
            latch.cpp
            latch.h
            #lss.cpp
            #lss.h
            lucid.cpp
            lucid.h
            matcher.cpp
            matcher.h
            matchio.cpp
            matchio.h
            #msd.cpp
            #msd.h
            mser.cpp
            mser.h
            orb.cpp
            orb.h
            sift.cpp
            sift.h
            star.cpp
            star.h
            surf.cpp
            surf.h
            vgg.h
            vgg.cpp
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET ${PROJECT_NAME}
             PROPERTY FOLDER "modules")

# Instalación
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
                          tl_geom
                          ${OpenCV_LIBS})
    
	if (HAVE_GDAL)
        target_link_libraries(${PROJECT_NAME} ${GDAL_LIBRARY})
    endif(HAVE_GDAL)
    
else()
    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()


install(DIRECTORY .
        DESTINATION include/tidop/featmatch
        FILES_MATCHING PATTERN "*.h")


set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})

endif(HAVE_OPENCV)