Página principal {#mainpage}
==============

Introducción
------------


### Estructura del proyecto

El proyecto se estructura actualmente en los siguientes directorios:

- apps: Aplicaciones desarrolladas con la librería
- data: Datos para pruebas, test, etc.
- doc: Documentación de la aplicación generada con Doxygen
- examples: Código de ejemplo
- props_vs: Archivos de hojas de propiedades de proyecto de Visual Studio
- QtPrj: Archivos de proyecto de Qt Creator
- src: Código de la librería Tidod incluyendo el archivo de proyecto de Visual Studio
- test: Proyectos de test
- third_party: librerías (siempre que su tamaño lo recomiende) y código de terceros.

### Modulos

- @ref angleConversion
- @ref utilities
- @ref FeatDetection
- @ref GeometricEntities
- @ref ImgProc
- @ref trfGroup

### Tutoriales

@ref tutorials

### Instalación

#### Prerrequisitos

La librería **TidopLib** hace uso de una serie de librerias externas. En función de su activación habrá ciertas funcionalidades que estarán disponibles o no.

Para la instalación de la libreria se hara uso de la herramienta CMake.

#### Listado de librerías

- Eigen 3 (Opcional)
- Boost (Requerida)
- OpenCV (Opcional)
- Cuda (Opcional - Requerida en función de la compilación de OpenCV)
- GDAL (Opcional)
- GoogleTest

#### Instalación

##### Instalación en Windows

Se clona el repositorio o bien utilizando algún cliente como TortoiseGit o SourceTree o directamente desde la consola. Para utilzar algún cliente se selecciona la opción de clonar el repositorio indicando la url del repositorio:

```
https://[nombre_usuario]@bitbucket.org/tidop/tidoplib.git
```

Desde la consola vamos al directorio donde queremos situar el repositorio y se clona:

```
cd D:/desarrollo
git clone https://[nombre_usuario]@bitbucket.org/tidop/tidoplib.git
```

Con CMake se crea el proyecto para Visual Studio (2013 o superior).

##### Instalación en Linux

Se va al directorio donde se quiere descargar el código y se clona el repositorio:

```
cd ~/desarrollo
git clone https://[nombre_usuario]@bitbucket.org/tidop/tidoplib.git
```

###### Compilación con CMake

Una vez clonado se crea una carpeta build para realizar la compilación con CMake:

```
cd tidoplib
mkdir build
```

###### Compilación desde Qt Creator

