[](http://)# TidopLib

<p align="center">
<img src="tidop_logo.png" />
</p>

TidopLib is an open-source C++ library developed by [Tidop Research Group](http://tidop.usal.es)

## Build from sources

### Clone 

```
cd D:/dev
git clone https://github.com/TIDOP-USAL/tidoplib.git
```

### CMake 

TidopLib uses [CMake](https://cmake.org) for configure and build.

From the command line we navigate to the tidoplib directory and create a build directory:

```
cd tidoplib
mkdir build
```

Next, navigate to the build directory and run CMake to configure the project and generate a native build system:

```
cd build
cmake ..
cmake --build .
```

## License
View [LICENSE](LICENSE)