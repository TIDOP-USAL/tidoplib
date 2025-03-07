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
#  TidopLib Viewer module                                                #
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
                            group/Mesh.cpp
							group/PointCloud.cpp
                            group/Grid.cpp
                            texture/Texture.cpp
							renderer/Camera.cpp
							renderer/TrackballCamera.cpp
                            renderer/Renderer.cpp
                            io/ModelReader.cpp
                            io/ModelWriter.cpp
                            io/ASCIIReader.cpp
                            io/LASReader.cpp
                            io/AssimpReader.cpp
                            widget/ViewerWidget.cpp
                         HEADER_FILES
                            opengl/buffer/Buffer.h
							opengl/buffer/IndexBuffer.h
							opengl/buffer/VertexBuffer.h
							opengl/buffer/VertexArray.h
							opengl/shader/Shader.h
							group/ModelBase.h
                            group/Mesh.h
							group/PointCloud.h
                            group/Grid.h
                            texture/Texture.h
                            texture/vendor/stb_image.h
							renderer/Camera.h
							renderer/TrackballCamera.h
                            renderer/Renderer.h
							Vertex.h
                            io/ModelReader.h
                            io/ModelWriter.h
                            io/ASCIIReader.h
                            io/LASReader.h
                            io/AssimpReader.h
                            raycasting/RayCaster.h
                            raycasting/Picker.h
                            widget/ViewerWidget.h
							 )
                         
    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})

    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    #if(MSVC)
    #    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:__cplusplus")
    #endif(MSVC)

    target_include_directories(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_INCLUDE_DIR}>)

    if(LASLib_FOUND)
        include_directories(${LASLib_INCLUDE_DIR})
    endif()

    target_link_libraries(${PROJECT_NAME} PUBLIC
                          $<$<BOOL:${TL_HAVE_OPENMP}>:OpenMP::OpenMP_CXX>
                          $<$<BOOL:${UNIX}>:-lpthread>
                          ${LASLib_LIBRARIES}
                          TidopLib::Core
                          TidopLib::Geom
                          TidopLib::Math
						  GLEW::GLEW
						  opengl32
						  glu32
                          assimp::assimp
                          ${OpenCV_LIBS}
                          $<$<BOOL:${Qt6_FOUND}>:Qt6::Core>
                          $<$<BOOL:${Qt6_FOUND}>:Qt6::Concurrent>
                          $<$<BOOL:${Qt6_FOUND}>:Qt6::OpenGL>
                          $<$<BOOL:${Qt6_FOUND}>:Qt6::Widgets>
                          $<$<BOOL:${Qt6_FOUND}>:Qt6::OpenGLWidgets>
                          $<$<BOOL:${Qt5_FOUND}>:Qt5::Core>
                          $<$<BOOL:${Qt5_FOUND}>:Qt5::Concurrent>
                          $<$<BOOL:${Qt5_FOUND}>:Qt5::OpenGL>
                          $<$<BOOL:${Qt5_FOUND}>:Qt5::Widgets>
                          $<$<BOOL:${Qt5_FOUND}>:Qt5::OpenGLExtensions>
                          TidopLib::GeoTools
                          TidopLib::PointCloudTools
                          $<$<BOOL:${TL_HAVE_GDAL}>:${GDAL_LIBRARY}>
                          $<$<BOOL:${TL_HAVE_PROJ}>:PROJ::proj>
                          ${PDAL_LIBRARIES})
    
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
