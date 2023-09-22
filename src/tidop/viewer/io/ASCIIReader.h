#pragma once

#include <iostream>
#include <fstream>

#include "ModelReader.h"

namespace tl
{

class ASCIIReader : public ModelReader
{
	GENERATE_UNIQUE_PTR(ASCIIReader)

private:

	std::ifstream file;

public:

	ASCIIReader(const Path& path);
	ASCIIReader() = default;
	~ASCIIReader() = default;

public:

	void open() override;
	bool isOpen() const override;
	void close() override;
};

}