Main Page {#mainpage}
==============

![TidopLib Logo](tidop_logo.png)

TidopLib 4.0.0
==============

TidopLib is an open-source C++ library developed by [Tidop Research Group](http://tidop.usal.es).

[![License: LGPL v3](https://img.shields.io/badge/License-LGPL_v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)

Modules
------------

- @ref Core
- @ref MathTools
- @ref Features
- @ref Geometry
- @ref GeoTools
- @ref GraphicEntities
- @ref RasterTools
- @ref VectorTools
- @ref PointCloudTools

## Getting Started

### Prerequisites


* **[CMake](https://cmake.org/) v3.10+**
* **C++ Compiler** with support for C++14
* Git

### Dependencies

| Library | Version Requirement | Required/Optional | License |
|---------|---------------------|------|---------|
| [Boost](https://www.boost.org) | > 1.70 | Required | [![License](https://img.shields.io/badge/License-Boost_1.0-lightblue.svg)](https://www.boost.org/LICENSE_1_0.txt) |
| [fmt](https://fmt.dev/latest/index.html) | - | Required (1) | [![License](https://img.shields.io/badge/License-%7Bfmt%7D-%20orange)](https://github.com/fmtlib/fmt/blob/master/LICENSE) |
| [GDAL](https://gdal.org) | > 3.5 | Opcional (2, 3, 4) | [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) |
| [PROJ](https://proj.org/en/9.4/index.html) | > 8.0 | Opcional (2) | [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) |
| [OpenCV](https://opencv.org) | > 3.0 | Opcional (3, 5) | [![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) |
| [OpenBLAS](https://www.openblas.net) | - | Opcional | [![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) |
| [OpenMP](https://www.openmp.org) | - | Opcional | - |
| [Visual Leak Detector](https://marketplace.visualstudio.com/items?itemName=ArkadyShapkin.VisualLeakDetectorforVisualC) | - | Opcional | [![License: LGPL v2.1](https://img.shields.io/badge/License-LGPL_v2.1-blue.svg)](https://www.gnu.org/licenses/lgpl-2.1) |
| [laz-perf](https://github.com/hobu/laz-perf) | - | Opcional (5) | [![License: BSD-2-Clause](https://img.shields.io/badge/License-BSD_2--Clause-orange.svg)](https://opensource.org/licenses/BSD-2-Clause) |
| [PDAL](https://pdal.io) | - | Opcional (5) | [![License: BSD](https://img.shields.io/badge/License-BSD-blue.svg)](https://opensource.org/licenses/BSD-3-Clause) |
| [copc-lib](https://github.com/copcio/copc-lib) | - | Opcional (5) | [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) |

---

**Notes:**  
1. `fmt` is **required only if the compiler uses C++ < 20**.
2. Required for the GeoTools module. 
3. Required for the RasterTools module.
4. Required for the VectorTools module.
5. Required for the FeaureMatching modules. 
6. Required for the PointCloudTools module. 

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

TidopLib is distributed under the GNU Lesser General Public License Version 3 (LGPL 3).