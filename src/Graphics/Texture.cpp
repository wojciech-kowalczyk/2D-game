#include "Texture.h"
#include "renderFunctions.h"

#define STB_IMAGE_IMPLEMENTATION

#include "imageLoader.h"

Texture::Texture() : id(0) {}

Texture::Texture(std::string filePath) : Texture() { create(std::move(filePath)); }

void Texture::create(std::string filePath) {
	path = std::move(filePath);
	
	glGenTextures(1, &id);
	activate();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_set_flip_vertically_on_load(true);
	
	int channelCount;
	unsigned char const *const data {stbi_load(path.c_str(), &imageSize.x, &imageSize.y, &channelCount, 0)};
	
	if (!data) {
		extern bool error;
		extern std::string errorDescription;
		error = true;
		errorDescription = "Image \"" + path + "\" doesn't exist or is not supported.";
		return;
	}
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	if (channelCount == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSize.x, imageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageSize.x, imageSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	delete[] data;
	
	renderFunctions::texture::glGenerateMipmap(GL_TEXTURE_2D);
}

Size<int> const &Texture::getImageSize() const { return imageSize; }

std::string const &Texture::getPath() const { return path; }

void Texture::activate() const {
	renderFunctions::texture::glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
}

/*
void Texture::setRepeatMode(int const axis) const {
	activate();

	if (axis & static_cast<int>(Axis::X))
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	if (axis & static_cast<int>(Axis::Y))
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (axis & static_cast<int>(Axis::Z))
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

void Texture::setRepeatMode() const {
	activate();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

void Texture::setBorderMode(int const axis, Color const &color) const {
	activate();

	if (axis & static_cast<int>(Axis::X))
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

	if (axis & static_cast<int>(Axis::Y))
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	if (axis & static_cast<int>(Axis::Z))
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color.toArray());
}

void Texture::setBorderMode(Color const &color) const {
	activate();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color.toArray());
}

void Texture::setFilterMinMode(FilterMinMode const mode) const {
	activate();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, [mode]{
		switch (mode) {
			case NearestMipmapNearest:
				return GL_NEAREST_MIPMAP_NEAREST;
			case NearestMipmapLinear:
				return GL_NEAREST_MIPMAP_LINEAR;
			case LinearMipmapNearest:
				return GL_LINEAR_MIPMAP_NEAREST;
			case LinearMipmapLinear:
				return GL_LINEAR_MIPMAP_LINEAR;
		}
	}());
}

void Texture::setFilterMagMode(FilterMagMode const mode) const {
	activate();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, [mode]{
		switch (mode) {
			case Nearest:
				return GL_NEAREST;
			case Linear:
				return GL_LINEAR;
		}
	}());
}
*/
