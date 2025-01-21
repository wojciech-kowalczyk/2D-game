#ifndef COLOR_H
#define COLOR_H

class Color {
public:
	constexpr Color();
	constexpr Color(float red, float green, float blue, float alpha = 1);
	
	float red, green, blue, alpha;
};

constexpr Color::Color() : red(0), green(0), blue(0), alpha(1) {}

constexpr Color::Color(float const red, float const green, float const blue, float const alpha) : red(red), green(green), blue(blue), alpha(alpha) {}

#endif // COLOR_H
