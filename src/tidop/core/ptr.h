#pragma once

#include <iostream>
#include <memory>

#define GENERATE_PTR(ptr, clazz) public: \
                                using Ptr = std:: ptr <clazz>; \
                                template<class... Args> \
                                inline static Ptr New(Args&&... args) { \
                                    return std::make_shared<clazz>(std::forward<Args>(args)...); \
                                }\

#define GENERATE_SHARED_PTR(clazz) GENERATE_PTR(shared_ptr, clazz)

#define GENERATE_UNIQUE_PTR(clazz) GENERATE_PTR(unique_ptr, clazz)

namespace tl
{

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

}