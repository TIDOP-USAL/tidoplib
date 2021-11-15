# Guía de estilo {#style_guide}

## Convención de nombres

Con el fin de reducir el esfuerzo necesario para leer y entender el código fuente, así como de mejorar la apariencia del código fuente se define un conjunto de reglas para la elección de los nombres de variables, clases, funciones, etc. Estas normas están orientadas a una uniformización del código que repercuta en una mejor legibilidad del mismo por parte de todos los desarrolladores.

Para definir los nombres se usará fundamentalmente el estilo CamelCase y en algunos casos concretos (datos miembro) combinado con la notación húngara.

#### Notación húngara

En programación informática, la notación húngara es un sistema usado normalmente para crear los nombres de variables. Consiste en prefijos en minúsculas que se añaden a los nombres de las variables y que indican su tipo. El resto del nombre indica, lo más claramente posible, la función que realiza la variable.

#### CamelCase

Como criterio general se usará el estilo CamelCase o capitalización medial ya que si el identificador está compuesto de mas de una palabra al separarlas con mayúsculas aumenta la legibilidad. Existen dos tipos de CamelCase:

- UpperCamelCase, cuando la primera letra de cada una de las palabras es mayúscula. Ejemplo: *UpperCamelCase*.
- lowerCamelCase, igual que la anterior con la excepción de que la primera letra es minúscula. Ejemplo: *lowerCamelCase*.

Se procurará evitar abreviaturas en los nombres a menos que éstas identifiquen de manera inequívoca al elemento al que hacemos referencia, en cuyo caso se aconseja su uso. Por ejemplo xml, wms, etc, son abreviaturas cuyo uso está más ampliamente extendido que su forma larga. Otros casos que se puede abreviar sin pérdida de claridad son por ejemplo:

- config o cfg por configuration.
- dist por distance.

### Variables y datos miembro

Se deben evitar los nombres de variables de un solo carácter excepto para las variables temporales que se limitan a una zona muy concreta del código como bucles *for*. Ejemplos comunes de este tipo de variables son *i*, *j*, *k*, *m*, *n*, *x*, *y* o *z*. Aún así siempre es preferible usar algún identificador que aporte más claridad ya que podemos tener varias estructuras de control anidadas. En este caso si son válidos e incluso recomendables los identificadores cortos:


```cpp
for (int imln = 0; imln < multiLine.size(); imln++ ) {
  ...
  for (int iln = 0; iln < multiLine[imln].size(); iln++ ) {
    ...
  }
  ...
}
```

Los datos miembro comenzarán con una *m* o con una *s* si son estáticos.

```cpp
static std::string sVariableStatic;
double mVariable;
```

#### Variables de retorno

Se nombraran con una letra correspondiente al tipo de dato (si se trata de tipos básicos), con una barra baja y la palabra return. Por ejemplo:

```cpp
bool b_return = false;
int i_return = 0;
double d_return = 0.0;
```

#### Clases

Los nombres de las clases irán en formato *UpperCamelCase*.

#### Métodos miembro

Los métodos miembro deben ser verbos (sólos o acompañados de una o varias palabras) en estilo *lowerCamelCase*. Es decir que deben ser de la forma:

```cpp
run();
close();
open();
readXml();
writeXml();
```

Encontramos tres casos especiales en los cuales se utilizarán unos prefijos preestablecidos:

1. A los que se les pase algún parámetro utilizarán el prefijo set: void setParameters( int param1, double param2 );
1. Los que devuelvan algún valor utilizarán el prefijo get: int getValue( );
1. Los métodos que comprueban una condición o un valor booleano se precederán del prefijo is: bool isBoolean();

En casos de funciones que realicen algún tipo de conversión se empleará como separador *To* en lugar del también frecuentemente utilizado *2*. Es decir se usará:

```cpp
convertFloatToDoble();
```

en lugar de:

```cpp
convertFloat2Doble(); 
```

#### Constantes

Los nombres de las constantes se deben escribir en mayúsculas separadas por guiones bajos.

```cpp
const char *LOG_FILE = “[nombre_fichero_log]”; 
```

##Estructura de los archivos de cabecera

En los archivos de cabecera se añadirá la declaración de las clases, de las estructuras, funciones, enumeraciones y constantes.

En el caso de usar *templates*, si sus métodos se definen fuera del cuerpo de la clase se definirán también en el archivo de cabecera y además deberán ser *inline*.

### Include guards
Los archivos de cabecera (.h) comenzarán siempre con un include guards o guardián de inclusión múltiple, que es algo como:

```cpp
#ifndef TL_NOMBREARCHIVO_H
#define TL_NOMBREARCHIVO_H
```

Y terminarán cerrando con *#endif* y con el nombre de la macro comentado para saber que bloque estamos cerrando:

```cpp
#endif // TL_NOMBREARCHIVO_H
```

Un include guards no es más que una forma de evitar nuevas definiciones si un archivo de cabecera se incluye mas de una vez. La primera vez que se incluye la macro TL_NOMBREARCHIVO_H no está definida aún y por tanto el preprocesador entra en el bloque de código definiendo la macro en la siguiente línea. La siguiente vez que se incluya el archivo de cabecera, como ya existe dicha macro, el preprocesador salta hasta al *#endif* ignorando todo el código encerrado en el bloque.

El compilador MS VC++ dispone de un pragma específico para este fin: *#pragma once*. Sirve para indicar al compilador que el fichero, en que está incluida dicha directiva, debe ser incluido solo una vez en cada compilación.

```cpp
#pragma once
```

### Nombre de la macro

El nombre de la macro que utilizamos como include guard comenzará siempre con TL (siglas de Tidop Lib) seguido por un guión bajo, el nombre del fichero, otro guión bajo y una H. Todas las letras irán en mayúsculas.

#include
La inclusión de las cabeceras se hace mediante la directiva del preprocesador #include. Las inclusiones se harán al comienzo del archivo justo detrás del include guard. Primero se añadirán las cabeceras estándar, ya sean de C++ o de C, después las de las librerías de terceros y finalmente las propias.

```cpp
#ifndef TL_NOMBREARCHIVO_H
#define TL_NOMBREARCHIVO_H
#include <iostream>
#include <string>

//cabeceras propias
#include "core/defs.h"
```

Cuando las cabeceras se incluyan con ruta relativa (core/defs.h) se utilizará siempre (/) en lugar de (\\). En Visual Studio la última opción es la que reconoce (nos muestra las sugerencias de los archivos que hay en esa ruta) pero esa barra da problemas en Linux y por tanto es mejor usar la otra opción. Tambien hay que distinguir entre mayusculas y minusculas ya que en Linux se distingue entre ambas.

### La diferencia entre “” y <>

```cpp
#include <iostream>
#include “core.h”
```

Como se puede ver en los includes de arriba de estas líneas se pueden incluir los archivos con dos delimitadores diferentes. En el primer caso la directiva *#include* lo que hace es buscar entre todos los directorios que se han especificado en apartado include del compilador. En el caso de las comillas la diferencia es que la búsqueda se realiza en primer lugar en el mismo directorio del fichero que se está compilando y posteriormente en el resto de directorios.

En el caso de nuestras propias cabeceras se utilizará la versión con comillas.

### Cabeceras estándar

Las cabeceras estándar de C++ ahora van sin el *.h*. Las de la librería estándar de C, por razones de estandarización, ahora se deben incluir sin el *.h*, y con la letra *c* como prefijo.



|Cabecera antigua|Nueva cabecera|
|----------------|--------------|
|iostream.h      |iostream      |
|string.h        |string        |
|stdlib.h	     |cstdlib       |
|stdio.h	     |cstdio        |

### forward declarations

La declaración forward o declaración incompleta de clases se usa para evitar tener que incluir una definición de tipo que sólo aparezca como puntero o como referencia en la cabecera.

```cpp
class Centro;

class Circulo
{
  Centro *center();
};
```
