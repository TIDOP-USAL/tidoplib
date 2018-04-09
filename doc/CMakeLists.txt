if(BUILD_DOC)

find_package(Doxygen)

if(DOXYGEN_FOUND)

  set(doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  set(rootfile "${CMAKE_CURRENT_BINARY_DIR}/root.markdown")
  set(tutorials "${CMAKE_CURRENT_SOURCE_DIR}/tutorials.markdown")
 
  
  configure_file(Doxyfile ${doxyfile} @ONLY)
  configure_file(root.markdown ${rootfile} @ONLY)
  configure_file(tutorials.markdown ${tutorials} @ONLY)
  
  add_custom_target(doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    DEPENDS ${doxyfile} ${rootfile} ${tutorials}
  )
  
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen/html
    DESTINATION "${TIDOP_DOC_INSTALL_PATH}"
    COMPONENT "docs" OPTIONAL
  )

else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")  
endif(DOXYGEN_FOUND)

endif(BUILD_DOC)