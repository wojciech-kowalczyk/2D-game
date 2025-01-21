#ifndef UTILITY_H
#define UTILITY_H

#include <string>

namespace utility {
	[[nodiscard]] constexpr std::string toString(int i);
	[[nodiscard]] constexpr int toInt(std::string const &string);
	[[nodiscard]] constexpr double toDouble(std::string const &string);
	[[nodiscard]] constexpr double exponentiation(double number, int exponent);
	template<typename T>
	constexpr void extendArray(T *&array, int oldSize, int newSize);
}


constexpr std::string utility::toString(int i) {
	if (!i)
		return "0";
	
	std::string invertedResult;
	while (i) {
		invertedResult += static_cast<char>(i % 10 + '0');
		i /= 10;
	}
	
	std::string result;
	for (int j = static_cast<int>(invertedResult.length()) - 1; j >= 0; --j)
		result += invertedResult[j];
	
	return result;
}

constexpr int utility::toInt(std::string const &string) {
	int result {0};
	for (int i = static_cast<int>(string.length()) - 1, j = 0; i >= 0; --i, ++j)
		result += (string[i] - '0') * static_cast<int>(utility::exponentiation(10, j));
	
	return result;
}

constexpr double utility::toDouble(std::string const &string) {
	// finding index of dot
	int const indexOfDot {[&string] {
		for (int i = 0; i < string.length(); ++i)
			if (string[i] == '.')
				return i;
		return -1;
	}()};
	
	// if dot haven't been found, then it means number is integer
	if (indexOfDot == -1)
		return toInt(string);
	
	// if dot have been found, but all digits on the right of it are zeros, then it means number is integer
	if ([&string, indexOfDot] {
		for (int i = indexOfDot; i < string.length(); ++i)
			if (string[i] != '0')
				return false;
		return true;
	}())
		return toInt(string);
	
	double result {0};
	for (int i = indexOfDot + 1, j = 1, end = static_cast<int>(string.length()); i < end; ++i, ++j)
		result += (string[i] - '0') / utility::exponentiation(10, j);
	for (int i = indexOfDot - 1, j = 0; i >= 0; --i, ++j)
		result += (string[i] - '0') * utility::exponentiation(10, j);
	
	return result;
}

constexpr double utility::exponentiation(double number, int exponent) {
	if (exponent == 0)
		return 1;
	
	double result {number};
	while (exponent > 1) {
		result *= number;
		--exponent;
	}
	
	return result;
}

template<typename T>
constexpr void utility::extendArray(T *&array, int const oldSize, int const newSize) {
	T *const newArray {new T[newSize]};
	for (int i = 0; i < oldSize; ++i)
		newArray[i] = std::move(array[i]);
	delete[] array;
	array = newArray;
}

#endif // UTILITY_H
