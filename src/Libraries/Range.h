#ifndef RANGE_H
#define RANGE_H

template<class Iterator>
class Range {
public:
	constexpr Range(Iterator begin, Iterator end);
	
	[[nodiscard]] constexpr Iterator begin() const;
	[[nodiscard]] constexpr Iterator end() const;
private:
	Iterator const _begin, _end;
};


template<class Iterator>
constexpr Range<Iterator>::Range(Iterator begin, Iterator end) : _begin(begin), _end(end) {}

template<class Iterator>
constexpr Iterator Range<Iterator>::begin() const { return _begin; }

template<class Iterator>
constexpr Iterator Range<Iterator>::end() const { return _end; }

#endif // RANGE_H
