# TidopLib 4.0.0

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

- [Boost](https://www.boost.org) > 1.70 [![License](https://img.shields.io/badge/License-Boost_1.0-lightblue.svg)](https://www.boost.org/LICENSE_1_0.txt)
- [GDAL](https://gdal.org) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
- [PROJ](https://proj.org/en/9.4/index.html)[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
- [OpenCV](https://opencv.org)[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
- [fmt](https://fmt.dev/latest/index.html) (REQUIRED for C++ Version < 20)
- [OpenBLAS](https://www.openblas.net) (optional) [![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
- [OpenMP](https://www.openmp.org) (optional)
- [Visual Leak Detector](https://marketplace.visualstudio.com/items?itemName=ArkadyShapkin.VisualLeakDetectorforVisualC) (optional) [![License: LGPL v2.1](https://img.shields.io/badge/License-LGPL_v2.1-blue.svg)](https://www.gnu.org/licenses/lgpl-2.1)

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
