#pragma once
#include <vector>
#include <iostream>
template <typename T>
class Base_iterator
{
public:
	Base_iterator(T* first = nullptr) :
		current_(first)
	{ }
	Base_iterator<T> operator++ ()
	{
		++current_;
		return *this;
	}
	Base_iterator<T> operator-- ()
	{
		--current_;
		return *this;
	}
	Base_iterator<T> operator++ (int)
	{
		Base_iterator<T> tmp(current_);
		++current_;
		return tmp;
	}
	Base_iterator<T> operator-- (int)
	{
		Base_iterator<T> tmp(current_);
		--current_;
		return tmp;
	}
	T& operator*()
	{
		return *current_;
	}
	bool operator< (Base_iterator<T> other)
	{
		if (other.current_ < current_)
			return true;
		else
			return false;
	}
	bool operator== (Base_iterator<T> other)
	{
		if (other.current_ == current_)
			return true;
		else
			return false;
	}
	bool operator!= (Base_iterator<T> other)
	{
		if (other.current_ != current_)
			return true;
		else
			return false;
	}
	Base_iterator<T> operator+(int count)
	{
		Base_iterator<T> rez(current_);
		rez.current_ += count;
		return rez;
	}
	T* current_;
};
template <typename T>
class Base_reverse_iterator : public Base_iterator<T>
{
public:
	Base_reverse_iterator<T>(T* cur, T* ptr_begin) : Base_iterator<T>(cur),
		ptr_begin_(ptr_begin) { }
	Base_reverse_iterator<T> operator++ ()
	{
		if (Base_iterator<T>::current_ != ptr_begin_)
		{
			--Base_iterator<T>::current_;
			return *this;
		}
		Base_iterator<T>::current_ = nullptr;
		return*this;
	}
	Base_reverse_iterator<T> operator-- ()
	{
		if (Base_iterator<T>::current_ != nullptr)
		{
			++Base_iterator<T>::current_;
			return*this;
		}
		Base_iterator<T>::current_ = ptr_begin_;
		return*this;
	}
	Base_reverse_iterator<T> operator++ (int)
	{
		Base_iterator<T> tmp(Base_iterator<T>::current_, ptr_begin_);
		++* this;
		return tmp;
	}
	Base_reverse_iterator<T> operator-- (int)
	{
		Base_iterator<T> tmp(Base_iterator<T>::current_, ptr_begin_);
		--* this;
		return tmp;
	}
protected:
	T* ptr_begin_;
};


template <typename T>
class DynArray
{
public:
	// constructor
	DynArray()
	{
		size_ = 0;
		capacity_ = 0;
		array_ = nullptr;
	}
	DynArray(size_t count) :
		size_(count), capacity_(count * 2)
	{
		array_ = new T[size_ * 2];
	}
	DynArray(Base_iterator<T> start, Base_iterator<T> end)
	{
		int ind = 0;
		for (Base_iterator<T> it = start; it != end; ++it)
			++ind;
		size_ = ind;
		capacity_ = size_ * 2;
		array_ = new T[size_ * 2];
		Base_iterator<T> this_it = begin();
		for (Base_iterator<T> it = start; it != end; ++it)
		{
			*this_it = *it;
			++this_it;
		}
	}
	DynArray(DynArray& init) :
		size_(init.size_), capacity_(init.capacity_)
	{
		if (size_ > 0)
			array_ = new T[capacity_];
		else
			array_ = nullptr;
		std::memcpy(array_, init.array_, size_ * sizeof(T));
	}
	DynArray(DynArray&& init) :
		size_(init.size_), capacity_(init.capacity_)
	{
		if (size_ > 0)
			array_ = new T[capacity_];
		else
			array_ = nullptr;
		std::memcpy(array_, init.array_, size_ * sizeof(T));
	}
	// destructor
	~DynArray() {
		delete[] array_;
		array_ = nullptr;
	}
	// methods
	void assign(size_t count, const T& value) {
		++size_;
		if (count >= capacity_)
			change_capacity(count * 2);
		for (int i = 0; i < count; ++i)
			array_[i] = value;
		size_ = count;
	}
	void assign(std::initializer_list<T> ilist) {
		++size_;
		size_t ilist_size = ilist.size();
		if (ilist_size >= capacity_)
			change_capacity(ilist_size * 2);
		int i = 0;
		for (auto it : ilist)
		{
			array_[i] = it;
			++i;
		}
		size_ = ilist_size;
	}
	void assign(Base_iterator<T> first, Base_iterator<T> last) {
		int ind = 0;
		size_ = 0;
		for (Base_iterator<T> it = first; it != last; ++it)
		{
			++size_;
			if (capacity_ == 0)
			{
				size_ = 1;
				change_capacity(1);
			}
			else if (ind >= capacity_)
				change_capacity(ind * 2);
			array_[ind] = *it;
			++ind;
		}
	}
	T& at(size_t ind) {
		try
		{
			if (ind < 0 || ind >= size_)
				throw  123;
		}
		catch (int i)
		{
			std::cout << "Index was out of range\n";
		}
		return array_[ind];
	}
	T& front() {
		return array_[0];
	}
	T& back() {
		return array_[size_ - 1];
	}
	bool empty() {
		if (begin() == end())
			return true;
		return false;
	}
	size_t size() {
		return size_;
	}
	size_t capacity() {
		return capacity_;
	}
	void shrink_to_fit() {
		if (capacity_ == size_)
			return;
		T* tmp = array_;
		array_ = nullptr;
		array_ = new T[size_];
		std::memcpy(array_, tmp, sizeof(T) * (size_));
		delete[] tmp;
		capacity_ = size_;
	}
	void clear() {
		size_ = 0;
	}
	Base_iterator<T> erase(Base_iterator<T> pos) {
		Base_iterator<T> end_of_arr = end();
		if (pos == end_of_arr)
		{
			pop_back();
			return end_of_arr;
		}
		for (Base_iterator<T> it = pos; it != end_of_arr; ++it)
			*it = *(it + 1);
		pop_back();
		return pos;
	}
	Base_iterator<T> erase(Base_iterator<T> first, Base_iterator<T> last) {
		if (first == last)
			return first;
		size_t delta_size = 0;
		bool is_end = false;
		if (last == end())
			is_end = true;
		for (Base_iterator<T> it = first; it != last; ++it)
			++delta_size;
		Base_iterator<T> first_it = first;
		Base_iterator<T> second_it = last;
		while (second_it != end())
		{
			*first_it = *second_it;
			++first_it;
			++second_it;
		}
		size_ -= delta_size;
		if (is_end)
			return end();
		return first;

	}
	Base_iterator<T> insert(Base_iterator<T> pos, const T& value) {
		if (++size_ > capacity_)
			change_capacity(size_ * 2);
		size_t ind = 0;
		for (Base_iterator<T> it = begin(); it != pos; ++it)
			++ind;
		for (int i = size_ - 1; i > ind; --i)
			array_[i] = array_[i - 1];
		array_[ind] = value;
		return pos;
	}
	Base_iterator<T> insert(Base_iterator<T> pos, const T&& value) {
		if (++size_ > capacity_)
			change_capacity(size_ * 2);
		size_t ind = 0;
		for (Base_iterator<T> it = begin(); it != pos; ++it)
			++ind;
		for (int i = size_ - 1; i > ind; --i)
			array_[i] = array_[i - 1];
		array_[ind] = value;
		return pos;
	}
	void insert(Base_iterator<T> pos, size_t count, const T& value) {
		++size_;
		int temp_ind = 0;
		int temp_i = 0;
		if (size_ + count - 1 > capacity_)
		{
			Base_iterator<T> temp = begin();
			while (temp != pos)
			{
				++temp;
				++temp_ind;
			}
			change_capacity((size_ + count - 1) * 2);
			pos = begin();;
			while (temp_i != temp_ind)
			{
				++pos;
				++temp_i;
			}
		}	
		size_t ind = 0;
		for (Base_iterator<T> it = begin(); it != pos; ++it)
			++ind;
		for (int i = size_ + count - 2, j = size_ - 2; j >= ind; --j, --i)
		{
			if (j < 0 || i < 0)
				break;
			array_[i] = array_[j];
		}
		for (int i = ind; i < ind + count; ++i)
			array_[i] = value;
		size_ += count - 1;
	}
	void insert(Base_iterator<T> pos, Base_iterator<T> first, Base_iterator<T> last) {
		size_t delta_size = 0;
		size_t ind = 0;
		++size_;
		for (Base_iterator<T> it = begin(); it != pos; ++it)
			++ind;
		for (Base_iterator<T> it = first; it != last; ++it)
			++delta_size;
		if (size_ + delta_size - 1 > capacity_)
			change_capacity((size_ + delta_size) * 2);
		for (int i = size_ + delta_size - 2, j = size_ - 2; j >= ind; --j, --i)
		{
			if (j < 0 || i < 0)
				break;
			array_[i] = array_[j];
		}
		for (Base_iterator<T> it = first; it != last; ++it)
		{
			*pos = *it;
			++pos;
		}
		size_ += delta_size;
		--size_;
	}
	Base_iterator<T> insert(Base_iterator<T> pos, std::initializer_list<T> ilist) {
		++size_;
		if (size_ + ilist.size() - 1 > capacity_)
			change_capacity((size_ + ilist.size() - 1) * 2);
		size_t ind = 0;
		for (Base_iterator<T> it = begin(); it != pos; ++it)
			++ind;
		for (int i = size_ + ilist.size() - 2, j = size_ - 2; j >= ind; --j, --i)
		{
			if (j < 0 || i < 0)
				break;
			array_[i] = array_[j];
		}
		for (auto it : ilist)
		{
			array_[ind] = it;
			++ind;
		}
		size_ += ilist.size() - 1;
		return pos;
	}
	void resize(size_t count) {
		if (count > size_)
		{
			++size_;
			change_capacity(count * 2);
			for (int i = size_ - 1; i < count; ++i)
				array_[i] = NULL;
		}
		size_ = count;
	}
	void push_back(T input) {
		if (++size_ > capacity_)
			change_capacity(size_ * 2);
		array_[size_ - 1] = input;
	}
	void pop_back() {
		if (size_ == 0)
			return;
		--size_;
		array_[size_] = NULL;
	}

	Base_iterator<T> begin() {
		return Base_iterator<T>(&array_[0]);
	}
	Base_iterator<T> end() {
		return Base_iterator<T>(&array_[size_]);
	}
	Base_reverse_iterator<T> rbegin() {
		return Base_reverse_iterator<T>(&array_[size_ - 1], array_);
	}
	Base_reverse_iterator<T> rend() {
		return Base_reverse_iterator<T>(nullptr, array_);
	}
	Base_iterator<const T> cbegin() {
		return Base_iterator<const T>(&array_[0]);
	}
	Base_iterator<const T> cend() {
		return Base_iterator<const T>(&array_[size_]);
	}
	Base_reverse_iterator<const T> crbegin() {
		return Base_reverse_iterator<const T>(&array_[size_ - 1], array_);
	}
	Base_reverse_iterator<const T> crend() {
		return Base_reverse_iterator<const T>(nullptr, array_);
	}
	// operators
	T& operator[] (size_t ind)
	{
		return array_[ind];
	}
	bool operator==(DynArray<T> dyn_arr)
	{
		if (size_ != dyn_arr.size_)
			return false;
		else
		{
			int i = 0;
			while (i < size_ && i < dyn_arr.size_)
			{
				if (array_[i] != dyn_arr.array_[i])
					return false;
				++i;
			}
		}
		return true;
	}
	bool operator!=(DynArray<T> dyn_arr)
	{
		if (size_ != dyn_arr.size_)
			return true;
		else
		{
			int i = 0;
			while (i < size_ && i < dyn_arr.size_)
			{
				if (array_[i] != dyn_arr.array_[i])
					return true;
				++i;
			}
		}
		return false;
	}
	DynArray<T>& operator=(DynArray<T> dyn_arr)
	{
		if (size_ < dyn_arr.size_)
		{
			++size_;
			change_capacity(dyn_arr.capacity_);
		}
		size_ = dyn_arr.size_;
		capacity_ = dyn_arr.capacity_;
		std::memcpy(array_, dyn_arr.array_, dyn_arr.size_ * sizeof(T));
		return *this;
	}
protected:
	void change_capacity(size_t new_capacity) {
		if (capacity_ == new_capacity)
			return;
		T* tmp = array_;
		array_ = nullptr;
		array_ = new T[new_capacity];
		if (new_capacity > capacity_)
			std::memcpy(array_, tmp, sizeof(T) * (size_ - 1));
		else
			std::memcpy(array_, tmp, sizeof(T) * (new_capacity - 1));
		delete[] tmp;
		capacity_ = new_capacity;
		if (size_ > capacity_)
			size_ = capacity_ - 1;
	}
	size_t size_;
	size_t capacity_;
	T* array_;
};
template <typename T>
class String :protected DynArray<T>
{
public:
	// constructor
	String() { 
	
	}
	String(String<T>&& init) { }
	String(const String<T>& init) {
		DynArray<T>::size_ = init.DynArray<T>::size_;
		DynArray<T>::capacity_ = init.DynArray<T>::capacity_;
		if (DynArray<T>::size_ > 0)
			DynArray<T>::array_ = new T[DynArray<T>::capacity_];
		else
			DynArray<T>::array_ = nullptr;
		std::memcpy(DynArray<T>::array_, init.DynArray<T>::array_, DynArray<T>::size_ * sizeof(T));
	}
	String(T* string) {
		DynArray<T>::size_ = std::strlen(string);
		DynArray<T>::capacity_ = DynArray<T>::size_ * 2;
		if (DynArray<T>::size_ > 0)
			DynArray<T>::array_ = new T[DynArray<T>::capacity_];
		else
			DynArray<T>::array_ = nullptr;
		for (int i = 0; i < DynArray<T>::size_; ++i)
			DynArray<T>::array_[i] = string[i];
		DynArray<T>::array_[DynArray<T>::size_] = '\0';
	}
	String(std::initializer_list<T> ilist) {
		DynArray<T>::assign(ilist);
	}
	String(std::string str) {
		DynArray<T>::size_ = str.length();
		size_t size_tmp = DynArray<T>::size_;
		DynArray<T>::capacity_ = size_tmp * 2;
		if (size_tmp > 0)
			DynArray<T>::array_ = new T[DynArray<T>::capacity_];
		else
			DynArray<T>::array_ = nullptr;
		for (int i = 0; i < size_tmp; ++i)
			DynArray<T>::array_[i] = str[i];
		DynArray<T>::array_[size_tmp] = '\0';
	}
	String(const T* string, size_t count) {
		DynArray<T>::size_ = count;
		DynArray<T>::capacity_ = count * 2;
		DynArray<T>::array_ = new T[DynArray<T>::capacity_];
		for (int i = 0; i < count; ++i)
			DynArray<T>::array_[i] = string[i];
	}
	String(size_t count, T value) {
		DynArray<T>::assign(count, value);
	}
	// destructor
	~String() { }
	// methods
	T* c_str() {
		return DynArray<T>::array_;
	}
	size_t size() {
		return DynArray<T>::size_;
	}
	size_t length() {
		return DynArray<T>::size_;
	}
	size_t capacity() {
		return DynArray<T>::capacity_;
	}
	bool empty() {
		return DynArray<T>::empty();
	}
	void shrink_to_fit() {
		DynArray<T>::shrink_to_fit();
	}
	void clear() {
		DynArray<T>::clear();
	}
	void insert(size_t ind, size_t count, T value) {
		Base_iterator<T> it = DynArray<T>::begin();
		int i = 0;
		while (i != ind)
		{
			++i;
			++it;
		}
		DynArray<T>::insert(it, count, value);
	}
	void insert(size_t ind, const T* string) {
		size_t delta_size = std::strlen(string);
		int begin_for_new = DynArray<T>::size_;
		if (delta_size + DynArray<T>::size_ > DynArray<T>::capacity_)
		{
			++DynArray<T>::size_;
			DynArray<T>::change_capacity((delta_size + DynArray<T>::size_) * 2);
			--DynArray<T>::size_;
		}	
		DynArray<T>::size_ += delta_size;
		for (int i = begin_for_new-1, j = DynArray<T>::size_; i >= ind; --i, --j)
		{
			if (i < 0 || j < 0)
				break;
			DynArray<T>::array_[j] = DynArray<T>::array_[i];
		}
		for (int i = 0, j = ind; i < delta_size; ++i, ++j)
			DynArray<T>::array_[j] = string[i];
	}
	void insert(size_t ind, const T* string, size_t count) {
		T* str = new T[count + 1];
		for (int i = 0; i < count; ++i)
			str[i] = string[i];
		str[count] = '\0';
		insert(ind, str);
	}
	void insert(size_t ind, std::string string) {
		T* str = new T[string.length() + 1];
		for (int i = 0; i < string.length(); ++i)
			str[i] = string[i];
		str[string.length()] = '\0';
		insert(ind, str);
	}
	void insert(size_t ind, std::string string, size_t count) {
		T* str = new T[count + 1];
		for (int i = 0; i < count; ++i)
			str[i] = string[i];
		str[count] = '\0';
		insert(ind, str);
	}
	void erase(size_t ind, size_t count) {
		Base_iterator<T> it = DynArray<T>::begin();
		Base_iterator<T> first;
		Base_iterator<T> last;
		for (int i = 0; i <= ind + count; ++i)
		{
			if (i == ind)
				first = it;
			if (i == ind + count)
				last = it;
			++it;
		}
		DynArray<T>::erase(first, last);
	}
	void append(size_t count, T value) {
		if (DynArray<T>::size_ + count >= DynArray<T>::capacity_)
		{
			++DynArray<T>::size_;
			DynArray<T>::change_capacity(2 * (DynArray<T>::size_ + count));
			--DynArray<T>::size_;
		}
		//--DynArray<T>::size_;
		for (int i = DynArray<T>::size_; i < DynArray<T>::size_ + count; ++i)
			DynArray<T>::array_[i] = value;
		DynArray<T>::size_ += count;
	}
	void append(const T* string) {
		insert(DynArray<T>::size_, string);
	}
	void append(const T* string, size_t ind, size_t count) {
		T* str = new T[count + 1];
		for (int i = 0, j = ind; j < ind + count; ++j, ++i)
			str[i] = string[j];
		insert(DynArray<T>::size_, str);
	}
	void append(std::string string) {
		insert(DynArray<T>::size_, string);
	}
	void append(std::string string, size_t ind, size_t count) {
		T* str = new T[count + 1];
		for (int i = 0, j = ind; j < ind + count; ++j, ++i)
			str[i] = string[j];
		insert(DynArray<T>::size_, str);
	}
	void replace(size_t ind, size_t count, const T* string) {
		erase(ind, count);
		insert(ind, string);
	}
	void replace(size_t ind, size_t count, std::string string) {
		replace(ind, count, string.c_str());
	}
	void substr(size_t ind) {
		erase(0, ind);

	}
	void substr(size_t ind, size_t count) {
		erase(0, ind);
		erase(count, size() - count);
	}
	int find(const T* str) {
		return find(str, 0);
	}
	int find(const T* str, size_t ind) {
		T c;
		for (int i = ind; i <= DynArray<T>::size_ - std::strlen(str); ++i)
		{
			c = *(DynArray<T>::array_ + i + std::strlen(str));
			*(DynArray<T>::array_ + i + std::strlen(str)) = 0;
			if (!std::strcmp(DynArray<T>::array_ + i, str))
			{
				*(DynArray<T>::array_ + i + std::strlen(str)) = c;
				return i;
			}
			*(DynArray<T>::array_ + i + std::strlen(str)) = c;
		}
		return -1;
	}
	int find(std::string string) {
		return find(string.c_str(), 0);
	}
	int find(std::string string, size_t ind) {
		return find(string.c_str(), ind);
	}
	// operators
	T& operator[] (size_t ind)
	{
		return DynArray<T>::array_[ind];
	}
	String<T>& operator+=(const T* string)
	{
		size_t size = std::strlen(string);
		insert(DynArray<T>::size_, string, size);
		return *this;
	}
	String<T>& operator+=(std::string& string)
	{
		return(*this += string.c_str());
	}
	String<T>& operator+=(String<T>& string)
	{
		return(*this += string.DynArray<T>::array_);
	}
	String<T>& operator=(String<T> string)
	{
		clear();
		T* str = new T[string.DynArray<T>::size_ + 1];
		for (int i = 0; i < string.DynArray<T>::size_; ++i)
			str[i] = string.DynArray<T>::array_[i];
		str[string.DynArray<T>::size_] = '\0';
		insert(0, str);
		return *this;
	}
	String<T>& operator=(const T* string)
	{
		clear();
		insert(0, string);
		return *this;
	}
	String<T>& operator=(std::string string)
	{
		clear();
		insert(0, string);
		return *this;
	}
	String<T>& operator=(T c)
	{
		clear();
		DynArray<T>::push_back(c);
		return *this;
	}
	bool operator==(String<T> string)
	{
		if (DynArray<T>::size_ != string.DynArray<T>::size_)
			return false;
		else
		{
			int i = 0;
			while (i < DynArray<T>::size_ && i < string.DynArray<T>::size_)
			{
				if (DynArray<T>::array_[i] != string.DynArray<T>::array_[i])
					return false;
				++i;
			}
		}
		return true;
	}
	bool operator!=(String<T> string)
	{
		if (DynArray<T>::size_ != string.DynArray<T>::size_)
			return true;
		else
		{
			int i = 0;
			while (i < DynArray<T>::size_ && i < string.DynArray<T>::size_)
			{
				if (DynArray<T>::array_[i] != string.DynArray<T>::array_[i])
					return true;
				++i;
			}
		}
		return false;
	}
	bool operator>(String<T>& string)
	{
		if (std::strcmp(this->c_str(), string.c_str_) > 0)
			return true;
		else
			return false;
	}
	bool operator<(String<T>& string)
	{
		if (std::strcmp(this->c_str(), string.c_str_) < 0)
			return true;
		else
			return false;
	}
	bool operator>=(String<T>& string)
	{
		if (std::strcmp(this->c_str(), string.c_str_) >= 0)
			return true;
		else
			return false;
	}
	bool operator<=(String<T>& string)
	{
		if (std::strcmp(this->c_str(), string.c_str_) <= 0)
			return true;
		else
			return false;
	}
	template <typename T> friend std::basic_ostream<T>& operator<< (std::basic_ostream<T>& out, const String<T>& str) {
		for (int i = 0; i < str.size_; ++i)
			out << str.array_[i];
		return out;
	}
	template <typename T> friend std::istream& operator>> (std::istream& is, String<T>& str) {
		str.clear();
		int i, cur_size = 0;;
		char c;
		for (i = 0; (c = is.get()) != '\n'; ++i)
		{
			++cur_size;
			++str.size_;
			if (str.capacity_ <= cur_size) str.change_capacity(2 * cur_size);
			str.size_ = 2 * cur_size;
			str.array_[i] = c;
		}
		str.array_[i] = '\0';
		return is;
	}
};


