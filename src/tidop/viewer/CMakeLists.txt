##########################################################################
#                                                                        #
# Copyright (C) 2021 by Tidop Research Group                             #
# Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       #
#                                                                        #
# This file is part of TidopLib                                          #
#                                                                        #
# TidopLib is free software: you can redistribute it and/or modify       #
# it under the terms of the GNU Lesser General Public License as         #
# published by the Free Software Foundation, either version 3 of the     #
# License, or (at your option) any later version.                        #
#                                                                        #
# TidopLib is distributed in the hope that it will be useful,            #
# but WITHOUT ANY WARRANTY; without even the implied warranty of         #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          #
# GNU Lesser General Public License for more details.                    #
#                                                                        #
# You should have received a copy of the GNU Lesser General Public       #
# License along with TidopLib. If not, see <http://www.gnu.org/licenses>.#
#                                                                        #
# @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         #
#                                                                        #
##########################################################################
 

##########################################################################
#  TidopLib Core module                                                  #
##########################################################################

include(TidopUtils)

if(BUILD_TL_VIEWER)

    project(Viewer)
    
    add_files_to_project(${PROJECT_NAME} 
                         SOURCE_FILES 
                            opengl/buffer/VertexBuffer.cpp
							opengl/buffer/IndexBuffer.cpp
							opengl/buffer/VertexArray.cpp
							opengl/shader/Shader.cpp
							group/PointCloud.cpp
							renderer/Camera.cpp
							renderer/TrackballCamera.cpp
                            renderer/Renderer.cpp
                            io/ModelReader.cpp
                            io/ModelWriter.cpp
                            io/ASCIIReader.cpp
                         HEADER_FILES
                            opengl/buffer/Buffer.h
							opengl/buffer/IndexBuffer.h
							opengl/buffer/VertexBuffer.h
							opengl/buffer/VertexArray.h
							opengl/shader/Shader.h
							group/ModelBase.h
							group/PointCloud.h
							renderer/Camera.h
							renderer/TrackballCamera.h
                            renderer/Renderer.h
							Vertex.h
                            io/ModelReader.h
                            io/ModelWriter.h
                            io/ASCIIReader.h
							 )
                         
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})

    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} PUBLIC
                               $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
                               $<INSTALL_INTERFACE:include>)
							   
	if(Qt6_FOUND)
		include_directories(${Qt6Core_INCLUDE_DIRS}
							${Qt6Concurrent_INCLUDE_DIRS}
							${Qt6OpenGL_INCLUDE_DIRS}
							${Qt6Widgets_INCLUDE_DIRS}
							${Qt6LinguistTools_INCLUDE_DIRS}
							${Qt6OpenGLWidgets_INCLUDE_DIRS})
	endif()

    target_link_libraries(${PROJECT_NAME} PUBLIC
                          ${Boost_FILESYSTEM_LIBRARY} 
                          ${Boost_SYSTEM_LIBRARY}
                          $<$<BOOL:${TL_HAVE_OPENMP}>:OpenMP::OpenMP_CXX>
                          $<$<BOOL:${UNIX}>:-lpthread>
                          TidopLib::Core
                          TidopLib::Geom
						  GLEW::GLEW
						  opengl32
						  glu32)
    
    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          FOLDER "modules"
                          OUTPUT_NAME "tl_viewer")

    if(BUILD_SHARED_LIBS)
  
        set_target_properties(${PROJECT_NAME} PROPERTIES
                              DEFINE_SYMBOL TIDOPLIB_EXPORTS)  
    endif()

    install(TARGETS ${PROJECT_NAME}
            EXPORT "${TARGETS_EXPORT_NAME}"
            RUNTIME DESTINATION ${TL_INSTALL_BINARIES_PREFIX}bin
            LIBRARY DESTINATION ${TL_INSTALL_BINARIES_PREFIX}lib
            ARCHIVE DESTINATION ${TL_INSTALL_BINARIES_PREFIX}lib)
                
    install(DIRECTORY .
            DESTINATION include/tidop/viewer
            FILES_MATCHING PATTERN "*.h")
    
    
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    message(STATUS "  TidopLib::Viewer enabled")
    
endif(BUILD_TL_VIEWER)
