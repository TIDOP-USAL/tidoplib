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
 
# Math tests

if(TL_HAVE_MATH)
add_subdirectory(utils)
add_subdirectory(matrix)
add_subdirectory(quaternion)
add_subdirectory(vector)
add_subdirectory(angle_conversion)
add_subdirectory(rotation_converter)
add_subdirectory(statistics)
add_subdirectory(statistic)
add_subdirectory(qr)
add_subdirectory(lu)
add_subdirectory(cholesky)
add_subdirectory(svd)
add_subdirectory(simd)
add_subdirectory(umeyama)
add_subdirectory(transform)
add_subdirectory(affine)
add_subdirectory(translation)
add_subdirectory(scaling)
add_subdirectory(rotation)
endif()
