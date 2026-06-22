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

#include "tidop/config.h"

#include <memory>
#include <map>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <span>

namespace tl
{


/*! \addtogroup RasterIO
 *  \{
 */

/*!
 * \brief Container for image metadata (key‑value pairs).
 *
 * This class stores arbitrary metadata associated with an image, such as
 * EXIF, XMP, or other format‑specific tags. Keys are case‑sensitive strings
 * (using std::less<> for heterogeneous lookup). The container is copyable
 * and movable.
 */
class TL_EXPORT ImageMetadata
{

public:

    using container_type = std::map<std::string, std::string, std::less<>>;
    using iterator = typename std::map<std::string, std::string>::iterator;
    using const_iterator = typename std::map<std::string, std::string>::const_iterator;

protected:

    container_type mMetadata;

public:

    /*!
     * \brief Default constructor – creates an empty metadata container.
     */
    ImageMetadata() = default;

    /*!
     * \brief Default destructor.
     */
    ~ImageMetadata() = default;

    /*!
     * \brief Copy constructor.
     */
    ImageMetadata(const ImageMetadata &) = default;

    /*!
     * \brief Move constructor.
     */
    ImageMetadata(ImageMetadata &&) noexcept = default;

    /*!
     * \brief Copy assignment operator.
     * \return Reference to this object.
     */
    auto operator=(const ImageMetadata &) -> ImageMetadata & = default;

    /*!
     * \brief Move assignment operator.
     * \return Reference to this object.
     */
    auto operator=(ImageMetadata &&) noexcept -> ImageMetadata & = default;

    /*!
     * \brief Checks whether a metadata item exists.
     * \param key Key to look up.
     * \return true if the key exists, false otherwise.
     */
    [[nodiscard]]
    auto contains(std::string_view key) -> bool;

    /*!
     * \brief Retrieves the value associated with a single key.
     * \param key Key to look up.
     * \return An optional containing the value as string_view if found,
     *         or std::nullopt otherwise.
     */
    [[nodiscard]]
    auto value(std::string_view key) const -> std::optional<std::string_view>;

    /*!
     * \brief Retrieves the value associated with the first existing key in a list.
     *
     * This method checks each key in the given order and returns the value
     * of the first key that exists in the container.
     *
     * \param keys A span of candidate keys to try.
     * \return An optional containing the value of the first matching key,
     *         or std::nullopt if none of the keys exist.
     */
    [[nodiscard]]
    auto value(std::span<const std::string> keys) const -> std::optional<std::string_view>;

    /*!
     * \brief Sets the value for a specific key.
     *
     * If the key already exists, its value is overwritten. If not, a new pair is inserted.
     *
     * \param key   The key to set.
     * \param value The value to associate with the key.
     */
    void set(std::string key, std::string value);

    /*!
     * \brief Mutable iterator to the first metadata item.
     * \return Iterator to the beginning.
     */
    auto begin() noexcept -> iterator;

    /*!
     * \brief Constant iterator to the first metadata item.
     * \return Constant iterator to the beginning.
     */
    auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Mutable iterator to the end of the metadata.
     * \return End iterator.
     */
    auto end() noexcept -> iterator;

    /*!
     * \brief Constant iterator to the end of the metadata.
     * \return Constant end iterator.
     */
    auto end() const noexcept -> const_iterator;

    /*!
     * \brief Checks whether the container is empty.
     * \return true if no metadata is stored, false otherwise.
     */
    [[nodiscard]]
    auto empty() const noexcept -> bool;

    /*!
     * \brief Returns the number of metadata items.
     * \return Number of key‑value pairs.
     */
    [[nodiscard]]
    auto size() const noexcept -> size_t;

    /*!
     * \brief Removes all metadata items.
     */
    void clear();

};

/*! \} */ // end of raster

}  // End namespace tl
