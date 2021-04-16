# TidopLib

Libreria C++ del grupo Tidop

## Primeros pasos


### Prerrequisitos

La librería TidopLib hace uso de una serie de librerias. En función de su activación habrá ciertas funcionalidades que estarán disponibles o no.

Para la instalación de la libreria se hara uso de la herramienta [CMake](https://cmake.org).

#### Listado de librerías

* [Eigen 3](https://bitbucket.org/tidop/tidoplib/wiki/Eigen%203) (Opcional)
* [Boost](https://bitbucket.org/tidop/tidoplib/wiki/Boost) (Requerida)
* [OpenCV](https://bitbucket.org/tidop/tidoplib/wiki/OpenCV) (Opcional)
* [Cuda](https://bitbucket.org/tidop/tidoplib/wiki/Cuda) (Opcional - Requerida en función de la compilación de OpenCV)
* [GDAL](http://www.gdal.org) (Opcional)
*

### Instalación

#### Instalación en Windows

Se clona el repositorio o bien utilizando algún cliente como [TortoiseGit](https://tortoisegit.org) o [SourceTree](www.sourcetreeapp.com) o directamente desde la consola. Para utilizar algún cliente se selecciona la opción de clonar el repositorio indicando la url del repositorio:

```
https://.../tidop/tidoplib.git
```

Desde la consola vamos al directorio donde queremos situar el repositorio y se clona:

```
cd D:/dev
git clone https://.../tidop/tidoplib.git
```

La instalación se hara con CMake.

#### Instalación en Linux (Pendiente)

Se va al directorio donde se quiere descargar el código y se clona el repositorio:

```
cd ~/desarrollo
git clone https://.../tidop/tidoplib.git
```

Una vez clonado se crea una carpeta build para realizar la compilación con CMake:

```
cd tidoplib
mkdir build
```

## Running the tests

Para los test se utiliza el framework Boost Test.

## Versioning

We use [SemVer](https://semver.org/lang/es/) for versioning.
