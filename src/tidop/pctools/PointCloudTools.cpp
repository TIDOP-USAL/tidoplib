// #include "./impl/PointCloudFileManager.h"
#include "tidop/core/exception.h"
#include "tidop/core/utils.h"

#include "tidop/geotools/GeoTools.h"
#include "tidop/geotools/CRSsTools.h"

#include <proj.h>
#include <copc-lib/las/header.hpp>
#include <lazperf/readers.hpp>
#include <copc-lib/io/copc_reader.hpp>
#include <copc-lib/laz/decompressor.hpp>


#include <io/CopcReader.hpp>
#include <io/LasReader.hpp>
#include <io/LasHeader.hpp>
#include <io/CopcWriter.hpp>
#include <filters/CropFilter.hpp>
#include <filters/ReprojectionFilter.hpp>
#include <filters/SortFilter.hpp>
#include <pdal/SrsBounds.hpp>
#include <pdal/util/FileUtils.hpp>
#include <pdal/private/gdal/GDALUtils.hpp>

#include <pdal/Stage.hpp>
#include <pdal/StageFactory.hpp>
#include <pdal/PipelineManager.hpp>
#include <pdal/util/FileUtils.hpp>

#include "PointCloudTools.h"

using namespace tl;

PointCloudTools* PointCloudTools::mInstance = 0;

PointCloudTools::PointCloudTools()
{
    mPtrGeoTools = GeoTools::getInstance();
    try {
        bool ignoreDeprecatedCRSs = true;
        mPtrGeoTools->initializeCRSsTools(ignoreDeprecatedCRSs);
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }
};

PointCloudTools::~PointCloudTools()
{
    clear();
}

bool PointCloudTools::assignCRS(std::string fileName, std::string crsId, std::string outputFileName)
{
    bool succesFull = false;
    if (mPtrGeoTools == nullptr)
    {
        TL_ASSERT(false, "GeoTools is not defined");
    }
    if (!mPtrGeoTools->ptrCRSsTools()->getIsCRSValid(crsId))
    {
        TL_ASSERT(false, "Invalid CRS Id: {}", crsId);
    }
    std::string pdalCrsId = "25830+4258";
    tl::Path inputFile(fileName);
    if (!inputFile.exists())
    {
        TL_ASSERT(false, "Not exists input file: {}", fileName);
    }
    if (!inputFile.isFile())
    {
        TL_ASSERT(false, "Input is not a file: {}", fileName);
    }
    std::string extension = inputFile.extension().toString();
    tl::Path outputFile = Path(outputFileName);
    if (outputFile.exists())
    {
        if (!outputFile.isFile())
        {
            TL_ASSERT(false, "Output is not a file: {}", outputFileName);
        }
    }
    std::string outputExtension = outputFile.extension().toString();
    if (!compareInsensitiveCase(extension, outputExtension))
    {
        TL_ASSERT(false, "Extensions are different for input and output files");
    }
    if (outputFile.exists())
    {
        tl::Path::removeFile(outputFile);
        if (outputFile.exists())
        {
            TL_ASSERT(false, "Error removing existing output file: {}", outputFileName);
        }
    }
    try
    {
        std::string crsWKT;
        pdal::StringList dimensionsName;
        pdal::PointTable pointTable;
        pdal::PipelineManager pdalPipeline;
        pdal::Options optsReader;
        optsReader.add("filename", fileName);
        if (compareInsensitiveCase(extension, ".las")
            || compareInsensitiveCase(extension, ".laz"))
        {
            bool isCopc = false;
            try
            {
                copc::FileReader copcFile(fileName); // error if file is not copc
                isCopc = true;
                auto las_header = copcFile.CopcConfig().LasHeader();
                auto copc_info = copcFile.CopcConfig().CopcInfo();
                crsWKT = copcFile.CopcConfig().Wkt();
                pdal::Stage& reader = pdalPipeline.addReader("readers.copc");
                pdal::Options optsWriter;
                optsWriter.add("a_srs",crsId);
                pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
            }
            catch (...)
            {
                pdal::LasReader lasReader;
                pdal::Options options;
                options.add("filename", fileName);
                lasReader.setOptions(options);
                lasReader.prepare(pointTable);
                const pdal::LasHeader& h = lasReader.header();
                pdal::SpatialReference crs = h.srs();
                crsWKT = crs.getWKT1();
                pdal::Stage& reader = pdalPipeline.addReader("readers.las");
                reader.setOptions(optsReader);
                if (crsWKT.empty())
                {
                    pdal::Options optsWriter;
                    optsWriter.add("a_srs", crsId);
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.las", reader, optsWriter);
                }
                else
                {
                    std::string inputFileCrsId;
                    mPtrGeoTools->ptrCRSsTools()->setCRSFromWkt(crsWKT, inputFileCrsId);
                    pdal::Stage& filterCrsOperation = pdalPipeline.makeFilter("filters.reprojection", reader);
                    pdal::Options optsFilterCrsOperation;
                    optsFilterCrsOperation.add("in_srs", inputFileCrsId);
                    optsFilterCrsOperation.add("out_srs", pdalCrsId);// crsId);
                    filterCrsOperation.setOptions(optsFilterCrsOperation);
                    pdal::Options optsWriter;
                    optsWriter.add("a_srs", crsId);
                    pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.las", filterCrsOperation, optsWriter);
                }
            }
            try {
                pdal::point_count_t np = pdalPipeline.execute();
            }
            catch (...) {
                TL_THROW_EXCEPTION_WITH_NESTED("");
            };
            succesFull = !std::ifstream(outputFileName).fail();
        }
        else
        {
            TL_ASSERT(false, "Input file must be LAS/LAZ");
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    };
    return(succesFull);
}

void PointCloudTools::clear()
{
    // if (mPtrPointCloudFileManager != NULL)
        // delete(mPtrPointCloudFileManager);
}

bool PointCloudTools::toCOPC(std::string fileName, std::string outputFileName, std::string crsId)
{
    bool succesFull = false;
    if (mPtrGeoTools == nullptr)
    {
        TL_ASSERT(false, "GeoTools is not defined");
    }
    if (!crsId.empty())
    {
        if (!mPtrGeoTools->ptrCRSsTools()->getIsCRSValid(crsId))
        {
            TL_ASSERT(false, "Invalid CRS Id: {}", crsId);
        }
    }
    tl::Path inputFile(fileName);
    if (!inputFile.exists())
    {
        TL_ASSERT(false, "Not exists input file: {}", fileName);
    }
    if (!inputFile.isFile())
    {
        TL_ASSERT(false, "Input is not a file: {}", fileName);
    }
    std::string extension = inputFile.extension().toString();
    tl::Path outputFile = Path(outputFileName);
    if (outputFile.exists())
    {
        if (!outputFile.isFile())
        {
            TL_ASSERT(false, "Output is not a file: {}", outputFileName);
        }
    }
    std::string outputExtension = outputFile.extension().toString();
    if (!compareInsensitiveCase(outputExtension, ".laz"))
    {
        TL_ASSERT(false, "Output file extension must be LAZ");
    }
    if (outputFile.exists())
    {
        tl::Path::removeFile(outputFile);
        if (outputFile.exists())
        {
            TL_ASSERT(false, "Error removing existing output file: {}", outputFileName);
        }
    }
    try
    {
        std::string crsWKT;
        pdal::StringList dimensionsName;
        pdal::PointTable pointTable;
        pdal::PipelineManager pdalPipeline;
        pdal::Options optsReader;
        optsReader.add("filename", fileName);
        if (compareInsensitiveCase(extension, ".las")
            || compareInsensitiveCase(extension, ".laz"))
        {
            bool isCopc = false;
            try
            {
                copc::FileReader copcFile(fileName); // error if file is not copc
                isCopc = true;
                auto las_header = copcFile.CopcConfig().LasHeader();
                auto copc_info = copcFile.CopcConfig().CopcInfo();
                crsWKT = copcFile.CopcConfig().Wkt();
                pdal::Stage& reader = pdalPipeline.addReader("readers.copc");
                pdal::Options optsWriter;
                optsWriter.add("a_srs", crsId);
                pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.copc", reader, optsWriter);
            }
            catch (...)
            {
                pdal::Stage& reader = pdalPipeline.addReader("readers.las");
                reader.setOptions(optsReader);
                pdal::Options optsWriter;
                //optsWriter.add("filename", outputFileName);
                optsWriter.add("a_srs", crsId);
                pdal::Stage& writer = pdalPipeline.makeWriter(outputFileName, "writers.las", reader, optsWriter);
                //pdal::Stage& writer = pdalPipeline.addWriter("writers.las");
                //writer.setOptions(optsWriter);
            }
            pdal::point_count_t np = pdalPipeline.execute();
            succesFull = !std::ifstream(outputFileName).fail();
        }
        else
        {
            TL_ASSERT(false, "Input file must be LAS/LAZ");
        }
    }
    catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    };
    return(succesFull);
}

// PointCloudFileManager* PointCloudTools::ptrPointCloudFileManager()
// {
    // if (mPtrPointCloudFileManager == NULL)
    // {
        // mPtrPointCloudFileManager = new PointCloudFileManagerImpl();
    // }
    // //return(dynamic_cast<CRSsTools*>(mPtrCRSsTools));
    // return(mPtrPointCloudFileManager);
// }
