#include <iostream>
#include <memory>
#include <tidop/core/base/property.h>

int main()
{
    using PropertyInt = Property<int>;
    using PropertyString = Property<std::string>;

    // Crear una propiedad de tipo int
    PropertyInt propertyInt(42);
    std::cout << "Initial int value: " << propertyInt.value() << "\n";
    std::cout << "String representation: " << propertyInt.toString() << "\n";

    propertyInt.fromString("100");
    std::cout << "Updated int value: " << propertyInt.value() << "\n";

    // Crear una propiedad de tipo string
    PropertyString propertyString("Hello, world!");
    std::cout << "Initial string value: " << propertyString.value() << "\n";
    std::cout << "String representation: " << propertyString.toString() << "\n";

    propertyString.fromString("Updated value");
    std::cout << "Updated string value: " << propertyString.value() << "\n";

    return 0;
}
