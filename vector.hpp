#ifndef SSTD_VECTOR_INCLUDED
#define SSTD_VECTOR_INCLUDED

#include "core.hpp"

#include <initializer_list>
#include <utility>
#include <stdlib.h>
#include <malloc.h>
#include <algorithm>
#include <xmemory>
#include <new>

SSTD_BEGIN

template<typename T>
class vector {
public:
	class iterator;
	class reverse_iterator;
	class const_iterator;
	class const_reverse_iterator;

public:

	// Default Constructor
	vector() SSTD_DEFAULT;

	// Constructor that initialize 'length' amount of objects 
	SSTD_EXPLICIT vector(sizet length) :
		m_size(length), m_capacity(length), m_data((T*)malloc(sizeof(T)* length)) {
		_Fill_Range(0, length);
	}

	// Constructor that set all the object to val
	vector(sizet length, const T& val) :
		m_size(length), m_capacity(length), m_data((T*)malloc(sizeof(T)* length)) {
		_Fill_Range(0, length, val);
	}

	// Constructor that initialize using a initializer list
	vector(std::initializer_list<T> list) :
		m_size(list.size()), m_capacity(list.size()), m_data((T*)malloc(sizeof(T)* list.size())) {
		auto ptr = list.begin();
		for (sizet i = 0; i < list.size(); ++i, ++ptr) {
			m_data[i] = *ptr;
		}
	}

	// Destructor
	~vector() {
		if (m_data) {
			for (int i = 0; i < m_size; ++i) {
				m_data[i].~T();
			}
			free(m_data);
			m_data = nullptr;
		}
	}

	SSTD_INLINE void clear() {
		if (m_size) {
			for (int i = 0; i < m_size; ++i) {
				m_data[i].~T();
			}
		}
		if (m_data != nullptr) {
			free(m_data);
			m_data = nullptr;
		}
		m_size = 0;
		m_capacity = 0;
	}

	// Construct the object to the back of the vector.
	// By constructing it direclty into the memory, 
	// we don't need to call any move / copy constructor.
	template<typename ... _Val>
	void emplace_back(_Val&& ...val) {
		if (m_data == nullptr) {
			_Malloc_Data(8);
		}
		else if (m_size >= m_capacity) {
			_Realloc_Data(m_capacity * 2);
		}
		new (&m_data[m_size++]) T(std::forward<_Val>(val)...);
	}

	// Push val to the back of the vector
	void push_back(const T& val) {
		emplace_back(val);
	}
	// Push val to the back of the vector
	void push_back(T&& val) {
		emplace_back(std::forward<T>(val));
	}

	// Reserve a certain amount of memory ( without initialization )
	void reserve(sizet new_cap) {
		m_capacity += new_cap;
		if (m_data == nullptr) {
			_Malloc_Data(m_capacity);
			return;
		}
		_Realloc_Data(m_capacity);
	}

	// Resize the vector to new_size ( with initialization )
	void resize(sizet new_size) {
		if (m_data == nullptr) {
			_Malloc_Data(new_size);
		}
		else if (new_size > m_capacity) {
			_Realloc_Data(new_size);
		}
		_Fill_Range(m_size, new_size);
		m_size = new_size;
	}

	template<typename _Iter>
	void insert(sizet pos, _Iter iter_beg, _Iter iter_end) {
		_Insert_At(pos, iter_beg, iter_end);
	}

	template<typename _Iter>
	void insert(iterator pos, _Iter iter_beg, _Iter iter_end) {
		_Insert_At(pos.m_ind, iter_beg, iter_end);
	}

	template<typename _Iter>
	void insert(reverse_iterator pos, _Iter iter_beg, _Iter iter_end) {
		_Insert_At(pos.m_ind, iter_beg, iter_end);
	}

	SSTD_INLINE SSTD_CONSTEXPR sizet size() const {
		return m_size;
	}

	SSTD_INLINE SSTD_CONSTEXPR sizet capacity() const {
		return m_capacity;
	}

	SSTD_INLINE SSTD_CONSTEXPR T& front() const noexcept{
		SSTD_ASSERT(m_size >= 0);

		return m_data[0];
	}
	SSTD_INLINE SSTD_CONSTEXPR T& back() const noexcept {
		SSTD_ASSERT(m_size >= 0);

		return m_data[m_size-1];
	}
	SSTD_INLINE SSTD_CONSTEXPR T* data() const noexcept {
		return m_data;
	}
	SSTD_INLINE T& operator[](int key) {
		SSTD_ASSERT(key >= 0 && key < this->m_size);

		return this->m_data[key];
	}

	SSTD_INLINE const T& operator[](int key) const {
		SSTD_ASSERT(key >= 0 && key < this->m_size);

		return this->m_data[key];
	}

	SSTD_INLINE SSTD_CONSTEXPR iterator begin() {
		return iterator(this, 0);
	}
	SSTD_INLINE SSTD_CONSTEXPR iterator end() {
		return iterator(this, this->m_size);
	}

	SSTD_INLINE SSTD_CONSTEXPR const_iterator begin() const {
		return const_iterator(this, 0);
	}
	SSTD_INLINE SSTD_CONSTEXPR const_iterator end() const {
		return const_iterator(this, this->m_size);
	}

	SSTD_INLINE SSTD_CONSTEXPR reverse_iterator rbegin() {
		return reverse_iterator(this, this->m_size-1);
	}
	SSTD_INLINE SSTD_CONSTEXPR reverse_iterator rend() {
		return reverse_iterator(this, -1);
	}

	SSTD_INLINE SSTD_CONSTEXPR const_reverse_iterator rbegin() const {
		return const_reverse_iterator(this, this->m_size - 1);
	}
	SSTD_INLINE SSTD_CONSTEXPR const_reverse_iterator rend() const {
		return const_reverse_iterator(this, -1);
	}

	SSTD_INLINE SSTD_CONSTEXPR const_iterator cbegin() const {
		return const_iterator(this, 0);
	}
	SSTD_INLINE SSTD_CONSTEXPR const_iterator cend() const {
		return const_iterator(this, this->m_size);
	}

	SSTD_INLINE SSTD_CONSTEXPR const_reverse_iterator crbegin() const {
		return const_reverse_iterator(this, this->m_size - 1);
	}
	SSTD_INLINE SSTD_CONSTEXPR const_reverse_iterator crend() const {
		return const_reverse_iterator(this, -1);
	}
private:
	T* m_data = nullptr;
	sizet m_size = 0;
	sizet m_capacity = 0;

	SSTD_INLINE void _Malloc_Data(sizet memsize) {
		m_data = (T*)malloc(sizeof(T) * memsize);
		m_capacity = memsize;
	}

	// Reallocate memory can help improve performance
	// because instead of allocating new chunks of memory everytime,
	// it extends the current allocated memory, 
	// ( Allocate another chunk of memory if extension is not possible.
	SSTD_INLINE void _Realloc_Data(sizet memsize) {
		T* tmp = (T*)realloc(m_data, sizeof(T) * memsize);

		// Handle situations if there aren't enough memory to extend
		if (tmp == nullptr) {
			tmp = (T*)malloc(sizeof(T) * memsize);
			std::copy(m_data, m_data + m_capacity, tmp);

			// Has already copyed the old data to the new memory, so the old memory is useless
			free(m_data);
		}
		m_data = tmp;
		m_capacity = memsize;
	}

	SSTD_INLINE void _Fill_Range(sizet start, sizet end) {
		for (; start < end; ++start) {
			new (&m_data[start]) T();
		}
	}

	SSTD_INLINE void _Fill_Range(sizet start, sizet end, const T& val) {
		for (; start < end; ++start) {
			m_data[start] = val;
		}
	}

	template<typename _Iter>
	SSTD_INLINE void _Fill_Range_Iter(sizet pos, _Iter _Start, _Iter _End) {
		for (; _Start != _End; ++pos, ++_Start) {
			m_data[pos] = *_Start;
		}
	}

	// Insert objects at a certain position.
	// First move all the existing object that will be affected 
	// Then fill in the iterator
	template<typename _Iter>
	SSTD_INLINE void _Insert_At(sizet pos, _Iter iter_beg, _Iter iter_end) {
		const sizet Dis = iter_end - iter_beg;
		const sizet Total_Cap = m_size + Dis;
		if (Total_Cap > m_capacity) {
			// Allocate more space
			_Realloc_Data(Total_Cap);
		}
		if (pos < m_size) {
			// Need to move existing objects
			if (std::is_trivially_move_constructible<T>::value) {
				std::memmove(m_data + pos + Dis, m_data + pos, sizeof(T) * (m_size - pos));
			}
			else {
				for (int i = pos; i < m_size; ++i) {
					new (m_data + Dis + i) T(std::move(m_data[i]));
				}
			}
			_Fill_Range_Iter(pos, iter_beg, iter_end);
		}
	}
	
// Iterator of vector class
public:

	// -----------------------------------------
	//
	//   Forward Iterator
	//
	// -----------------------------------------

	class iterator {
		friend class vector;
	public:
		iterator(vector<T>* vec, sizet ind) :
			m_vec(vec), m_ind(ind) {

		}

		SSTD_INLINE iterator& operator++() noexcept {
			this->m_ind++;
			return *this;
		}
		SSTD_INLINE iterator operator++(int) noexcept {
			iterator tmp = *this;
			this->m_ind++;
			return tmp;
		}
		SSTD_INLINE iterator& operator--() noexcept {
			this->m_ind--;
			return *this;
		}
		SSTD_INLINE iterator operator--(int) noexcept {
			iterator tmp = *this;
			this->m_ind--;
			return tmp;
		}

		SSTD_INLINE iterator& operator+=(const sizet dis) noexcept {
			this->m_ind += dis;
			return *this;
		}
		SSTD_INLINE iterator operator+(const sizet dis) const noexcept {
			return iterator(m_vec, m_ind + dis);
		}
		SSTD_INLINE iterator& operator-=(const sizet dis) noexcept {
			this->m_ind -= dis;
			return *this;
		}
		SSTD_INLINE iterator operator-(const sizet dis) const noexcept {
			return iterator(m_vec, m_ind - dis);
		}

		SSTD_INLINE sizet operator-(const iterator dis) const noexcept {
			return m_ind - dis.m_ind;
		}

		SSTD_INLINE T& operator*() const noexcept {
			return this->m_vec->operator[](m_ind);
		}

		SSTD_INLINE bool operator==(const iterator& other) const noexcept {
			return this->m_vec == other.m_vec && this->m_ind == other.m_ind;
		}

		SSTD_INLINE bool operator!=(const iterator& other) const noexcept {
			return this->m_vec != other.m_vec || this->m_ind != other.m_ind;
		}
	private:
		vector<T> *m_vec;
		sizet m_ind;
	};

	// -----------------------------------------
	//
	//   Reverse Iterator
	//
	// -----------------------------------------

	class reverse_iterator {
		friend class vector;
	public:
		reverse_iterator(vector<T>* vec, sizet ind) :
			m_vec(vec), m_ind(ind) {

		}

		SSTD_INLINE reverse_iterator& operator++() noexcept {
			this->m_ind--;
			return *this;
		}
		SSTD_INLINE reverse_iterator operator++(int) noexcept {
			reverse_iterator tmp = *this;
			this->m_ind--;
			return tmp;
		}
		SSTD_INLINE reverse_iterator& operator--() noexcept {
			this->m_ind++;
			return *this;
		}
		SSTD_INLINE reverse_iterator operator--(int) noexcept {
			reverse_iterator tmp = *this;
			this->m_ind++;
			return tmp;
		}

		SSTD_INLINE reverse_iterator& operator+=(const sizet dis) noexcept {
			this->m_ind -= dis;
			return *this;
		}
		SSTD_INLINE reverse_iterator operator+(const sizet dis) const noexcept {
			return reverse_iterator(m_vec, m_ind - dis);
		}
		SSTD_INLINE reverse_iterator& operator-=(const sizet dis) noexcept {
			this->m_ind += dis;
			return *this;
		}
		SSTD_INLINE reverse_iterator operator-(const sizet dis) const noexcept {
			return reverse_iterator(m_vec, m_ind + dis);
		}

		SSTD_INLINE T& operator*() const noexcept {
			return this->m_vec->operator[](m_ind);
		}

		SSTD_INLINE bool operator==(const reverse_iterator& other) const noexcept {
			return this->m_vec == other.m_vec && this->m_ind == other.m_ind;
		}

		SSTD_INLINE bool operator!=(const reverse_iterator& other) const noexcept {
			return this->m_vec != other.m_vec || this->m_ind != other.m_ind;
		}
	private:
		vector<T> *m_vec;
		sizet m_ind;
	};

	// -----------------------------------------
	//
	//   Const Iterator
	//
	// -----------------------------------------

	class const_iterator {
		friend class vector;
	public:
		const_iterator(const vector<T>* vec, sizet ind) :
			m_vec(vec), m_ind(ind) {

		}

		SSTD_INLINE const_iterator& operator++() noexcept {
			this->m_ind++;
			return *this;
		}
		SSTD_INLINE const_iterator operator++(int) noexcept {
			const_iterator tmp = *this;
			this->m_ind++;
			return tmp;
		}
		SSTD_INLINE const_iterator& operator--() noexcept {
			this->m_ind--;
			return *this;
		}
		SSTD_INLINE const_iterator operator--(int) noexcept {
			const_iterator tmp = *this;
			this->m_ind--;
			return tmp;
		}

		SSTD_INLINE const_iterator& operator+=(const sizet dis) noexcept {
			this->m_ind += dis;
			return *this;
		}
		SSTD_INLINE const_iterator operator+(const sizet dis) const noexcept {
			return const_iterator(m_vec, m_ind + dis);
		}
		SSTD_INLINE const_iterator& operator-=(const sizet dis) noexcept {
			this->m_ind -= dis;
			return *this;
		}
		SSTD_INLINE const_iterator operator-(const sizet dis) const noexcept {
			return const_iterator(m_vec, m_ind - dis);
		}

		SSTD_INLINE const T& operator*() const noexcept {
			return this->m_vec->operator[](m_ind);
		}

		SSTD_INLINE bool operator==(const const_iterator& other) const noexcept {
			return this->m_vec == other.m_vec && this->m_ind == other.m_ind;
		}

		SSTD_INLINE bool operator!=(const const_iterator& other) const noexcept {
			return this->m_vec != other.m_vec || this->m_ind != other.m_ind;
		}
	private:
		const vector<T> *m_vec;
		sizet m_ind;
	};

	// -----------------------------------------
	//
	//   Const Reverse Iterator
	//
	// -----------------------------------------

	class const_reverse_iterator {
		friend class vector;
	public:
		const_reverse_iterator(const vector<T>* vec, sizet ind) :
			m_vec(vec), m_ind(ind) {

		}

		SSTD_INLINE const_reverse_iterator& operator++() noexcept {
			this->m_ind--;
			return *this;
		}
		SSTD_INLINE const_reverse_iterator operator++(int) noexcept {
			const_reverse_iterator tmp = *this;
			this->m_ind--;
			return tmp;
		}
		SSTD_INLINE const_reverse_iterator& operator--() noexcept {
			this->m_ind++;
			return *this;
		}
		SSTD_INLINE const_reverse_iterator operator--(int) noexcept {
			const_reverse_iterator tmp = *this;
			this->m_ind++;
			return tmp;
		}

		SSTD_INLINE const_reverse_iterator& operator+=(const sizet dis) noexcept {
			this->m_ind -= dis;
			return *this;
		}
		SSTD_INLINE const_reverse_iterator operator+(const sizet dis) const noexcept {
			return const_reverse_iterator(m_vec, m_ind - dis);
		}
		SSTD_INLINE const_reverse_iterator& operator-=(const sizet dis) noexcept {
			this->m_ind += dis;
			return *this;
		}
		SSTD_INLINE const_reverse_iterator operator-(const sizet dis) const noexcept {
			return const_reverse_iterator(m_vec, m_ind + dis);
		}

		SSTD_INLINE const T& operator*() const noexcept {
			return this->m_vec->operator[](m_ind);
		}

		SSTD_INLINE bool operator==(const const_reverse_iterator& other) const noexcept {
			return this->m_vec == other.m_vec && this->m_ind == other.m_ind;
		}

		SSTD_INLINE bool operator!=(const const_reverse_iterator& other) const noexcept {
			return this->m_vec != other.m_vec || this->m_ind != other.m_ind;
		}
	private:
		const vector<T> *m_vec;
		sizet m_ind;
	};
};

SSTD_END

#endif