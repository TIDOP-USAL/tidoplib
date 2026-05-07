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

/*!
 * \file event.h
 * \brief Event system for task and UI notifications
 *
 * This module provides an event-based notification system for tracking task execution
 * and UI state changes. It uses a type-safe enumeration pattern to represent different
 * event categories and their associated data.
 *
 * ### Classes
 *
 * - \ref Event - Abstract base class for all events
 * - \ref EventBase - Generic event implementation with type tracking
 * - \ref ImageChangeEvent - Specialized event for image changes
 *
 * ### Features
 *
 * - Type-safe event enumeration
 * - Polymorphic event hierarchy
 * - Support for task lifecycle events (running, paused, stopped, etc.)
 * - UI state change notifications (image changes)
 * - Clearable event data for reuse
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * ImageChangeEvent imgEvent;
 * imgEvent.setImage("path/to/image.jpg");
 * 
 * if (imgEvent.type() == Event::Type::image_change) {
 *     std::string path = imgEvent.image();
 * }
 * imgEvent.clear();
 * \endcode
 *
 * \see tl::Event, tl::EventBase, tl::ImageChangeEvent
 */

#pragma once

#include "tidop/config.h"

#include <string>

namespace tl
{

/*! \addtogroup Core
 *  \{
 */

/*!
 * \brief Abstract base class for all events in the system.
 *
 * Defines the interface that all event types must implement.
 * This allows polymorphic handling of different event types through
 * a common base class.
 *
 * ### Usage
 *
 * Inherit from Event to create specialized event types:
 *
 * \code{.cpp}
 * class CustomEvent : public EventBase {
 * public:
 *     CustomEvent() : EventBase(Type::custom_event) {}
 *     // ... custom implementation
 * };
 * \endcode
 */
class TL_EXPORT Event
{

public:

    /*!
     * \enum Type
     * \brief Enumeration of all event types in the system.
     *
     * Represents different events that can occur during application execution.
     *
     * ### Task Events
     * - `task_running` - Task has started execution
     * - `task_pausing` - Task is about to pause
     * - `task_paused` - Task has been paused
     * - `task_resumed` - Task has resumed from pause
     * - `task_stopping` - Task is about to stop
     * - `task_stopped` - Task has stopped
     * - `task_finalized` - Task has completed all cleanup
     * - `task_error` - Error occurred during task execution
     *
     * ### UI Events
     * - `image_change` - Image displayed has changed
     */
    enum class Type
    {
        /* Task events*/

        task_running,    /*!< Task has started execution */
        task_pausing,    /*!< Task is about to pause */
        task_paused,     /*!< Task has been paused */
        task_resumed,    /*!< Task has resumed from pause */
        task_stopping,   /*!< Task is about to stop */
        task_stopped,    /*!< Task has stopped */
        task_finalized,  /*!< Task has completed all cleanup */
        task_error,      /*!< Error occurred during task execution */

        /* Image events */

        image_change     /*!< Image displayed has changed */
    };

public:

    Event() = default;
    virtual ~Event() = default;

    /*!
     * \brief Get the type of this event.
     * \return The event type enumeration value
     */
    virtual auto type() const -> Type = 0;

    /*!
     * \brief Clear all event data.
     *
     * Resets the event to its initial state, clearing any stored data.
     * This allows event objects to be reused.
     */
    virtual void clear() = 0;

};


/*!
 * \brief Generic implementation of Event for simple event types.
 *
 * Provides a concrete implementation of the Event interface that stores
 * a single Type value. Suitable for events that don't require additional
 * data beyond their type.
 */
class TL_EXPORT EventBase
  : public Event
{

public:

    /*!
     * \brief Constructor with event type.
     *
     * \param[in] type The type of event this instance represents
     */
	explicit EventBase(Type type);

    /*!
     * \brief Get the type of this event.
     * \return The event type passed in constructor
     */
    auto type() const -> Type override;

private:

    Type mType;

};



/*!
 * \brief Event emitted when an image changes in the UI.
 *
 * Carries information about an image path or URL that has been displayed.
 * This event is typically emitted when a new image is loaded or when
 * the displayed image changes due to user interaction or application state.
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * ImageChangeEvent event;
 * event.setImage("path/to/new/image.jpg");
 * 
 * // Later, retrieve the image
 * std::string imgPath = event.image();
 * \endcode
 */
class TL_EXPORT ImageChangeEvent final
    : public EventBase
{
private:

    std::string mImage;

public:

    /*!
     * \brief Constructor.
     *
     * Initializes an ImageChangeEvent with type set to image_change.
     */
    ImageChangeEvent();

    /*!
     * \brief Get the image path or URL.
     * \return The stored image path
     */
    auto image() const -> std::string;

    /*!
     * \brief Set the image path or URL.
     * \param[in] image The image path or URL to store
     */
    void setImage(const std::string &image);

    /*!
     * \brief Clear the image path.
     *
     * Resets the image string to empty.
     */
    void clear() override;

};

/*! \} */ 

} // namespace tl

