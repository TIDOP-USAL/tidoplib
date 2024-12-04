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

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/ptr.h"

#include <memory>
#include <map>

namespace tl
{


/*! \addtogroup raster
 *  \{
 */


class TL_EXPORT MetadataItem
{

public:

    MetadataItem() = default;
    virtual ~MetadataItem() = default;

    virtual void parseValue(const std::string &value) = 0;

};


class TL_EXPORT MetadataItemBase
    : public MetadataItem
{

private:

    std::string mName;
    std::string mDefaultValue;
    std::string mValue;
    bool bActive;

public:

    MetadataItemBase(std::string name,
                     std::string defValue = "");
    ~MetadataItemBase() override = default;

    auto value() const -> std::string;
    void setValue(const std::string &value);
    auto defaultValue() const -> std::string;
    void setDefaultValue(const std::string &defValue);
    auto isActive() const -> bool;

};


class MetadataItemNumber  // NOLINT(cppcoreguidelines-special-member-functions)
    : public MetadataItemBase
{

public:

    MetadataItemNumber(const std::string &name,
                       const std::string &defValue = "");
    ~MetadataItemNumber() override = default;

    void parseValue(const std::string &value) override;

};

class MetadataItemText
    : public MetadataItemBase
{

public:

    MetadataItemText(const std::string &name,
                     const std::string &defValue = "");
    ~MetadataItemText() override = default;

    void parseValue(const std::string &value) override;

};



class TL_EXPORT ImageMetadata
{

    GENERATE_SHARED_PTR(ImageMetadata)

public:

    enum class Format
    {
        tiff,
        jpeg,
        jp2000,
        png,
        bmp,
        gif
    };

protected:

    Format mFormat;

public:

    ImageMetadata(Format format);
    virtual ~ImageMetadata();

    Format format() const;

    virtual auto metadata(const std::string& name, bool &active) const -> std::string = 0;
    virtual void setMetadata(const std::string &name, const std::string &value) = 0;
    virtual auto metadata() const -> std::map<std::string, std::string> = 0;
    virtual auto activeMetadata() const -> std::map<std::string, std::string> = 0;
    virtual void reset() = 0;

};





class TL_EXPORT ImageMetadataFactory
{

private:

    ImageMetadataFactory() {}

public:

    static auto create(const std::string &format) -> std::shared_ptr<ImageMetadata>;
};

/*! \} */ // end of raster

}  // End namespace tl
