#ifndef LIST_H
#define LIST_H

#include <utility>

template<class Data>
class List {
	struct Node;
public:
	constexpr List();
	constexpr List(List const &);
	constexpr List(List &&) noexcept;
	template<typename ConvertableList>
	constexpr explicit List(ConvertableList const &);
	constexpr ~List();
	
	[[nodiscard]] constexpr List &operator=(List const &);
	[[nodiscard]] constexpr List &operator=(List &&) noexcept;
	template<typename ConvertableList>
	[[nodiscard]] constexpr List &operator=(ConvertableList const &);
	[[nodiscard]] constexpr Data &operator[](int index);
	[[nodiscard]] constexpr Data const &operator[](int index) const;
	
	constexpr void add(Data data, int index);
	constexpr void addFirst(Data data);
	constexpr void addLast(Data data);
	constexpr Data remove(int index);
	constexpr Data removeFirst();
	constexpr Data removeLast();
	constexpr void removeAll();
	
	[[nodiscard]] constexpr Data &getFirst();
	[[nodiscard]] constexpr Data &getLast();
	[[nodiscard]] constexpr Data const &getFirst() const;
	[[nodiscard]] constexpr Data const &getLast() const;
	[[nodiscard]] constexpr int getCount() const;
	[[nodiscard]] constexpr bool empty() const;
	[[nodiscard]] constexpr Node *find(int index);
	[[nodiscard]] constexpr Node const *find(int index) const;
	
	class Iterator {
	public:
		constexpr explicit Iterator(Node *node);
		
		[[nodiscard]] constexpr Data &operator*();
		[[nodiscard]] constexpr Data *operator->();
		constexpr Iterator &operator++();
		[[nodiscard]] constexpr Iterator const operator++(int);
		constexpr Iterator &operator--();
		[[nodiscard]] constexpr Iterator const operator--(int);
		template<class ConvertableIterator>
		[[nodiscard]] constexpr bool operator==(ConvertableIterator const &convertableIterator);
		template<class ConvertableIterator>
		[[nodiscard]] constexpr bool operator!=(ConvertableIterator const &convertableIterator);
	private:
		Node *node;
	};
	
	[[nodiscard]] constexpr Iterator begin() const;
	[[nodiscard]] constexpr Iterator end() const;
private:
	struct Node {
		Data data;
		Node *previous, *next;
	};
	
	int count;
	Node *first, *last;
};


template<class Data>
constexpr List<Data>::List() : count(0), first(nullptr), last(nullptr) {}

template<class Data>
constexpr List<Data>::List(List const &copy) : List() {
	for (Data const &data : copy)
		addFirst(data);
}

template<class Data>
constexpr List<Data>::List(List &&move) noexcept: count(move.count), first(move.first), last(move.last) {
	move.count = 0;
	move.first = move.last = nullptr;
}

template<class Data>
template<typename ConvertableList>
constexpr List<Data>::List(ConvertableList const &copy) : List() {
	for (Data const &data : copy)
		addFirst(data);
}

template<class Data>
constexpr List<Data>::~List() { removeAll(); }

template<class Data>
constexpr List<Data> &List<Data>::operator=(List const &copy) {
	if (this != &copy) {
		removeAll();
		for (Data const &data : copy)
			addFirst(data);
	}
	return *this;
}

template<class Data>
constexpr List<Data> &List<Data>::operator=(List &&move) noexcept {
	if (this != &move) {
		removeAll();
		count = move.count;
		first = move.first;
		last = move.last;
		move.count = 0;
		move.first = move.last = nullptr;
	}
	return *this;
}

template<class Data>
template<typename ConvertableList>
constexpr List<Data> &List<Data>::operator=(ConvertableList const &copy) {
	if (this != &copy) {
		removeAll();
		for (Data const &data : copy)
			addFirst(data);
	}
	return *this;
}

template<class Data>
constexpr Data &List<Data>::operator[](int const index) { return find(index)->data; }

template<class Data>
constexpr Data const &List<Data>::operator[](int const index) const { return find(index)->data; }

template<class Data>
constexpr void List<Data>::add(Data data, int const index) {
	if (index == 0)
		addFirst(std::move(data));
	else if (index == count)
		addLast(std::move(data));
	else {
		Node *const prev = find(index - 1);
		prev->next = new Node {std::move(data), prev, prev->next};
		prev->next->next->previous = prev->next;
		++count;
	}
}

template<class Data>
constexpr void List<Data>::addFirst(Data data) {
	first = new Node {std::move(data), nullptr, first};
	if (empty())
		last = first;
	++count;
}

template<class Data>
constexpr void List<Data>::addLast(Data data) {
	Node *const newLast = new Node {std::move(data), last, nullptr};
	if (empty())
		first = newLast;
	else
		last->next = newLast;
	last = newLast;
	++count;
}

template<class Data>
constexpr Data List<Data>::remove(int const index) {
	if (index == 0)
		return std::move(removeFirst());
	if (index == count - 1)
		return std::move(removeLast());
	Node *const prev = find(index - 1);
	Node *const next = prev->next->next;
	Data returnData {std::move(prev->next->data)};
	delete prev->next;
	prev->next = next;
	next->previous = prev;
	--count;
	return std::move(returnData);
}

template<class Data>
constexpr Data List<Data>::removeFirst() {
	Node *const oldFirst = first;
	first = first->next;
	if (count == 1)
		last = nullptr;
	else
		first->previous = nullptr;
	Data returnData {std::move(oldFirst->data)};
	delete oldFirst;
	--count;
	return std::move(returnData);
}

template<class Data>
constexpr Data List<Data>::removeLast() {
	Node *const oldLast = last;
	if (count == 1)
		first = last = nullptr;
	else {
		last = last->previous;
		last->next = nullptr;
	}
	Data returnData {std::move(oldLast->data)};
	delete oldLast;
	--count;
	return std::move(returnData);
}

template<class Data>
constexpr void List<Data>::removeAll() { while (!empty()) removeFirst(); }

template<class Data>
constexpr Data &List<Data>::getFirst() { return first->data; }

template<class Data>
constexpr Data &List<Data>::getLast() { return last->data; }

template<class Data>
constexpr Data const &List<Data>::getFirst() const { return first->data; }

template<class Data>
constexpr Data const &List<Data>::getLast() const { return last->data; }

template<class Data>
constexpr int List<Data>::getCount() const { return count; }

template<class Data>
constexpr bool List<Data>::empty() const { return last == nullptr; }

template<class Data>
constexpr List<Data>::Node *List<Data>::find(int const index) {
	List<Data>::Node *temp;
	
	if (index < count / 2) {
		temp = first;
		for (int i = 0; i < index; ++i)
			temp = temp->next;
	} else {
		temp = last;
		for (int i = count - 1; i > index; --i)
			temp = temp->previous;
	}
	
	return temp;
}

template<class Data>
constexpr List<Data>::Node const *List<Data>::find(int const index) const {
	List<Data>::Node const *temp;
	
	if (index < count / 2) {
		temp = first;
		for (int i = 0; i < index; ++i)
			temp = temp->next;
	} else {
		temp = last;
		for (int i = count - 1; i > index; --i)
			temp = temp->previous;
	}
	
	return temp;
}

// Range

template<class Data>
constexpr List<Data>::Iterator List<Data>::begin() const { return Iterator {first}; }

template<class Data>
constexpr List<Data>::Iterator List<Data>::end() const { return Iterator {nullptr}; }

// Iterator

template<class Data>
constexpr List<Data>::Iterator::Iterator(Node *const node) : node(node) {}

template<class Data>
constexpr Data &List<Data>::Iterator::operator*() { return node->data; }

template<class Data>
constexpr Data *List<Data>::Iterator::operator->() { return &node->data; }

template<class Data>
constexpr List<Data>::Iterator &List<Data>::Iterator::operator++() {
	node = node->next;
	return *this;
}

template<class Data>
constexpr List<Data>::Iterator const List<Data>::Iterator::operator++(int) {
	Iterator iterator {*this};
	operator++();
	return iterator;
}

template<class Data>
constexpr List<Data>::Iterator &List<Data>::Iterator::operator--() {
	node = node->previous;
	return *this;
}

template<class Data>
constexpr List<Data>::Iterator const List<Data>::Iterator::operator--(int) {
	Iterator const iterator {*this};
	operator++();
	return iterator;
}

template<class Data>
template<class ConvertableIterator>
constexpr bool List<Data>::Iterator::operator==(ConvertableIterator const &convertableIterator) { return node == convertableIterator.node; }

template<class Data>
template<class ConvertableIterator>
constexpr bool List<Data>::Iterator::operator!=(ConvertableIterator const &convertableIterator) { return !(*this == convertableIterator); }


// ==================================  Specialization for references  ==================================


template<class Data>
class List<Data &> {
	struct Node;
public:
	constexpr List();
	constexpr List(List const &);
	constexpr List(List &&) noexcept;
	template<typename ConvertableList>
	constexpr explicit List(ConvertableList const &);
	constexpr ~List();
	
	[[nodiscard]] constexpr List &operator=(List const &);
	[[nodiscard]] constexpr List &operator=(List &&) noexcept;
	template<typename ConvertableList>
	[[nodiscard]] constexpr List &operator=(ConvertableList const &);
	[[nodiscard]] constexpr Data &operator[](int index);
	[[nodiscard]] constexpr Data const &operator[](int index) const;
	
	constexpr void add(Data &data, int index);
	constexpr void addFirst(Data &data);
	constexpr void addLast(Data &data);
	constexpr Data &remove(int index);
	constexpr Data &removeFirst();
	constexpr Data &removeLast();
	constexpr void removeAll();
	
	[[nodiscard]] constexpr Data &getFirst();
	[[nodiscard]] constexpr Data &getLast();
	[[nodiscard]] constexpr Data const &getFirst() const;
	[[nodiscard]] constexpr Data const &getLast() const;
	[[nodiscard]] constexpr int getCount() const;
	[[nodiscard]] constexpr bool empty() const;
	[[nodiscard]] constexpr Node *find(int index);
	[[nodiscard]] constexpr Node const *find(int index) const;
	
	class Iterator {
	public:
		constexpr explicit Iterator(Node *node);
		
		[[nodiscard]] constexpr Data &operator*();
		[[nodiscard]] constexpr Data *operator->();
		constexpr Iterator &operator++();
		[[nodiscard]] constexpr Iterator const operator++(int);
		constexpr Iterator &operator--();
		[[nodiscard]] constexpr Iterator const operator--(int);
	private:
		Node *node;
	};
	
	[[nodiscard]] constexpr Iterator begin() const;
	[[nodiscard]] constexpr Iterator end() const;
private:
	struct Node {
		Data &data;
		Node *previous, *next;
	};
	
	int count;
	Node *first, *last;
};


template<class Data>
constexpr List<Data &>::List() : count(0), first(nullptr), last(nullptr) {}

template<class Data>
constexpr List<Data &>::List(List const &copy) : List() {
	for (Data const &data : copy)
		addFirst(data);
}

template<class Data>
constexpr List<Data &>::List(List &&move) noexcept: count(move.count), first(move.first), last(move.last) {
	move.count = 0;
	move.first = move.last = nullptr;
}

template<class Data>
template<typename ConvertableList>
constexpr List<Data &>::List(ConvertableList const &copy) : List() {
	for (Data const &data : copy)
		addFirst(data);
}

template<class Data>
constexpr List<Data &>::~List() { removeAll(); }

template<class Data>
constexpr List<Data &> &List<Data &>::operator=(List const &copy) {
	if (this != &copy) {
		removeAll();
		for (Data const &data : copy)
			addFirst(data);
	}
	return *this;
}

template<class Data>
constexpr List<Data &> &List<Data &>::operator=(List &&move) noexcept {
	if (this != &move) {
		removeAll();
		count = move.count;
		first = move.first;
		last = move.last;
		move.count = 0;
		move.first = move.last = nullptr;
	}
	return *this;
}

template<class Data>
template<typename ConvertableList>
constexpr List<Data &> &List<Data &>::operator=(ConvertableList const &copy) {
	if (this != &copy) {
		removeAll();
		for (Data const &data : copy)
			addFirst(data);
	}
	return *this;
}

template<class Data>
constexpr Data &List<Data &>::operator[](int const index) { return find(index)->data; }

template<class Data>
constexpr Data const &List<Data &>::operator[](int const index) const { return find(index)->data; }

template<class Data>
constexpr void List<Data &>::add(Data &data, int const index) {
	if (index == 0)
		addFirst(data);
	else if (index == count)
		addLast(data);
	else {
		Node *const prev = find(index - 1);
		prev->next = new Node {data, prev, prev->next};
		prev->next->next->previous = prev->next;
		++count;
	}
}

template<class Data>
constexpr void List<Data &>::addFirst(Data &data) {
	first = new Node {data, nullptr, first};
	if (empty())
		last = first;
	++count;
}

template<class Data>
constexpr void List<Data &>::addLast(Data &data) {
	Node *const newLast = new Node {data, last, nullptr};
	if (empty())
		first = newLast;
	else
		last->next = newLast;
	last = newLast;
	++count;
}

template<class Data>
constexpr Data &List<Data &>::remove(int const index) {
	if (index == 0)
		return removeFirst();
	if (index == count - 1)
		return removeLast();
	Node *const prev = find(index - 1);
	Node *const next = prev->next->next;
	Data returnData {prev->next->data};
	delete prev->next;
	prev->next = next;
	next->previous = prev;
	--count;
	return returnData;
}

template<class Data>
constexpr Data &List<Data &>::removeFirst() {
	Node *const oldFirst = first;
	first = first->next;
	if (count == 1)
		last = nullptr;
	else
		first->previous = nullptr;
	Data returnData {oldFirst->data};
	delete oldFirst;
	--count;
	return returnData;
}

template<class Data>
constexpr Data &List<Data &>::removeLast() {
	Node *const oldLast = last;
	if (count == 1)
		first = last = nullptr;
	else {
		last = last->previous;
		last->next = nullptr;
	}
	Data returnData {oldLast->data};
	delete oldLast;
	--count;
	return returnData;
}

template<class Data>
constexpr void List<Data &>::removeAll() { while (!empty()) removeFirst(); }

template<class Data>
constexpr Data &List<Data &>::getFirst() { return first->data; }

template<class Data>
constexpr Data &List<Data &>::getLast() { return last->data; }

template<class Data>
constexpr Data const &List<Data &>::getFirst() const { return first->data; }

template<class Data>
constexpr Data const &List<Data &>::getLast() const { return last->data; }

template<class Data>
constexpr int List<Data &>::getCount() const { return count; }

template<class Data>
constexpr bool List<Data &>::empty() const { return last == nullptr; }

template<class Data>
constexpr List<Data &>::Node *List<Data &>::find(int const index) {
	List<Data &>::Node *temp;
	
	if (index < count / 2) {
		temp = first;
		for (int i = 0; i < index; ++i)
			temp = temp->next;
	} else {
		temp = last;
		for (int i = count - 1; i > index; --i)
			temp = temp->previous;
	}
	
	return temp;
}

template<class Data>
constexpr List<Data &>::Node const *List<Data &>::find(int const index) const {
	List<Data &>::Node const *temp;
	
	if (index < count / 2) {
		temp = first;
		for (int i = 0; i < index; ++i)
			temp = temp->next;
	} else {
		temp = last;
		for (int i = count - 1; i > index; --i)
			temp = temp->previous;
	}
	
	return temp;
}

// Range

template<class Data>
constexpr List<Data &>::Iterator List<Data &>::begin() const { return Iterator {first}; }

template<class Data>
constexpr List<Data &>::Iterator List<Data &>::end() const { return Iterator {nullptr}; }

// Iterator

template<class Data>
constexpr List<Data &>::Iterator::Iterator(Node *const node) : node(node) {}

template<class Data>
constexpr Data &List<Data &>::Iterator::operator*() { return node->data; }

template<class Data>
constexpr Data *List<Data &>::Iterator::operator->() { return &node->data; }

template<class Data>
constexpr List<Data &>::Iterator &List<Data &>::Iterator::operator++() {
	node = node->next;
	return *this;
}

template<class Data>
constexpr List<Data &>::Iterator const List<Data &>::Iterator::operator++(int) {
	Iterator iterator {*this};
	operator++();
	return iterator;
}

template<class Data>
constexpr List<Data &>::Iterator &List<Data &>::Iterator::operator--() {
	node = node->previous;
	return *this;
}

template<class Data>
constexpr List<Data &>::Iterator const List<Data &>::Iterator::operator--(int) {
	Iterator const iterator {*this};
	operator++();
	return iterator;
}

#endif // LIST_H
