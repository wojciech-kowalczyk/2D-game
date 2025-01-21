#ifndef POINT_H
#define POINT_H

template<typename T>
class Point;

template<typename T> using Size = Point<T>;

template<typename T>
class Point {
public:
	constexpr Point();
	constexpr Point(T x, T y);
	template<typename S>
	constexpr explicit Point(Point<S> const &);
	
	constexpr Point &operator+=(Point const &point);
	constexpr Point &operator-=(Point const &point);
	constexpr Point &operator*=(Point const &point);
	constexpr Point &operator/=(Point const &point);
	constexpr Point &operator*=(T scalar);
	constexpr Point &operator/=(T scalar);
	[[nodiscard]] constexpr Point operator+(Point const &point) const;
	[[nodiscard]] constexpr Point operator-(Point const &point) const;
	[[nodiscard]] constexpr Point operator*(Point const &point) const;
	[[nodiscard]] constexpr Point operator/(Point const &point) const;
	[[nodiscard]] constexpr Point operator*(T scalar) const;
	[[nodiscard]] constexpr Point operator/(T scalar) const;
	template<typename S>
	[[nodiscard]] constexpr bool operator==(Point<S> const &point) const;
	template<typename S>
	[[nodiscard]] constexpr bool operator!=(Point<S> const &point) const;
	
	/// Converts point from coordinate system where (-1, -1) is down-left and (1, 1) is up-rigth corner of client area to
	/// coordinate system where (0, 0) is down-left and (clientSize.x, clientSize.y) is up-rigth corner of client area.
	[[nodiscard]] constexpr Point<T> toNormalCoords(Size<int> const &clientSize) const;
	
	/// Converts point from coordinate system where (0, 0) is down-left and (clientSize.x, clientSize.y) is up-rigth corner of client area to
	/// coordinate system where (-1, -1) is down-left and (1, 1) is up-rigth corner of client area.
	[[nodiscard]] constexpr Point<float> toOpenGlCoords(Size<int> const &clientSize) const;
	
	/// Converts point from coordinate system where (0, 0) is down-left and (1, 1) is up-rigth corner of image area to
	/// coordinate system where (0, 0) is down-left and (imageSize.x, imageSize.y) is up-rigth corner of image area.
	[[nodiscard]] constexpr Point<T> toNormalTextureCoords(Size<int> const &imageSize) const;
	
	/// Converts point from coordinate system where (0, 0) is down-left and (imageSize.x, imageSize.y) is up-rigth corner of image area to
	/// coordinate system where (0, 0) is down-left and (1, 1) is up-rigth corner of image area.
	[[nodiscard]] constexpr Point<float> toOpenGlTextureCoords(Size<int> const &imageSize) const;
	
	T x, y;
};


template<typename T>
constexpr Point<T>::Point() : x(0), y(0) {}

template<typename T>
constexpr Point<T>::Point(T const x, T const y) : x(x), y(y) {}

template<typename T>
template<typename S>
constexpr Point<T>::Point(Point<S> const &copy) : x(copy.x), y(copy.y) {}

template<typename T>
constexpr Point<T> &Point<T>::operator+=(Point const &point) {
	x += point.x;
	y += point.y;
	return *this;
}

template<typename T>
constexpr Point<T> &Point<T>::operator-=(Point const &point) {
	x -= point.x;
	y -= point.y;
	return *this;
}

template<typename T>
constexpr Point<T> &Point<T>::operator*=(Point const &point) {
	x *= point.x;
	y *= point.y;
	return *this;
}

template<typename T>
constexpr Point<T> &Point<T>::operator/=(Point const &point) {
	x /= point.x;
	y /= point.y;
	return *this;
}

template<typename T>
constexpr Point<T> &Point<T>::operator*=(T const scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

template<typename T>
constexpr Point<T> &Point<T>::operator/=(T const scalar) { return *this *= (1 / scalar); }

template<typename T>
constexpr Point<T> Point<T>::operator+(Point const &point) const { return Point {*this} += point; }

template<typename T>
constexpr Point<T> Point<T>::operator-(Point const &point) const { return Point {*this} -= point; }

template<typename T>
constexpr Point<T> Point<T>::operator*(Point const &point) const { return Point {*this} *= point; }

template<typename T>
constexpr Point<T> Point<T>::operator/(Point const &point) const { return Point {*this} /= point; }

template<typename T>
constexpr Point<T> Point<T>::operator*(T const scalar) const { return Point {*this} *= scalar; }

template<typename T>
constexpr Point<T> Point<T>::operator/(T const scalar) const { return Point {*this} /= scalar; }

template<typename T>
template<typename S>
constexpr bool Point<T>::operator==(Point<S> const &point) const { return x == point.x && y == point.y; }

template<typename T>
template<typename S>
constexpr bool Point<T>::operator!=(Point<S> const &point) const { return !(*this == point); }

template<typename T>
constexpr Point<T> Point<T>::toNormalCoords(Size<int> const &clientSize) const {
	return (*this + Point<T> {1, 1}) / 2 * static_cast<Point<T>>(clientSize);
}

template<typename T>
constexpr Point<float> Point<T>::toOpenGlCoords(Size<int> const &clientSize) const {
	return static_cast<Point<float>>(*this) * 2.f / static_cast<Point<float>>(clientSize) - Point<float> {1.f, 1.f};
}

template<typename T>
constexpr Point<T> Point<T>::toNormalTextureCoords(Size<int> const &imageSize) const { return *this * static_cast<Point<T>>(imageSize); }

template<typename T>
constexpr Point<float> Point<T>::toOpenGlTextureCoords(Size<int> const &imageSize) const {
	return static_cast<Point<float>>(*this) / static_cast<Point<float>>(imageSize);
}

#endif // POINT_H
