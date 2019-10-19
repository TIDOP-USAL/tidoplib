# Modulo featmatch de la librería TIDOP que incluye detectores/descriptores y matching

if(HAVE_OPENCV)

add_library(tl_featmatch ${LIB_TYPE}
            agast.cpp
            agast.h
            akaze.cpp
            akaze.h
            brief.cpp
            brief.h
            brisk.cpp
            brisk.h
            daisy.cpp
            daisy.h
            evaluation.h
            fast.cpp
            fast.h
            features.cpp
            features.h
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
)
  
# Añade el proyecto a la carpeta 'modules'
set_property(TARGET tl_featmatch
             PROPERTY FOLDER "modules")

# Instalación
if(BUILD_SHARED_LIBS)
    install(TARGETS tl_featmatch
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
	
    set_target_properties(tl_featmatch PROPERTIES
                          DEFINE_SYMBOL TIDOPLIB_EXPORTS)
						  
    target_link_libraries(tl_featmatch 
                          tl_core 
                          tl_geom
                          ${OpenCV_LIBS})
    
	if (HAVE_GDAL)
        target_link_libraries(tl_featmatch ${GDAL_LIBRARY})
    endif(HAVE_GDAL)
    
else()
    install(TARGETS tl_featmatch
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib 
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}/lib)
endif()

#target_include_directories(
#    tl_featmatch PUBLIC
#    "$<INSTALL_INTERFACE:include>"
#)

install(DIRECTORY .
        DESTINATION include/tidop/featmatch
        FILES_MATCHING PATTERN "*.h")


set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} tl_featmatch)

endif(HAVE_OPENCV)