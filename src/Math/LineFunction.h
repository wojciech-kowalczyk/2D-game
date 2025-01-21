#ifndef LINEFUNCTION_H
#define LINEFUNCTION_H

class LineFunction {
public:
	enum Type { Vertical, Horizontal, Oblique };
	
	constexpr LineFunction();
	constexpr LineFunction(double a, double b, Type type);
	
	[[nodiscard]] constexpr double operator()(double x) const;
	
	[[nodiscard]] constexpr double getY(double x) const;
	[[nodiscard]] constexpr double getX(double y) const;
	
	double a, b;
	Type type;
};


constexpr LineFunction::LineFunction() : a(0), b(0), type(Vertical) {}

constexpr LineFunction::LineFunction(double const a, double const b, Type const type) : a(a), b(b), type(type) {}

constexpr double LineFunction::operator()(double const x) const { return getY(x); }

constexpr double LineFunction::getY(double const x) const { return x * a + b; }

constexpr double LineFunction::getX(double const y) const { return (y - b) / a; }

#endif // LINEFUNCTION_H
