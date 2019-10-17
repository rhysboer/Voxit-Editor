#pragma once
#include <list>

template <typename T>
class CircularBuffer {
public:
	CircularBuffer(const unsigned int size);
	~CircularBuffer();

	T GetNewest() const;
	bool Push(T& value);
	void Pop_Newest(); // Pop the newest entry
	void Pop_Oldest(); // Pop the oldest entry
	void Clear();

	bool IsFull() const;
	unsigned int BufferSize() const; // Max size of CircularBuffer
	unsigned int Count() const; // Size of content in CircularBuffer

private:
	std::list<T> list;

	unsigned int bufferSize;
};

template<typename T>
inline CircularBuffer<T>::CircularBuffer(const unsigned int size) {
	this->list = std::list<T>();
	this->bufferSize = size;
}

template<typename T>
inline CircularBuffer<T>::~CircularBuffer() {
	Clear();
}

template<typename T>
inline bool CircularBuffer<T>::Push(T& value) {
	if(bufferSize <= 0) return false;

	if(list.size() + 1 > bufferSize) {
		if(list.begin() != list.end()) {
			Pop_Oldest();
		}
	}

	list.push_back(value);
	return true;
}

template<typename T>
inline bool CircularBuffer<T>::IsFull() const {
	return (list.size() >= bufferSize) ? true : false;
}

template<typename T>
inline void CircularBuffer<T>::Pop_Newest() {
	if(list.size() > 0) {
		list.pop_back();
	}
}

template<typename T>
inline T CircularBuffer<T>::GetNewest() const {
	if(list.size() <= 0) return nullptr;
	return list.back();
}

template<typename T>
inline void CircularBuffer<T>::Clear() {
	list.clear();
}

template<typename T>
inline unsigned int CircularBuffer<T>::BufferSize() const {
	return bufferSize;
}

template<typename T>
inline unsigned int CircularBuffer<T>::Count() const {
	return list.size();
}

template<typename T>
inline void CircularBuffer<T>::Pop_Oldest() {
	if(list.size() > 0) {
		list.pop_front();
	}
}
