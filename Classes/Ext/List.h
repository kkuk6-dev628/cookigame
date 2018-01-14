#ifndef __CONNON_LIST_H__
#define __CONNON_LIST_H__

#define LIST_DELETEALL(list, T)		(list).removeAll([](T* item) { if (item != nullptr) delete item; })

template <class T>
class List
{
public:
	static const int DEFAULT_SIZE = 8;
	static const int DEFAULT_STEP = 8;

	List() {
		_data = new T[DEFAULT_SIZE];
		memset(_data, 0, sizeof(T) * DEFAULT_SIZE);
		_size = DEFAULT_SIZE;
		_count = 0;
	}

	List(int size) {
		if (size <= 0)
			size = DEFAULT_SIZE;
		_data = new T[size];
		memset(_data, 0, sizeof(T) * size);
		_size = size;
		_count = 0;
	};

	~List() {
		delete _data;
	};

	void setSize(int size) {
		if (size > _size) {
			T* buf = new T[size];
			if (_count > 0)
				memcpy(buf, _data, sizeof(T) * _count);
			memset(buf + _count, 0, sizeof(T) * (size - _count));
			delete _data;
			_data = buf;
			_size = size;
		}
	}

	int getCount() { return _count; }
	int getSize() { return _size; }
	bool isEmpty() { return _count == 0; }

	void add(T data) {
		if (_count >= _size)
			setSize(_size + DEFAULT_STEP);
		_data[_count++] = data;
	};

	void insert(T data, int index) {
		if (_count >= _size)
			setSize(_size + DEFAULT_STEP);
		if (index < 0)
			index = 0;
		else if (index > _count)
			index = _count;
		for (int i = _count; i > index; i--)
			_data[i] = _data[i - 1];
		_data[index] = data;
		_count++;
	}

	T getAt(int index) {
		if (index >= 0 && index < _count)
			return _data[index];
		return NULL;
	};

	void setAt(int index, T data)
	{
		if (index >= 0 && index < _count)
			_data[index] = data;
	}

	T getLast() {
		if (_count > 0)
			return _data[_count - 1];
		return NULL;
	}

	void removeLast() {
		_count--;
	}

	void removeAt(int index) {
		if (index >= 0 && index < _count)
		{
			for (int i = index; i < _count; i++)
				_data[i] = _data[i + 1];
			_count--;
		}
	}

	void remove(T& t) {
		int index = indexOf(t);
		if (index != -1)
			removeAt(index);
	}

	void removeAll(std::function<void(T&)> callback)
	{
		if (callback != nullptr)
			for (int i = 0; i < _count; i++)
				callback(_data[i]);
		_count = 0;
	}

    T& operator[](int index)
    {
        return _data[index];
    }
    
    const T& operator[](int index) const
    {
        return _data[index];
    }

	void clear() {
		_count = 0;
	}

	void swap(int i, int j)
	{
		T temp = _data[i];
		_data[i] = _data[j];
		_data[j] = temp;
	}

	int indexOf(T& t)
	{
		for (int i = 0; i < _count; i++)
			if (_data[i] == t)
				return i;
		return -1;
	}

	bool contains(T& t)
	{
		return indexOf(t) != -1;
	}

	//RETURN: index or -1
	int find(std::function<bool(T&)> condition, T* result)
	{
		return find(0, _count - 1, condition, result);
	}

	int find(int startIndex, std::function<bool(T&)> condition, T* result)
	{
		return find(startIndex, _count - 1, condition, result);
	}

	int find(int startIndex, int endIndex, std::function<bool(T&)> condition, T* result)
	{
		for (int i = startIndex; i <= endIndex; i++)
		{
			if (condition(_data[i]))
			{
				if (result != nullptr)
					*result = _data[i];
				return i;
			}
		}
		return -1;
	}

	void sort(std::function<bool(T&,T&)> compare/*swap condition for i, j*/)
	{
		for (int i = 0; i < _count - 1; i++)
		{
			for (int j = i + 1; j < _count; j++)
			{
				if (compare(_data[i], _data[j]))
					swap(i, j);
			}
		}
	}

	void copyTo(List<T>& other)
	{
		other.clear();
		other.setSize(this->_count);
		other._count = this->_count;
		memcpy(other._data, this->_data, sizeof(T) * _count);
	}

	List<T>* clone()
	{
		List<T>* other = new List<T>(_count);
		this->copyTo(*other);
		return other;
	}

private:
	T* _data;
	int _size;
	int _count;
};

#endif //__CONNON_LIST_H__