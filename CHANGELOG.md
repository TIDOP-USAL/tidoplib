# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

- Added: para funcionalidades nuevas.
- Changed: para los cambios en las funcionalidades existentes.
- Deprecated: para indicar que una característica está obsoleta y que se eliminará en las próximas versiones.
- Removed: para las características en desuso que se eliminaron en esta versión.
- Fixed: para correcciones y bugs.
- Security: para invitar a los usuarios a actualizar, en el caso de que haya vulnerabilidades.

## [1.1.0] -

### Added

- El código de los modulos se cambia a una carpeta tidop quedando como src/tidop/[modulo]. De esta forma los include incluiran tidop/[modulo]/[...].h
- Operador de asignación StyleLabel
- Añadido ejemplo chrono.cpp
- Generación automatica de la documentación del programa con Doxygen
- Macro TL_TODO para sacar en los mensajes del compilador los mensajes TODO
- Macro para controlar Warnings tambien en Linux
- Añadido método tickCount para reemplazar a getTickCount
- Test conversión de angulos
- Añadida macro TL_NOEXCEPT
- Constructores de movimento y sobrecarga del operador asignación de movimiento en las clases de entidades geométricas.

### Changed
  
- mathutils se saca del modulo core y su contenido se mueve al modulo math en diferentes ficheros.
- Restructuración del modulo de entidades gráficas. Se divide en ficheros independientes
- Lectura y escritura de ficheros vectoriales
- Nuevas clases para el parseo de argumentos de comandos por consola.

### Removed

- Se elimina getAppVersion

### Fixed

- Se añade un bloqueo a la clase RegisterGdal para que sólo haga el registro una vez y no de error al acceder varias veces.

## [1.0.1] - 2018-05-09

### Added

- Constructor de movimiento en la clase
- Se añaden los test
- Se añade el modulo geospatial. Por ahora incluye las clases relaccionadas con los sistema de referencia que se han extraido de transform.
- Añadido modulo vect
- En la clase Font se añade el operador de asignación

### Changed

- Los métodos miembro de setColor y setOutlineColor de la clase StyleSymbol ahora reciben un objeto constante como parámetro.
- Las entidades gráficas ahora son puntos doubles en lugar de float
- En la clase Process se ponen los mensajes en inglés

### Removed

### Fixed

- La clase CanvasCV no debería estar activa cuando no se añade OpenCV como dependencia de la libreria. Se comprueba si esta definida con #ifdef HAVE_OPENCV desactivando el bloque de código encerrado en caso contrario.
- Solucionado bug en operador de asignación de la clase MultiPoint

## [1.0.0] - 2018-03-04

### Added

- Se reestructura la librería actual (TidopLib) para separarla en modulos. 
- De momento se han creado los modulos core, geometry, img, img_process y graphic. 
- Se añaden los ficheros de CMake para montar el proyecto.
- Se añade un proyecto de ejemplo de uso de la librería (se añadiran posteriormente el resto)
- 
