/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include <opencv2/core.hpp>

#include "tidop/core/base/Defs.h"
#include "tidop/rastertools/process/ImgProcess.h"
#include "tidop/rastertools/process/filters/Bilateral.h"
#include "tidop/rastertools/process/filters/Blur.h"
#include "tidop/rastertools/process/filters/Box.h"
#include "tidop/rastertools/process/filters/Canny.h"
#include "tidop/rastertools/process/filters/Convolution.h"
#include "tidop/rastertools/process/filters/GaussBlur.h"
#include "tidop/rastertools/process/filters/Laplacian.h"
#include "tidop/rastertools/process/filters/MedianBlur.h"
#include "tidop/rastertools/process/filters/Sobel.h"