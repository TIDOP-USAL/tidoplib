/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by David Hernandez Lopez                            *
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

#include <map>
#include <string>
#include <memory>

#include "tidop/config.h"
#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/ptr.h"
#include "tidop/core/exception.h"
#include "tidop/core/utils.h"

#include <proj.h>
#include <copc-lib/las/header.hpp>
#include <lazperf/readers.hpp>
#include <copc-lib/io/copc_reader.hpp>
#include <copc-lib/laz/decompressor.hpp>


#include "tidop/geotools/GeoTools.h"
#include "../PointCloudReader.h"
#include "PointCloudReaderPDAL.h"

namespace tl
{
    auto PointCloudReaderFactory::create(const Path& file) -> PointCloudReader::Ptr
    {
        PointCloudReader::Ptr pointCloudReader;
        //try {
            TL_ASSERT(file.exists(), "File doesn't exist: {}", file.toString());
            std::string extension = file.extension().toString();
            if (compareInsensitiveCase(extension, ".las")
                || compareInsensitiveCase(extension, ".laz"))
            {
                pointCloudReader = PointCloudReaderPDAL::New(file);
            }
            else
            {
                TL_THROW_EXCEPTION("Invalid Point Cloud Reader: {}", file.fileName().toString());
            }

            /*
            if(compareInsensitiveCase(extension, ".las")
                || compareInsensitiveCase(extension, ".laz"))
            {
                bool isCopcLaz = true;
                try
                {
                    copc::FileReader copcFile(file.toString());
                }
                catch (...)
                {
                    isCopcLaz = false;
                    //TL_THROW_EXCEPTION("Error while opening COPC file: {}", file.fileName().toString());
                    //auto f_stream = new std::fstream;
                    //f_stream->open(file.toString().c_str(), std::ios::in | std::ios::binary);
                    //if (!f_stream->good())
                    //    TL_THROW_EXCEPTION("Error while opening file: {}", file.fileName().toString());
                    //std::unique_ptr<lazperf::reader::generic_file> reader_;
                    //reader_ = std::make_unique<lazperf::reader::generic_file>(*f_stream);
                    //const base_header& header = reader_->header();
                }
                if (!isCopcLaz)
                {
                    TL_THROW_EXCEPTION("File must be COPC LAZ: {}", file.fileName().toString());
                }
                try {
                    //mCopcFile(mFileName);
                    pointCloudReader = PointCloudReaderPDAL::New(file);
                        //auto las_header = mCopcFile.CopcConfig().LasHeader();
                        //auto copc_info = mCopcFile.CopcConfig().CopcInfo();
                        //auto copc_extents = mCopcFile.CopcConfig().CopcExtents();
                }
                catch (...) {
                    // try lastools
                    TL_THROW_EXCEPTION_WITH_NESTED("");
                }
            }
            else
            {
                TL_THROW_EXCEPTION("Invalid Point Cloud Reader: {}", file.fileName().toString());
            }
            */
        //}
        //catch (...) {
        //    TL_THROW_EXCEPTION_WITH_NESTED("");
        //}
        return pointCloudReader;
    }
}