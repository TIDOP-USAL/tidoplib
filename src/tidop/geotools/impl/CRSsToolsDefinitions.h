#ifndef CRSSTOOLS_DEFINITIONS_H
#define CRSSTOOLS_DEFINITIONS_H

#define CRSSTOOLS_DEFAULT_CRS                            "EPSG:25830+5782"
#define CRSSTOOLS_DEFAULT_GEOGRAPHIC_CRS                 "EPSG:4258+5782"

#define CRSSTOOLS_ENVIRONMENT_VARIABLE_PROJ_LIB          "PROJ_LIB"
#define CRSSTOOLS_ENVIRONMENT_VARIABLE_GDAL_DATA         "GDAL_DATA"
#define CRSSTOOLS_ENVIRONMENT_VARIABLE_GDAL_DRIVER_PATH  "GDAL_DRIVER_PATH"
#define CRSSTOOLS_SETTINGS_LOCALGEODETICENU_NUMBER_OF_CRS                         "CrsTools/LocalENU/NumberOfCRSs"
#define CRSSTOOLS_SETTINGS_LOCALGEODETICENU_TAG                                   "CrsTools/LocalENU/CRS_"
#define CRSSTOOLS_COORDINATE_ANGLE_PRECISION    9
#define CRSSTOOLS_COORDINATE_LINEAR_PRECISION   4
#define CRSSTOOLS_ANGULAR_INFINITESIMAL_INCREMENT_SECONDS                          0.1
#define CRSSTOOLS_DIFFERENCIAL_DEG_DISTANCE_FOR_DEFORMATIONS          1.0/3600.0
#define CRSSTOOLS_ATYPICAL_MAP_PROJECTION_CONVERGENCE_THRESHOLD       15.0
#define CRSSTOOLS_LONGITUDE_MINIMUM_VALUE            -179.999999999
#define CRSSTOOLS_LONGITUDE_MAXIMUM_VALUE            180.0
#define CRSSTOOLS_LATITUDE_MINIMUM_VALUE            -90.0
#define CRSSTOOLS_LATITUDE_MAXIMUM_VALUE            90.0
#define CRSSTOOLS_ELLIPSOID_ALTITUDE_MINIMUM_VALUE            -199.9
#define CRSSTOOLS_ELLIPSOID_ALTITUDE_MAXIMUM_VALUE            9999.9999

#define CRS_EPSG_AUTHORITY                      "EPSG"
#define CRS_EPSG_CODE_NULL                     -1
#define CRS_ENGINEERING_CRS_EPSG_CODE           0
#define CRS_VERTICAL_EPSG_CODE_ELLIPSOID        -1
#define CRS_VERTICAL_ELLIPSOID_TAG              "Ellipsoid"
#define CRS_VERTICAL_EPSG_CODE_ORTHOMETRIC      0
#define CRS_ID_STRING_EPSG_PREFIX               "EPSG:"
#define CRS_ID_STRING_ENU_PREFIX                "ENU:"
#define CRS_ID_STRING_ENU_STRING_SEPARATOR      ";" // "ENU:4258;-4.495021180808;36.756413127079;142.1590"
#define CRS_TYPE_PROJ_COMPOUND                       "Compound Coordinate Reference System"
#define CRS_TYPE_PROJ_ECEF                           "Geocentric Coordinate Reference System"
#define CRS_TYPE_PROJ_GEODETIC_2D                    "Geographic 2D Coordinate Reference System"
#define CRS_TYPE_PROJ_GEODETIC_3D                    "Geographic 3D Coordinate Reference System"
#define CRS_TYPE_PROJ_PROJECTED                      "Projected Coordinate Reference System"
#define CRS_TYPE_PROJ_VERTICAL                       "Vertical Coordinate Reference System"
#define CRS_TYPE_ECEF                           "ECEF"
#define CRS_TYPE_GEODETIC                       "Geodetic"
#define CRS_TYPE_PROJECTED                      "Projected"
#define CRS_TYPE_VERTICAL                       "Vertical"
#define CRS_TYPE_GEODETIC_COMPOUND              "GeodeticCompound"
#define CRS_TYPE_LOCALCS_WKT                    "LOCAL_CS"
#define CRS_TYPE_LOCALENU                       "Local(ENU)"
#define CRS_TYPE_GLOBAL                         "Global"
#define CRS_TYPE_ECEF_PRECISION              4
#define CRS_TYPE_LOCALGEODETICENU_PRECISION  4
#define CRS_TYPE_GEODETIC_PRECISION          9
#define CRS_TYPE_PROJECTED_PRECISION         4
#define CRS_TYPE_GEODETIC_COMPOUND_PRECISION 9
#define CRS_TYPE_HEIGHT_PRECISION            4
#define CRS_TYPE_ENU_PRECISION              4
#define CRS_USER_TAG                            "User"
#define CRS_USER_EPSGCODE_TAG                   "EPSG"
#define CRS_USER_PROJ4STRING_TAG                "Proj4String"
#define CRS_USER_PROJ4STRING_PREFIX             "+proj="
#define CRS_EQUAL_SEMI_MAJOR_AXIS_TOLERANCE 0.001
#define CRS_EQUAL_SEMI_MINOR_AXIS_TOLERANCE 0.001
#define CRS_EQUAL_PRIME_MERIDIAN_TOLERANCE  0.0000000001

#define CRSSTOOLS_XMLFILE_TAG_ORTHOMETRIC_HEIGHTS                                 "ORTHOMETRIC_HEIGHTS"
#define CRSSTOOLS_XMLFILE_TAG_ORTHOMETRIC_HEIGHT                                  "ORTHOMETRIC_HEIGHT"
#define CRSSTOOLS_XMLFILE_TAG_ELLIPSOID_HEIGHT                                    "ELLIPSOID_HEIGHT"
#define CRSSTOOLS_XMLFILE_TAG_ELLIPSOID_HEIGHTS                                   "ELLIPSOID_HEIGHTS"
#define CRSSTOOLS_ENGINEERING_CRS                                                "Engineering CRS"

#define CRSSTOOLS_WKT_TAG "COMPD_CS"
#define CRSSTOOLS_WKT_GEODETIC_CRS_TAG "GEOGCS"
#define CRSSTOOLS_WKT_PROJECTED_CRS_TAG "PROJCS"
#define CRSSTOOLS_WKT_VERTICAL_CRS_TAG "VERT_CS"
#define CRSSTOOLS_WKT_CS_TAG "CS"

#endif
