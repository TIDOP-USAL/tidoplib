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

#include "tidop/img/metadata.h"

namespace tl
{


class TL_EXPORT ImageMetadataBase
  : public ImageMetadata
{

protected:

    std::map<std::string, std::pair<std::string, bool>> mMetadata;

public:

    ImageMetadataBase(Format format)
        : ImageMetadata(format)
    {
    }

    ~ImageMetadataBase() override = default;

    auto metadata(const std::string& name,
                  bool &active) const -> std::string override;
    void setMetadata(const std::string &name,
                     const std::string &value) override;
    auto metadata() const -> std::map<std::string, std::string> override;
    auto activeMetadata() const -> std::map<std::string, std::string> override;
    void reset() override;

private:

    virtual void init() {}
    auto metadata(bool all) const -> std::map<std::string, std::string>;

};


}  // End namespace tl
