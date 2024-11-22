#include "LASReader.h"

#include "tidop/geometry/entities/bbox.h"
#include "tidop/viewer/group/PointCloud.h"
#include "tidop/viewer/widget/ViewerWidget.h"

#include <lasreader_las.hpp>

// DHL
#include <tidop/geotools/GeoTools.h>
#include <tidop/pctools/PointCloudTools.h>
#include <tidop/pctools/PointCloudReader.h>
//DHL

namespace tl
{

	LASReader::LASReader(const Path& path)
		: ModelReader(path) {
	}

	void LASReader::open()
	{
		std::string pathStr = path.toString();
		Path outputPointsPath = path;
		outputPointsPath.replaceExtension(".txt");
		std::string outputPointsFileName = outputPointsPath.toString();
		std::ofstream ofs;
		ofs.open(outputPointsFileName, std::ofstream::out | std::ofstream::trunc);

		std::vector<Vertex> points;
		PointCloudReader::Ptr pointCloudReader;
		pointCloudReader = PointCloudReaderFactory::create(pathStr);
		pointCloudReader->open();
		std::string crsId_25830_5782 = "EPSG:25830+5782";
		std::string crsId_4937 = "EPSG:4937";
		std::string crsId_4258_5782 = "EPSG:4258+5782";
		// 25830+5782
		double x_min = 598400.0;
		double y_min = 4264600.0;
		double z_min = 487.0;
		double x_max = 598800.0;
		double y_max = 4265000.0;
		double z_max = 781.0;
		// 4258+5782
		//double x_min = -1.87111208185624056;
		//double y_min = 38.52431347581596555;
		//double z_min = 487.0;
		//double x_max = -1.86652393733056021;
		//double y_max = 38.52796193315600703;
		//double z_max = 781.0;
		double x_o, y_o, z_o;
		std::vector<std::string> dimensionsNames;
		pointCloudReader->getDimensionsNames(dimensionsNames);
		int posRed = -1;
		int posGreen = -1;
		int posBlue = -1;
		int posAlpha = -1;
		int i = 0;
		for (auto dimensionName : dimensionsNames)
		{
			if (compareInsensitiveCase("red", dimensionName))
				posRed = i;
			if (compareInsensitiveCase("green", dimensionName))
				posGreen = i;
			if (compareInsensitiveCase("blue", dimensionName))
				posBlue = i;
			if (compareInsensitiveCase("alpha", dimensionName))
				posAlpha = i;
			i++;
		}
		double xmin_25830_5782, ymin_25830_5782, zmin_25830_5782;
		double xmax_25830_5782, ymax_25830_5782, zmax_25830_5782;
		pointCloudReader->getBoundingBox(xmin_25830_5782, ymin_25830_5782, zmin_25830_5782,
			xmax_25830_5782, ymax_25830_5782, zmax_25830_5782, crsId_25830_5782);
		double xmin_4937, ymin_4937, zmin_4937;
		double xmax_4937, ymax_4937, zmax_4937;
		pointCloudReader->getBoundingBox(xmin_4937, ymin_4937, zmin_4937,
			xmax_4937, ymax_4937, zmax_4937, crsId_4937);
		std::map<int, double> resolutionByLevel;
		pointCloudReader->copcGetResolutionByLevel(resolutionByLevel);
		//double resolution = resolutionByLevel[0];
		double resolution = resolutionByLevel[0];// 0.05;
		std::vector<std::vector<float> > coordinates;
		std::vector<std::vector<float> > dimensionsValues;
		pointCloudReader->getPoints(x_o, y_o, z_o, coordinates,
			dimensionsNames, dimensionsValues,
			x_min, y_min, z_min, x_max, y_max, z_max, resolution, crsId_25830_5782);// crsId_25830_5782);

		int np = 0;
		double x_min_real = 1000000000.;
		double y_min_real = 1000000000.;
		double z_min_real = 1000000000.;
		double x_max_real = -1000000000.;
		double y_max_real = -1000000000.;
		double z_max_real = -1000000000.;
		for (auto pto : coordinates)
		{
			if (pto[0] < x_min_real) x_min_real = pto[0];
			if (pto[1] < y_min_real) y_min_real = pto[1];
			if (pto[2] < z_min_real) z_min_real = pto[2];
			if (pto[0] > x_max_real) x_max_real = pto[0];
			if (pto[1] > y_max_real) y_max_real = pto[1];
			if (pto[2] > z_max_real) z_max_real = pto[2];
			Vector3f position = {
				static_cast<float>(pto[0],pto[1],pto[2])
			};
			if (ViewerWidget::isEnabledSwitchAxis()) {
				position = {
					-static_cast<float>(pto[0]),
					static_cast<float>(pto[2]),
					static_cast<float>(pto[1])
				};
			}
			Vector4f color = {
				static_cast<float>(0.),
				static_cast<float>(0.),
				static_cast<float>(0.),
				static_cast<float>(1.)
			};
			if (posRed != -1 && posGreen != -1 && posBlue != -1)
			{
				float alpha = 65536.;
				if (posAlpha != -1)
					alpha = dimensionsValues[np][posAlpha];
				color = {
					static_cast<float>(dimensionsValues[np][posRed] / 65536.),
					static_cast<float>(dimensionsValues[np][posGreen] / 65536.),
					static_cast<float>(dimensionsValues[np][posBlue] / 65536.),
					static_cast<float>(alpha / 65536.)
				};
			}
			Vertex vertex(position, color);
			points.push_back(vertex);
			ofs << pto[0] << ", " << pto[1] << ", " << pto[2] << ", ";
			if (posRed != -1 && posGreen != -1 && posBlue != -1)
			{
				ofs << (dimensionsValues[np][posRed] / 65536.) << ", ";
				ofs << (dimensionsValues[np][posGreen] / 65536.) << ", ";
				ofs << (dimensionsValues[np][posBlue] / 65536.);
			}
			ofs << "\n";
			np++;
		}
		Point3<double> min(x_min_real, y_min_real, z_min_real);
		Point3<double> max(x_max_real, y_max_real, z_max_real);
		BoundingBox<Point3<double>> bbox(min, max);
		Vector3d offset = bbox.center().vector();
		//offset = { 0.0, 0.0, 0.0 };
		modelBase = PointCloud::New(points);
		modelBase->setOffset(offset);

		/*
		std::string pathStr = path.toString();
		file = std::ifstream(pathStr, std::ios_base::binary);

		LASreaderLAS lasreader;
		lasreader.open(file);

		LASheader& lasheader = lasreader.header;

		Point3<double> min(lasheader.min_x, lasheader.min_y, lasheader.min_z);
		Point3<double> max(lasheader.max_x, lasheader.max_y, lasheader.max_z);

		BoundingBox<Point3<double>> bbox(min, max);

		Point3d scale_factor(lasreader.header.x_scale_factor, lasreader.header.y_scale_factor, lasreader.header.z_scale_factor);
		Vector3d offset = bbox.center().vector();

		std::vector<Vertex> points;
		while (lasreader.read_point()) {

			const LASpoint& laspoint = lasreader.point;

			Vector3f position = {
				static_cast<float>(laspoint.get_x() - offset.x()),
				static_cast<float>(laspoint.get_y() - offset.y()),
				static_cast<float>(laspoint.get_z() - offset.z())
			};

			if(ViewerWidget::isEnabledSwitchAxis()) {
				position = {
					-static_cast<float>(laspoint.get_x() - offset.x()),
					static_cast<float>(laspoint.get_z() - offset.z()),
					static_cast<float>(laspoint.get_y() - offset.y())
				};
			}

			Vector4f color = {
				static_cast<float>(laspoint.rgb[0] / 65536.),
				static_cast<float>(laspoint.rgb[1] / 65536.),
				static_cast<float>(laspoint.rgb[2] / 65536.),
				static_cast<float>(laspoint.rgb[3] / 65536.)
			};

			Vertex vertex(position, color);
			points.push_back(vertex);
		}
		modelBase = PointCloud::New(points);
		modelBase->setOffset(offset);
		*/
		close();
	}

	bool LASReader::isOpen() const
	{
		return file.is_open();
	}

	void LASReader::close()
	{
		file.close();
	}

}