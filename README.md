# TidopLib 3.0.0

<p align="center">
<img src="tidop_logo.png" />
</p>

[![License: LGPL v3](https://img.shields.io/badge/License-LGPL_v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)

TidopLib is an open-source C++ library developed by [Tidop Research Group](http://tidop.usal.es)

## Getting Started

### Prerequisites


* **[CMake](https://cmake.org/) v3.10+**
* **C++ Compiler** with support for C++14
* Git

### Dependencies

- [Boost](https://www.boost.org) > 1.70
- [GDAL](https://gdal.org)
- [PROJ](https://proj.org/en/9.4/index.html)
- [OpenCV](https://opencv.org)
- [fmt](https://fmt.dev/latest/index.html) (REQUIRED for C++ Version < 20)
- [OpenBLAS](https://www.openblas.net) (optional)
- [OpenMP](https://www.openmp.org) (optional)
- [Visual Leak Detector](https://marketplace.visualstudio.com/items?itemName=ArkadyShapkin.VisualLeakDetectorforVisualC) (optional)

### Installing

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

TidopLib is distributed under the GNU Lesser General Public License Version 3 (LGPL 3). Please see the [LICENSE](LICENSE) file for more information.