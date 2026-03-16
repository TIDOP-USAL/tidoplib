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
#  TidopLib Geometry module                                              #
##########################################################################

include(TidopUtils)

if(BUILD_TL_GEOMETRY)

    project(Geometry)
  
    add_files_to_project(${PROJECT_NAME} 
                        HEADER_FILES
                            Operations.h
                            algorithms/analysis/Boundary.h
                            algorithms/analysis/Boundary.impl.h
                            algorithms/analysis/Contains.h
                            algorithms/analysis/Contains.impl.h
                            algorithms/analysis/Crosses.h
                            algorithms/analysis/Crosses.impl.h
                            algorithms/analysis/Disjoint.h
                            algorithms/analysis/Disjoint.impl.h
                            algorithms/analysis/Equals.h
                            algorithms/analysis/Equals.impl.h
                            algorithms/analysis/EqualsExact.h
                            algorithms/analysis/EqualsExact.impl.h
                            algorithms/analysis/Intersects.h
                            algorithms/analysis/Intersects.impl.h
                            algorithms/analysis/Overlaps.h
                            algorithms/analysis/Overlaps.impl.h
                            algorithms/analysis/Touches.h
                            algorithms/analysis/Touches.impl.h
                            algorithms/analysis/Within.h
                            algorithms/analysis/Within.impl.h
                            algorithms/clustering/dbscan.h
                            algorithms/measurement/Angle.h
                            algorithms/measurement/Angle.impl.h
                            algorithms/measurement/Area.h
                            algorithms/measurement/Area.impl.h
                            algorithms/measurement/Distance.h
                            algorithms/measurement/Distance.impl.h
                            algorithms/measurement/Length.h
                            algorithms/measurement/Length.impl.h
                            algorithms/spatial/Buffer.h
                            algorithms/spatial/Buffer.impl.h
                            algorithms/spatial/ClosestPoints.h
                            algorithms/spatial/ClosestPoints.impl.h
                            algorithms/spatial/ConvexHull.h
                            algorithms/spatial/ConvexHull.impl.h
                            algorithms/spatial/Envelope.h
                            algorithms/spatial/Envelope.impl.h
                            algorithms/spatial/Intersection.h
                            algorithms/spatial/Intersection.impl.h
                            algorithms/projection.h
                            base/Concepts.h
                            base/Dimension.h
                            base/Geometry.h
                            base/Geometry.impl.h
                            base/GeometryContainer.h
                            base/GeometryCollection.h
                            base/PrecisionPolicy.h
                            base/TolerancePolicy.h
                            base/TopologyKernel.h
                            base/Traits.h
                            io/wkt/Writer.h
                            io/wkt/Proxy.h
                            primitives/LinearRing.h
                            primitives/LineString.h
                            primitives/Point.h
                            primitives/Polygon.h
                            primitives/Segment.h
                            primitives/MultiPoint.h
                            primitives/MultiLinestring.h
                            primitives/MultiPolygon.h
                            shapes/circle.h
                            shapes/ellipse.h
                            shapes/rectangle.h
                            shapes/shape.h
                            shapes/square.h
                            shapes/triangle.h
                            spatial/BoundingBox.h
                            spatial/index/KdTree.h
                            #spatial/index/KdTreeStrategies.h
                            #spatial/index/DynamicKdTree.h
                            #spatial/index/StaticKdTree.h
    )
        
    add_library(${PROJECT_NAME} INTERFACE
                ${PROJECT_SOURCE_FILES}
                ${PROJECT_HEADER_FILES})
    
    add_library(TidopLib::${PROJECT_NAME}
                ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} INTERFACE
                               $<BUILD_INTERFACE:${TidopLib_SOURCE_DIR}/src>
                               $<INSTALL_INTERFACE:include>)

    target_compile_definitions(${PROJECT_NAME} INTERFACE
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:HAVE_LAPACK_CONFIG_H>
                               $<$<BOOL:${TL_HAVE_OPENBLAS}>:LAPACK_COMPLEX_STRUCTURE>)

    target_link_libraries(${PROJECT_NAME} INTERFACE
                          TidopLib::Core
                          TidopLib::Math
                          $<$<BOOL:${TL_HAVE_OPENBLAS}>:OpenBLAS::OpenBLAS>)


    set_target_properties(${PROJECT_NAME} PROPERTIES
                          FOLDER "modules"
                          OUTPUT_NAME "tl_geom")

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
            DESTINATION include/tidop/geometry
            FILES_MATCHING PATTERN "*.h")
  		
    set(TidopLib_LIBRARIES ${TidopLib_LIBRARIES} ${PROJECT_NAME})
  
    set(TL_HAVE_GEOMETRY YES CACHE INTERNAL "")
    message(STATUS "  TidopLib::Geometry enabled")

elseif(BUILD_TL_CORE AND BUILD_TL_MATH) 

    message(STATUS "  TidopLib::Geometry disabled")
    message(STATUS "  TidopLib::RasterTools disabled      [Module dependent on TidopLib::Geometry that is deactivated]") 
    message(STATUS "  TidopLib::Graphic disabled          [Module dependent on TidopLib::Geometry that is deactivated]") 
    message(STATUS "  TidopLib::FeatureMatching disabled  [Module dependent on TidopLib::Geometry that is deactivated]")
    message(STATUS "  TidopLib::Geospatial disabled       [Module dependent on TidopLib::Geometry that is deactivated]")

endif(BUILD_TL_GEOMETRY)