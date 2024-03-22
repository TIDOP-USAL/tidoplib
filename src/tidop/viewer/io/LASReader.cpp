#include "LASReader.h"

#include "tidop/geometry/entities/bbox.h"
#include "tidop/viewer/group/PointCloud.h"

#include <lasreader_las.hpp>

namespace tl
{

	LASReader::LASReader(const Path& path)
		: ModelReader(path) {
	}

	void LASReader::open()
	{
		std::string pathStr = path.toString();
		file = std::ifstream(pathStr, std::ios_base::binary);

		LASreaderLAS lasreader;
		lasreader.open(file);

		LASheader& lasheader = lasreader.header;

		Point3<double> min(lasheader.min_x, lasheader.min_y, lasheader.min_z);
		Point3<double> max(lasheader.max_x, lasheader.max_y, lasheader.max_z);

		BoundingBox<Point3<double>> bbox(min, max);

		tl::Point3d scale_factor(lasreader.header.x_scale_factor, lasreader.header.y_scale_factor, lasreader.header.z_scale_factor);
		tl::Point3d offset = bbox.center();

		std::vector<Vertex> points;
		while (lasreader.read_point()) {

			const LASpoint& laspoint = lasreader.point;

			Vertex vertex(
				Vector3f{ 
					static_cast<float>(laspoint.get_x() - offset.x), 
					static_cast<float>(laspoint.get_y() - offset.y), 
					static_cast<float>(laspoint.get_z() - offset.z) 
				}, Vector4f{
					static_cast<float>(laspoint.rgb[0] / 65536.), 
					static_cast<float>(laspoint.rgb[1] / 65536.),
					static_cast<float>(laspoint.rgb[2] / 65536.),
					static_cast<float>(laspoint.rgb[3] / 65536.)
				}
			);

			points.push_back(vertex);
		}

		modelBase = PointCloud::New(points);

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