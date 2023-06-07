#pragma once
#include <stdlib.h>

template <typename T, size_t S>
class StackVec {
public:
	size_t size() {
		return m_size;
	}
	void push_back(T element) {
		if (m_size >= capacity) {
			throw std::exception();
		}
		arr[m_size++] = element;
	}
	void pop_back() {
		m_size--;
	}
	T operator[](size_t index) {
		if (index >= m_size || index < 0) {
			throw std::exception();
		}
		return arr[index];
	}
    void clear(){
        m_size = 0;
    }


	const size_t capacity = S;
private:
	size_t m_size = 0;
    T arr[S];
};