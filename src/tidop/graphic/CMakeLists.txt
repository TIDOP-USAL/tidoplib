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
#  TidopLib graphic module                                               #
##########################################################################

include(TidopUtils)


if (BUILD_TL_GRAPHIC)
  
    project(Graphic)

    add_files_to_project(${PROJECT_NAME} 
                        SOURCE_FILES 
                            color/Color.cpp
                            color/ColorConversion.cpp
                            color/ColorHSL.cpp
                            color/ColorHSV.cpp
                            entities/GLineString.cpp
                            entities/GLineString3D.cpp
                            entities/GMultiLineString.cpp
                            entities/GMultiLineString3D.cpp
                            entities/GMultiPoint.cpp
                            entities/GMultiPoint3D.cpp
                            entities/GMultiPolygon.cpp
                            entities/GMultiPolygon3D.cpp
                            entities/GPoint.cpp
                            entities/GPoint3D.cpp
                            entities/GPolygon.cpp
                            entities/GPolygon3D.cpp
                            entities/GraphicEntity.cpp
                            model/GLayer.cpp
                            model/RegisterValue.cpp
                            model/TableField.cpp
                            model/TableRegister.cpp
                            render/Canvas.cpp
                            render/Painter.cpp
                            styles/GraphicStyle.cpp
                        HEADER_FILES
                            Color.h
                            Styles.h
                            base/Rect.h
                            color/ColorCMYK.h
                            color/Color.h
                            color/ColorConversion.h
                            color/ColorHSL.h
                            color/ColorHSV.h
                            color/ColorRGB.h
                            color/ColorRGBA.h
                            color/Concepts.h
                            entities/GLineString.h
                            entities/GLineString3D.h
                            entities/GMultiLineString.h
                            entities/GMultiLineString3D.h
                            entities/GMultiPoint.h
                            entities/GMultiPoint3D.h
                            entities/GMultiPolygon.h
                            entities/GMultiPolygon3D.h
                            entities/GPoint.h
                            entities/GPoint3D.h
                            entities/GPolygon.h
                            entities/GPolygon3D.h
                            entities/GraphicEntity.h
                            model/GLayer.h
                            model/RegisterValue.h
                            model/TableField.h
                            model/TableRegister.h
                            render/Canvas.h
                            render/Painter.h
                            styles/Brush.h
                            styles/BrushPattern.h
                            styles/Font.h
                            styles/GraphicStyle.h
                            styles/Label.h
                            styles/LabelAnchor.h
                            styles/Pen.h
                            styles/Symbol.h)

    add_library(${PROJECT_NAME} ${LIB_TYPE}
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})

    add_library(TidopLib::${PROJECT_NAME} 
                ALIAS ${PROJECT_NAME})

    target_compile_definitions(${PROJECT_NAME} PUBLIC
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} PUBLIC
  	                      TidopLib::Core 
                          TidopLib::Geometry
                          $<$<BOOL:${TL_HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>
                          ${OpenCV_LIBS})

    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          FOLDER "modules"
                          OUTPUT_NAME "tl_graphic")


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
            DESTINATION include/tidop/graphic
            FILES_MATCHING PATTERN "*.h")
            
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
    
    set(TL_HAVE_GRAPHIC YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::${PROJECT_NAME} enabled")
   
elseif(BUILD_TL_CORE AND BUILD_TL_GEOMETRY)

    message(STATUS "  TidopLib::Graphic disabled")
    message(STATUS "  TidopLib::RasterTools disabled  [Module dependent on TidopLib::Graphic that is deactivated]") 
    message(STATUS "  TidopLib::VectorTools disabled  [Module dependent on TidopLib::Graphic that is deactivated]") 
    message(STATUS "  TidopLib::Experimental disabled [Module dependent on TidopLib::Graphic that is deactivated]")

endif(BUILD_TL_GRAPHIC)