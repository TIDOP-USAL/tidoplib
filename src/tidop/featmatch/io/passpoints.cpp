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

#include "tidop/featmatch/io/passpoints.h"

//#include "tidop/core/base/exception.h"
//#include "tidop/core/base/string_utils.h"
#include "tidop/core/base/split.h"

#include <fstream>

namespace tl
{


void passPointsWrite(const std::string &fname,
                     const std::vector<std::vector<std::pair<std::string, int>>> &pass_points)
{
    std::ofstream ofs(fname, std::ofstream::trunc);
    if (ofs.is_open()) {

        for (size_t i = 0; i < pass_points.size(); i++) {

            ofs << i;

            for (size_t j = 0; j < pass_points[i].size(); j++) {
                ofs << ";" << pass_points[i][j].first
                    << ";" << pass_points[i][j].second;
            }

            ofs << std::endl;
        }

        ofs.close();
    }
}

void passPointsRead(const std::string &fname, std::vector<std::vector<std::pair<std::string, int>>> &pass_points)
{
    pass_points.resize(0);
    std::ifstream ifs(fname);
    std::string line;
    if (ifs.is_open()) {

        int r = 0;
        while (std::getline(ifs, line)) {

            std::vector<std::string> list;
            list = split<std::string>(line, ';');
            size_t size = list.size();
            if (size >= 1) {
                if (size == 1 || size % 2 == 0) {
                    /// deleted point
                    pass_points.push_back(std::vector<std::pair<std::string, int>>());
                } else {
                    std::vector<std::pair<std::string, int>> pass_point;
                    for (size_t i = 1; i < size; i++) {
                        std::string idImage = list[i];
                        int idx = convertStringTo<int>(list[++i]);
                        pass_point.emplace_back(idImage, idx);
                    }
                    pass_points.push_back(pass_point);
                }
            }

            r++;
        }

        ifs.close();
    }
}


} // namespace tl
