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

/*! \addtogroup Macros
 *  \{
 */

/*!
 * \brief Disable copy constructor and copy assignment operator.
 *
 * Marks a class as non-copyable by explicitly deleting the copy constructor
 * and copy assignment operator. This prevents instances of the class from being
 * copied, which is useful for classes that manage unique resources.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * class FileHandle
 * {
 * public:
 *     FileHandle(const std::string& filename);
 *     ~FileHandle();
 *
 *     TL_DISABLE_COPY(FileHandle)
 *
 *     // Other methods...
 * private:
 *     int file_descriptor;
 * };
 *
 * // Compilation errors:
 * // FileHandle f1("file.txt");
 * // FileHandle f2 = f1;              // Error: deleted copy constructor
 * // f2 = f1;                         // Error: deleted copy assignment
 * \endcode
 *
 * \param Class  The class name for which copy operations should be disabled
 *
 * \note
 * - The macro should be placed in the public section of the class
 * - Use with `TL_DISABLE_MOVE` if move semantics should also be disabled
 * - Requires C++11 or later (uses `= delete`)
 * - More explicit than making the copy constructor private without implementation
 *
 * \see TL_DISABLE_MOVE, TL_DISABLE_COPY_AND_MOVE
 */
#define TL_DISABLE_COPY(Class) \
    Class(const Class &) = delete; \
    Class &operator = (const Class &) = delete;


/*!
 * \brief Disable move constructor and move assignment operator.
 *
 * Marks a class as non-movable by explicitly deleting the move constructor
 * and move assignment operator. This prevents instances of the class from being
 * moved, which is useful for classes with fixed memory locations or special
 * resource management requirements.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * class Singleton
 * {
 * public:
 *     static Singleton& getInstance();
 *
 *     TL_DISABLE_MOVE(Singleton)
 *
 * private:
 *     Singleton() = default;
 *     static Singleton instance;
 * };
 *
 * // Compilation errors:
 * // Singleton s1 = Singleton::getInstance();  // Error: deleted move constructor
 * // std::vector<Singleton> vec;
 * // vec.push_back(std::move(s1));             // Error: move not allowed
 * \endcode
 *
 * \param Class  The class name for which move operations should be disabled
 *
 * \note
 * - The macro should be placed in the public section of the class
 * - Use with `TL_DISABLE_COPY` if copy semantics should also be disabled
 * - Requires C++11 or later (uses `= delete`)
 * - Useful for singleton patterns and objects with fixed addresses
 *
 * \see TL_DISABLE_COPY, TL_DISABLE_COPY_AND_MOVE
 */
#define TL_DISABLE_MOVE(Class) \
    Class(Class &&) = delete; \
    Class &operator = (Class &&) = delete;


/*!
 * \brief Disable both copy and move semantics.
 *
 * Convenience macro that disables both copy and move operations in a single call.
 * Equivalent to calling both `TL_DISABLE_COPY` and `TL_DISABLE_MOVE`.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * class DatabaseConnection
 * {
 * public:
 *     DatabaseConnection(const std::string& connStr);
 *     ~DatabaseConnection();
 *
 *     TL_DISABLE_COPY_AND_MOVE(DatabaseConnection)
 *
 *     void execute(const std::string& query);
 *
 * private:
 *     void* connection_handle;
 * };
 *
 * // All these generate compilation errors:
 * // DatabaseConnection conn1("localhost");
 * // DatabaseConnection conn2 = conn1;        // Error: copy disabled
 * // DatabaseConnection conn3 = std::move(conn1); // Error: move disabled
 * \endcode
 *
 * \param Class  The class name for which copy and move operations should be disabled
 *
 * \note
 * - More concise than calling both `TL_DISABLE_COPY` and `TL_DISABLE_MOVE`
 * - The macro should be placed in the public section of the class
 * - Requires C++11 or later
 * - Typically used for classes managing exclusive resources (files, connections, etc.)
 *
 * \see TL_DISABLE_COPY, TL_DISABLE_MOVE
 */
#define TL_DISABLE_COPY_AND_MOVE(Class) \
    TL_DISABLE_COPY(Class) \
    TL_DISABLE_MOVE(Class)

/*! \} */