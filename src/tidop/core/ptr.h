#pragma once

#include <iostream>
#include <memory>

#define GENERATE_PTR(ptr, make, clazz) public: \
                                using Ptr = std:: ptr <clazz>; \
                                template<class... Args> \
                                inline static Ptr New(Args&&... args) { \
                                    return std:: make <clazz>(std::forward<Args>(args)...); \
                                }\

#define GENERATE_SHARED_PTR(clazz) GENERATE_PTR(shared_ptr, make_shared, clazz)

#define GENERATE_UNIQUE_PTR(clazz) GENERATE_PTR(unique_ptr, make_unique, clazz)

namespace tl
{

template<typename Base, typename T>
bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

}