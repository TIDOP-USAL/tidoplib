
#include "CRSsUtils.h"

namespace tl
{

    bool findCaseInsensitiveRegex(const std::string& completeString, const std::string& toSearch)
    {
        std::regex pattern(toSearch, std::regex_constants::icase);
        return std::regex_search(completeString, pattern);
    }

    std::vector<std::string> split(std::string str, char delim)
    {
        std::vector<std::string> result;
        auto left = str.begin();
        for (auto it = left; it != str.end(); ++it)
        {
            if (*it == delim)
            {
                result.emplace_back(&*left, it - left);
                left = it + 1;
            }
        }
        if (left != str.end())
            result.emplace_back(&*left, str.end() - left);
        return result;
    }

    std::string to_string_with_precision(double a_value, const int n)
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << a_value;
        return std::move(out).str();
    }

    std::string pjTypeToString(PJ_TYPE type)
    {
        switch (type)
        {
        case PJ_TYPE_UNKNOWN:
            return "Unknown";
        case PJ_TYPE_ELLIPSOID:
            return "Ellipsoid";
        case PJ_TYPE_PRIME_MERIDIAN:
            return "Prime Meridian";
        case PJ_TYPE_GEODETIC_REFERENCE_FRAME:
            return "Geodetic Reference Frame";
        case PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME:
            return "Dynamic Geodetic Reference Frame";
        case PJ_TYPE_VERTICAL_REFERENCE_FRAME:
            return "Vertical Reference Frame";
        case PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME:
            return "Dynamic Vertical Reference Frame";
        case PJ_TYPE_DATUM_ENSEMBLE:
            return "Datum Ensemble";
        case PJ_TYPE_CRS:
            return "Coordinate Reference System";
        case PJ_TYPE_GEODETIC_CRS:
            return "Geodetic Coordinate Reference System";
        case PJ_TYPE_GEOCENTRIC_CRS:
            return "Geocentric Coordinate Reference System";
        case PJ_TYPE_GEOGRAPHIC_CRS:
            return "Geographic Coordinate Reference System";
        case PJ_TYPE_GEOGRAPHIC_2D_CRS:
            return "Geographic 2D Coordinate Reference System";
        case PJ_TYPE_GEOGRAPHIC_3D_CRS:
            return "Geographic 3D Coordinate Reference System";
        case PJ_TYPE_VERTICAL_CRS:
            return "Vertical Coordinate Reference System";
        case PJ_TYPE_PROJECTED_CRS:
            return "Projected Coordinate Reference System";
        case PJ_TYPE_COMPOUND_CRS:
            return "Compound Coordinate Reference System";
        case PJ_TYPE_TEMPORAL_CRS:
            return "Temporal Coordinate Reference System";
        case PJ_TYPE_ENGINEERING_CRS:
            return "Engineering Coordinate Reference System";
            //case PJ_TYPE_PARAMETRIC_CRS:
            //    return "Parametric Coordinate Reference System";
        case PJ_TYPE_OTHER_CRS:
            return "Other Coordinate Reference System";
        case PJ_TYPE_CONVERSION:
            return "Conversion";
        case PJ_TYPE_TRANSFORMATION:
            return "Transformation";
        case PJ_TYPE_CONCATENATED_OPERATION:
            return "Concatenated Operation";
        case PJ_TYPE_OTHER_COORDINATE_OPERATION:
            return "Other Coordinate Operation";
            //case PJ_TYPE_DATUM:
            //    return "Datum";
            //case PJ_TYPE_COORDINATE_OPERATION:
            //    return "Coordinate Operation";
            //case PJ_TYPE_COORDINATE_SYSTEM:
            //    return "Coordinate System";
            //case PJ_TYPE_AXIS:
            //    return "Axis";
            //case PJ_TYPE_UNIT_OF_MEASURE:
            //    return "Unit of Measure";
            //case PJ_TYPE_ELLIPSOIDAL_CS:
            //    return "Ellipsoidal Coordinate System";
            //case PJ_TYPE_CARTESIAN_CS:
            //    return "Cartesian Coordinate System";
            //case PJ_TYPE_SPHERICAL_CS:
            //    return "Spherical Coordinate System";
            //case PJ_TYPE_VERTICAL_CS:
            //    return "Vertical Coordinate System";
            //case PJ_TYPE_TEMPORAL_CS:
            //    return "Temporal Coordinate System";
            //case PJ_TYPE_PARAMETRIC_CS:
            //    return "Parametric Coordinate System";
            //case PJ_TYPE_ORDINATE_CS:
            //    return "Ordinate Coordinate System";
        default:
            return "Unknown";
        }
    }

    PJ_TYPE stringToPjType(std::string strType)
    {
        if (strType == "Unknown")
            return PJ_TYPE_UNKNOWN;
        else if (strType == "Ellipsoid")
            return PJ_TYPE_ELLIPSOID;
        else if (strType == "Prime Meridian")
            return PJ_TYPE_PRIME_MERIDIAN;
        else if (strType == "Geodetic Reference Frame")
            return PJ_TYPE_GEODETIC_REFERENCE_FRAME;
        else if (strType == "Dynamic Geodetic Reference Frame")
            return PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME;
        else if (strType == "Vertical Reference Frame")
            return PJ_TYPE_VERTICAL_REFERENCE_FRAME;
        else if (strType == "Dynamic Vertical Reference Frame")
            return PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME;
        else if (strType == "Datum Ensemble")
            return PJ_TYPE_DATUM_ENSEMBLE;
        else if (strType == "Coordinate Reference System")
            return PJ_TYPE_CRS;
        else if (strType == "Geodetic Coordinate Reference System")
            return PJ_TYPE_GEODETIC_CRS;
        else if (strType == "Geocentric Coordinate Reference System")
            return PJ_TYPE_GEOCENTRIC_CRS;
        else if (strType == "Geographic Coordinate Reference System")
            return PJ_TYPE_GEOGRAPHIC_CRS;
        else if (strType == "Geographic 2D Coordinate Reference System")
            return PJ_TYPE_GEOGRAPHIC_2D_CRS;
        else if (strType == "Geographic 3D Coordinate Reference System")
            return PJ_TYPE_GEOGRAPHIC_3D_CRS;
        else if (strType == "Vertical Coordinate Reference System")
            return PJ_TYPE_VERTICAL_CRS;
        else if (strType == "Projected Coordinate Reference System")
            return PJ_TYPE_PROJECTED_CRS;
        else if (strType == "Compound Coordinate Reference System")
            return PJ_TYPE_COMPOUND_CRS;
        else if (strType == "Temporal Coordinate Reference System")
            return PJ_TYPE_TEMPORAL_CRS;
        else if (strType == "Engineering Coordinate Reference System")
            return PJ_TYPE_ENGINEERING_CRS;
        //case PJ_TYPE_PARAMETRIC_CRS:
        //    return "Parametric Coordinate Reference System";
        else if (strType == "Other Coordinate Reference System")
            return PJ_TYPE_OTHER_CRS;
        else if (strType == "Conversion")
            return PJ_TYPE_CONVERSION;
        else if (strType == "Transformation")
            return PJ_TYPE_TRANSFORMATION;
        else if (strType == "Concatenated Operation")
            return PJ_TYPE_CONCATENATED_OPERATION;
        else if (strType == "Other Coordinate Operation")
            return PJ_TYPE_OTHER_COORDINATE_OPERATION;
        //case PJ_TYPE_DATUM:
        //    return "Datum";
        //case PJ_TYPE_COORDINATE_OPERATION:
        //    return "Coordinate Operation";
        //case PJ_TYPE_COORDINATE_SYSTEM:
        //    return "Coordinate System";
        //case PJ_TYPE_AXIS:
        //    return "Axis";
        //case PJ_TYPE_UNIT_OF_MEASURE:
        //    return "Unit of Measure";
        //case PJ_TYPE_ELLIPSOIDAL_CS:
        //    return "Ellipsoidal Coordinate System";
        //case PJ_TYPE_CARTESIAN_CS:
        //    return "Cartesian Coordinate System";
        //case PJ_TYPE_SPHERICAL_CS:
        //    return "Spherical Coordinate System";
        //case PJ_TYPE_VERTICAL_CS:
        //    return "Vertical Coordinate System";
        //case PJ_TYPE_TEMPORAL_CS:
        //    return "Temporal Coordinate System";
        //case PJ_TYPE_PARAMETRIC_CS:
        //    return "Parametric Coordinate System";
        //case PJ_TYPE_ORDINATE_CS:
        //    return "Ordinate Coordinate System";
        else
            return PJ_TYPE_UNKNOWN;
    }
}