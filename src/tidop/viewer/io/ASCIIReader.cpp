#include "ASCIIReader.h"

#include "tidop/core/base/defs.h"
#include "tidop/core/base/split.h"
#include "tidop/viewer/group/PointCloud.h"

namespace tl
{

ASCIIReader::ASCIIReader(const Path& path)
	 : ModelReader(path) {
}

void ASCIIReader::open()
{
	std::string pathStr = path.toString();
	file = std::ifstream(pathStr);

	std::string line;

	if (isOpen()) {

		std::vector<Vertex> points;

		while (std::getline(file, line)) {

			if (line.rfind("//", 0) == 0) continue;

			std::vector<float> position = split<float>(line, ' ');

			//Vertex vertex(Vector3f{ position[0], position[1], position[2] }, Vector4f{1.0f, 1.0f, 1.0f, 1.0f});
			Vertex vertex(Vector3f{ position[0], position[1], position[2] }, Vector4f{ position[3] / 255, position[4] / 255, position[5] / 255, 1.0f });
			points.push_back(vertex);
		}

		modelBase = PointCloud::New(points);

		close();
	}
}

bool ASCIIReader::isOpen() const
{
	return file.is_open();
}

void ASCIIReader::close()
{
	file.close();
}

}