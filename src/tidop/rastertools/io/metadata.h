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

#include "tidop/core/base/defs.h"
#include "tidop/core/base/ptr.h"

#include <memory>
#include <map>

namespace tl
{


/*! \addtogroup RasterIO
 *  \{
 */

/*!
 * \brief Abstract base class for metadata items.
 *
 * The `MetadataItem` class provides an interface for handling different types of metadata items. 
 * Derived classes must implement the `parseValue` method to parse and handle specific metadata values.
 */
class TL_EXPORT MetadataItem
{

public:

    /*!
     * \brief Default constructor.
     */
    MetadataItem() = default;
    virtual ~MetadataItem() = default;

    /*!
     * \brief Parses and sets the value of the metadata item.
     * \param[in] value The string representation of the value to parse.
     */
    virtual void parseValue(const std::string &value) = 0;

};



/*!
 * \brief Base class for specific types of metadata items.
 *
 * The `MetadataItemBase` class extends `MetadataItem` to handle common properties such as
 * name, default value, and current value. It also tracks whether the item is active.
 */
class TL_EXPORT MetadataItemBase
    : public MetadataItem
{

private:

    std::string mName;         /*!< Name of the metadata item */
    std::string mDefaultValue; /*!< Default value of the metadata item */
    std::string mValue;        /*!< Current value of the metadata item */
    bool bActive;              /*!< Flag indicating if the metadata item is active */

public:

    /*!
     * \brief Constructor to initialize a metadata item with a name and optional default value.
     * \param[in] name Name of the metadata item.
     * \param[in] defValue Default value of the metadata item.
     */
    MetadataItemBase(std::string name,
                     std::string defValue = "");

    ~MetadataItemBase() override = default;

    /*!
     * \brief Returns the current value of the metadata item.
     * \return Current value.
     */
    auto value() const -> std::string;

    /*!
     * \brief Sets the current value of the metadata item.
     * \param[in] value New value to set.
     */
    void setValue(const std::string &value);

    /*!
     * \brief Returns the default value of the metadata item.
     * \return Default value.
     */
    auto defaultValue() const -> std::string;

    /*!
     * \brief Sets the default value of the metadata item.
     * \param[in] defValue New default value to set.
     */
    void setDefaultValue(const std::string &defValue);

    /*!
     * \brief Checks if the metadata item is active.
     * \return True if active, false otherwise.
     */
    auto isActive() const -> bool;

};



/*!
 * \brief Metadata item representing a numeric value.
 *
 * The `MetadataItemNumber` class extends `MetadataItemBase` to handle numeric values.
 * It overrides the `parseValue` method to ensure the value is parsed as a number.
 */
class MetadataItemNumber
  : public MetadataItemBase
{

public:

    /*!
     * \brief Constructor to initialize a numeric metadata item.
     * \param[in] name Name of the metadata item.
     * \param[in] defValue Default numeric value as a string.
     */
    MetadataItemNumber(const std::string &name,
                       const std::string &defValue = "");
    ~MetadataItemNumber() override = default;

    /*!
     * \brief Parses and sets the value as a numeric value.
     * \param[in] value The string representation of the numeric value to parse.
     */
    void parseValue(const std::string &value) override;

};


/*!
 * \brief Metadata item representing a text value.
 *
 * The `MetadataItemText` class extends `MetadataItemBase` to handle text values.
 * It overrides the `parseValue` method to handle text-specific parsing.
 */
class MetadataItemText
    : public MetadataItemBase
{

public:

    /*!
     * \brief Constructor to initialize a text metadata item.
     * \param[in] name Name of the metadata item.
     * \param[in] defValue Default text value.
     */
    MetadataItemText(const std::string &name,
                     const std::string &defValue = "");

    ~MetadataItemText() override = default;

    /*!
     * \brief Parses and sets the value as a text value.
     * \param[in] value The string representation of the text value to parse.
     */
    void parseValue(const std::string &value) override;

};



/*!
 * \brief Class representing image metadata.
 *
 * The `ImageMetadata` class provides an interface for managing metadata of different image formats.
 * It supports retrieving, setting, and resetting metadata, as well as accessing active metadata.
 */
class TL_EXPORT ImageMetadata
{

    GENERATE_SHARED_PTR(ImageMetadata)

public:

    /*!
     * \brief Enum representing supported image formats.
     */
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

    /*!
     * \brief Constructor to initialize metadata with a specific format.
     * \param[in] format Format of the image.
     */
    ImageMetadata(Format format);
    virtual ~ImageMetadata();

    /*!
     * \brief Returns the format of the image.
     * \return Image format.
     */
    auto format() const -> Format;

    /*!
     * \brief Retrieves the value of a specific metadata item.
     * \param[in] name Name of the metadata item.
     * \param[out] active Indicates if the metadata item is active.
     * \return Value of the metadata item.
     */
    virtual auto metadata(const std::string &name, bool &active) const -> std::string = 0;

    /*!
     * \brief Sets the value of a specific metadata item.
     * \param[in] name Name of the metadata item.
     * \param[in] value Value to set.
     */
    virtual void setMetadata(const std::string &name, const std::string &value) = 0;

    /*!
     * \brief Returns a map of all metadata items and their values.
     * \return Map of metadata items.
     */
    virtual auto metadata() const -> std::map<std::string, std::string> = 0;

    /*!
     * \brief Returns a map of active metadata items and their values.
     * \return Map of active metadata items.
     */
    virtual auto activeMetadata() const -> std::map<std::string, std::string> = 0;

    /*!
     * \brief Resets all metadata to their default values.
     */
    virtual void reset() = 0;

};




/*!
 * \brief Factory class for creating `ImageMetadata` instances.
 *
 * The `ImageMetadataFactory` provides a static method to create `ImageMetadata` instances
 * based on the specified image format.
 */
class TL_EXPORT ImageMetadataFactory
{

private:

    ImageMetadataFactory() {}

public:

    /*!
     * \brief Creates an `ImageMetadata` instance for the specified format.
     * \param[in] format String representation of the image format.
     * \return Shared pointer to the created `ImageMetadata` instance.
     */
    static auto create(const std::string &format) -> std::shared_ptr<ImageMetadata>;
};

/*! \} */ // end of raster

}  // End namespace tl
