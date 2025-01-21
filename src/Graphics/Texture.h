#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "Math/Point.h"

class Texture {
public:
	/*
	enum FilterMinMode {
		NearestMipmapNearest, // takes the nearest mipmap level to match the pixel size and uses nearest interpolation for texture sampling
		LinearMipmapNearest, // takes the nearest mipmap level to match the pixel size and uses linear interpolation for texture sampling
		NearestMipmapLinear, // linearly interpolates between the two closest matching mipmaps and uses nearest interpolation for texture sampling
		LinearMipmapLinear // linearly interpolates between the two closest mipmaps and uses linear interpolation for texture sampling
	};
	
	enum FilterMagMode {
		Nearest, // default, nearest pixel's color is chosen
		Linear // color is calculated depending on colors of neighbouring pixels and their distance
	};
	*/
	/*
	/// \param axis - Combination of Axis enums
	void setRepeatMode(int axis) const;
	/// Set every axis in repeat mode, set by default by opengl - don't use when no mode was set
	void setRepeatMode() const;
	
	/// \param axis - Combination of Axis enums
	void setBorderMode(int axis, Color const &color) const;
	/// Set every axis in border mode
	void setBorderMode(Color const &color) const;
	
	void setFilterMinMode(FilterMinMode mode) const;
	void setFilterMagMode(FilterMagMode mode) const;
	*/
	
	Texture();
	/// \warning Can generate error
	explicit Texture(std::string filePath);
	/// \warning Can generate error
	void create(std::string filePath);
	
	[[nodiscard]] Size<int> const &getImageSize() const;
	[[nodiscard]] std::string const &getPath() const;
	
	void activate() const;
private:
	unsigned id;
	Size<int> imageSize;
	std::string path;
};

#endif // TEXTURE_H
