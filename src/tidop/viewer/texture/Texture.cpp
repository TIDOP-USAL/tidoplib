#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

#include <opencv2/opencv.hpp>


namespace tl
{


unsigned int Texture::count = 0;
int Texture::textureUnits = 0;


Texture::Texture(const std::string& _path, const Type& _type, bool _flip)
	: path(_path), id(0), width(0), height(0), bpp(0), slot(0), type(_type),
	flip(_flip), freeGPU(true) {
	initTextureUnits();
	//if(count < textureUnits) generateTexture();
	generateTextureFromFile(path);
}

Texture::Texture(unsigned char* buffer, const Type& _type)
	: path(""), id(0), width(0), height(0), bpp(0), slot(0), type(_type),
	flip(false), freeGPU(true) {
	initTextureUnits();
	generateTextureFromBuffer(buffer);
}

Texture::Texture(unsigned int _width, unsigned int _height, const Type& _type)
	: path(""), id(0), width(_width), height(_height), bpp(0), slot(0), type(_type),
	flip(false), freeGPU(true) {
	initTextureUnits();
	generateTexture();
}

Texture::Texture()
	: id(0), width(0), height(0), bpp(0), path(""), slot(0), type(Type::None),
	flip(false), freeGPU(true) {
	initTextureUnits();
}

Texture::Texture(const Texture& texture)
	: path(texture.path), id(texture.id), width(texture.width),
	height(texture.height), bpp(texture.bpp), slot(texture.slot), type(texture.type),
	freeGPU(false) {
}

Texture::Texture(Texture&& texture) noexcept
	: path(std::move(texture.path)), id(texture.id), width(texture.width),
	height(texture.height), bpp(texture.bpp), slot(texture.slot), type(texture.type),
	freeGPU(true) {
}

Texture::~Texture() {
	unbind();
	glActiveTexture(0);
	if (freeGPU) glDeleteTextures(1, &id);
}

Texture& Texture::operator=(const Texture& texture) {
	id = texture.id;
	width = texture.width;
	height = texture.height;
	bpp = texture.bpp;
	slot = texture.slot;
	path = texture.path;
	type = texture.type;
	freeGPU = false;
	return *this;
}

void Texture::generateTexture() {

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	slot = 0x84C0 + count;
	count++;
}

void Texture::loadTexture(unsigned char* buffer) {
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::generateTextureFromBuffer(unsigned char* buffer) {
	glGenTextures(1, &id);
	loadTexture(buffer);
	slot = 0x84C0 + count;
	count++;
}

void Texture::generateTextureFromFile(const std::string& path) {

	cv::Mat img = cv::imread(path);
	width = img.cols;
	height = img.rows;

	cvtColor(img, img, cv::COLOR_BGR2RGBA);
	if(flip) cv::flip(img, img, 0);
	
	generateTextureFromBuffer(img.data);

	/*
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
	generateTextureFromBuffer(data);
	if (data) stbi_image_free(data);
	*/
}

void Texture::bind() {
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::changeTexture(const std::string& path) {
	//stbi_set_flip_vertically_on_load(flip);
	//unsigned char* data = stbi_load(path.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
	//loadTexture(data);
	//if (data) stbi_image_free(data);
}

}
