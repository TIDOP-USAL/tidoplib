#pragma once

#include <fstream>

#include "ModelReader.h"

namespace tl
{

	class LASReader : public ModelReader
	{
		GENERATE_UNIQUE_PTR(LASReader)

	private:

		std::ifstream file;

	public:

		LASReader(const Path& path);
		LASReader() = default;
		~LASReader() = default;

	public:

		void open() override;
		bool isOpen() const override;
		void close() override;
	};

}