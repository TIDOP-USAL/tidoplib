if(BUILD_DOC)

  find_package(Doxygen
               OPTIONAL_COMPONENTS dot mscgen dia)

  if(DOXYGEN_FOUND)

    message(STATUS "DOXYGEN_FOUND ${DOXYGEN_FOUND}")

    #set(DOXYGEN_GENERATE_HTML YES)

    set(DOXYFILE  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(ROOTFILE  ${CMAKE_CURRENT_SOURCE_DIR}/root.markdown)
    set(TUTORIALS ${CMAKE_CURRENT_SOURCE_DIR}/tutorials.markdown)
    set(DOC_OUT   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYFILE}  ${DOC_OUT} @ONLY)
    #configure_file(${ROOTFILE}  ${DOC_OUT} @ONLY)
    #configure_file(${TUTORIALS} ${DOC_OUT} @ONLY)

    message("Doxygen build started.")

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
          COMMAND ${DOXYGEN_EXECUTABLE} ${DOC_OUT}
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMENT "Generating API documentation with Doxygen"
          VERBATIM )
 
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
        DESTINATION share/doc
        COMPONENT "docs" OPTIONAL)
        
  else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif(DOXYGEN_FOUND)

endif(BUILD_DOC)
