#include "Background.h"

BackgroundColor::BackgroundColor(Color const &colorInit) : color(colorInit) { glClearColor(color.red, color.green, color.blue, color.alpha); }

void BackgroundColor::setColor(Color const &colorInit) {
	color = colorInit;
	glClearColor(color.red, color.green, color.blue, color.alpha);
}

Color const &BackgroundColor::getColor() const { return color; }

void BackgroundColor::draw() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }


BackgroundTexture::BackgroundTexture(::Texture texture, Size<int> const &clientSize, Size<int> const &imageSize) { create(std::move(texture), clientSize, imageSize); }

void BackgroundTexture::create(::Texture texture, Size<int> const &clientSize, Size<int> const &imageSize) {
	Rectangle::create(static_cast<Point<double>>(clientSize) / 2, {1000, 1000});
	
	Point<double> texCoords[4];
	
	texCoords[0] = {-1000, 1000};
	texCoords[1] = {-1000, -1000};
	texCoords[2] = {1000, -1000};
	texCoords[3] = {1000, 1000};
	
	setTexture(std::move(texture), texCoords);
	
	update(clientSize, DrawType::Static, imageSize);
}
