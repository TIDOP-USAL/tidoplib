if(BUILD_DOC)

find_package(Doxygen)

if(DOXYGEN_FOUND)

  message(STATUS "DOXYGEN_FOUND ${DOXYGEN_FOUND}")

  #set(doxyfile  "${PROJECT_SOURCE_DIR}/doc/Doxyfile")
  set(rootfile  "${PROJECT_SOURCE_DIR}/doc/root.markdown")
  set(tutorials "${PROJECT_SOURCE_DIR}/doc/tutorials.markdown")
  #set(path_out  "${CMAKE_CURRENT_BINARY_DIR}/doc")
  set(doxyfile  "${CMAKE_CURRENT_BINARY_DIR}/doxyfile")
  
  configure_file(doxyfile ${path_out} @ONLY)
  configure_file(root.markdown ${path_out} @ONLY)
  configure_file(tutorials.markdown ${path_out} @ONLY)
  
  #add_custom_target(doxygen
  #  COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
  #  DEPENDS ${doxyfile} ${rootfile} ${tutorials}
  #)
  #
  #install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen/html
  #  DESTINATION "${TIDOP_DOC_INSTALL_PATH}"
  #  COMPONENT "docs" OPTIONAL
  #)

  add_custom_target( doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/src"
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
        
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")  
endif(DOXYGEN_FOUND)

endif(BUILD_DOC)