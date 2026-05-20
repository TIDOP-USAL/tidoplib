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
 * \file SmartPtr.h
 * \brief Convenience macros for smart pointer creation
 *
 * This module provides macros that simplify the creation of shared_ptr and unique_ptr
 * smart pointers with factory methods. Each macro generates a type alias and a static
 * factory method for convenient object instantiation.
 *
 * ### Macros
 *
 * - \ref GENERATE_PTR - Generic smart pointer factory macro
 * - \ref GENERATE_SHARED_PTR - Creates shared_ptr factory for a class
 * - \ref GENERATE_UNIQUE_PTR - Creates unique_ptr factory for a class
 *
 * ### Features
 *
 * - Type-safe smart pointer creation
 * - Consistent factory pattern across classes
 * - Perfect forwarding of constructor arguments
 * - Automatic memory management
 * - Convenient Ptr type alias
 *
 * \see GENERATE_SHARED_PTR, GENERATE_UNIQUE_PTR, GENERATE_PTR
 */
 
#pragma once

#include <iostream>
#include <memory>

/*! \addtogroup Macros
 *  \{
 */
 
/*!
 * \brief Generic macro for generating smart pointer factory methods.
 *
 * This macro generates a type alias and a static factory method for creating
 * instances managed by smart pointers. It uses perfect forwarding to pass
 * constructor arguments efficiently to the underlying smart pointer factory.
 *
 * ### Generated Members
 *
 * - `Ptr` - Type alias for the smart pointer (std::shared_ptr or std::unique_ptr)
 * - `New()` - Static factory method that creates and returns a managed instance
 *
 * ### Parameters
 *
 * - `ptr` - The smart pointer type without namespace (e.g., shared_ptr, unique_ptr)
 * - `make` - The factory function without namespace (e.g., make_shared, make_unique)
 * - `clazz` - The class type to manage
 *
 * ### Example
 *
 * \code{.cpp}
 * class Engine {
 * public:
 *     GENERATE_PTR(shared_ptr, make_shared, Engine)
 *     Engine(const std::string& type) : type_(type) {}
 * private:
 *     std::string type_;
 * };
 *
 * auto engine = Engine::New("V8");  // Returns std::shared_ptr<Engine>
 * \endcode
 *
 * ### Notes
 *
 * - Arguments are forwarded using std::forward for optimal performance
 * - Macro must be used in public section of class definition
 * - All constructor arguments are passed through to the smart pointer factory
 * - Thread-safe if the underlying smart pointer factory is thread-safe
 *
 * \see GENERATE_SHARED_PTR, GENERATE_UNIQUE_PTR
 */
#define GENERATE_PTR(ptr, make, clazz) public: \
                                using Ptr = std:: ptr <clazz>; \
                                template<class... Args> \
                                inline static Ptr New(Args&&... args) { \
                                    return std:: make <clazz>(std::forward<Args>(args)...); \
                                }\


/*!
 * \brief Macro for generating shared_ptr factory method.
 *
 * Convenience macro that generates a shared_ptr-based factory for a class.
 * This is equivalent to calling GENERATE_PTR(shared_ptr, make_shared, clazz).
 *
 * ### Generated Members
 *
 * - `Ptr` - Type alias for std::shared_ptr<clazz>
 * - `New()` - Static factory method creating a shared_ptr to a new instance
 *
 * ### Example
 *
 * \code{.cpp}
 * class Logger {
 * public:
 *     GENERATE_SHARED_PTR(Logger)
 *     Logger(const std::string& filename) : filename_(filename) {}
 * private:
 *     std::string filename_;
 * };
 *
 * // Usage
 * auto logger = Logger::New("app.log");  // Returns std::shared_ptr<Logger>
 * Logger::Ptr shared_logger = Logger::New("debug.log");
 * \endcode
 *
 * ### Notes
 *
 * - Suitable for objects with shared ownership semantics
 * - Multiple instances can share ownership of the same object
 * - Object is automatically deleted when last shared_ptr is destroyed
 * - Thread-safe reference counting
 *
 * \see GENERATE_PTR, GENERATE_UNIQUE_PTR
 */
#define GENERATE_SHARED_PTR(clazz) GENERATE_PTR(shared_ptr, make_shared, clazz)

/*!
 * \brief Macro for generating unique_ptr factory method.
 *
 * Convenience macro that generates a unique_ptr-based factory for a class.
 * This is equivalent to calling GENERATE_PTR(unique_ptr, make_unique, clazz).
 *
 * ### Generated Members
 *
 * - `Ptr` - Type alias for std::unique_ptr<clazz>
 * - `New()` - Static factory method creating a unique_ptr to a new instance
 *
 * ### Example
 *
 * \code{.cpp}
 * class FileHandler {
 * public:
 *     GENERATE_UNIQUE_PTR(FileHandler)
 *     FileHandler(const std::string& path) : path_(path) {}
 * private:
 *     std::string path_;
 * };
 *
 * // Usage
 * auto handler = FileHandler::New("data.txt");  // Returns std::unique_ptr<FileHandler>
 * FileHandler::Ptr unique_handler = FileHandler::New("config.ini");
 * \endcode
 *
 * ### Notes
 *
 * - Suitable for objects with exclusive ownership semantics
 * - Only one unique_ptr can own a given object at a time
 * - More efficient than shared_ptr (no reference counting overhead)
 * - Ownership transfers through move semantics
 *
 * \see GENERATE_PTR, GENERATE_SHARED_PTR
 */
#define GENERATE_UNIQUE_PTR(clazz) GENERATE_PTR(unique_ptr, make_unique, clazz)

/*! \} */