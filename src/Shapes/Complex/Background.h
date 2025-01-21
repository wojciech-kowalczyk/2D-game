#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Shapes/Specialized.h"

class BackgroundColor : public Drawable {
public:
	BackgroundColor() = default;
	explicit BackgroundColor(Color const &color);

	void setColor(Color const &color);
	
	[[nodiscard]] Color const &getColor() const;
	
	void draw() const override;
private:
	Color color;
};


class BackgroundTexture : public shape::rectangle::texture::Base {
public:
	BackgroundTexture() = default;
	BackgroundTexture(::Texture texture, Size<int> const &clientSize, Size<int> const &imageSize);
	void create(::Texture texture, Size<int> const &clientSize, Size<int> const &imageSize);
};

#endif // BACKGROUND_H
