#pragma once
#include <cstdlib>  // malloc и free
#include <utility>  // std::move

// При попытке разделить класс на .h и .cpp возникает "Ошибка средств компоновщика LNK2019"
// Не знаю, как исправить

template<typename T>
class Array final
{
private:
	T* data;				// указатель на массив данных
	int capacity;			// емкость массива
	int count;				// количество элементов

	void Extend()
	{
		capacity = capacity * 2 + 1;
		T* newData = static_cast<T*>(malloc(capacity * sizeof(T)));
		for (int i = 0; i < count; ++i)
		{
			new(newData + i) T(std::move(data[i]));
			data[i].~T();
		}
		free(data);
		data = newData;
	}
public:

#pragma region Constructors
	Array() : capacity(8), count(0)
	{
		data = static_cast<T*>(malloc(capacity * sizeof(T)));
	}

	Array(int capacity) : capacity(capacity), count(0)
	{
		data = static_cast<T*>(malloc(capacity * sizeof(T)));
	}

	~Array()
	{
		for (int i = 0; i < count; i++)
		{
			data[i].~T();
		}
		free(data);
		capacity = 0;
		count = 0;
	}
#pragma endregion

#pragma region ArrayManipulations

	int Insert(const T& value)
	{
		if (count == capacity) Extend();
		new(data + count) T(std::move(value));
		value.~T();
		return count++;
	}

	int Insert(int index, const T& value)
	{
		if (count == capacity) Extend();
		MakeIndexValidForInsert(&index);
		for (int i = count; i > index; i--) 
		{
			new(data + i) T(std::move(data[i - 1]));
			data[i - 1].~T();
		}
		new(data + index) T(std::move(value));
		value.~T();
		count++;
		return index;
	}

	void Remove(int index)
	{
		if (!CheckIndex(index)) return;
		data[index].~T();
		for (int i = index; i < count - 1; ++i) 
		{
			new(data + i) T(std::move(data[i + 1]));
			data[i + 1].~T();
			//(data + i + 1) = nullptr;
		}
		count--;
	}

#pragma endregion

#pragma region Iterators
	class Iterator
	{
	private:
		T* current;
		T* start;
		T* end;
		int direction;

	public:
		Iterator(T* start, T* end, int dir) : start(start), end(end), direction(dir)
		{
			current = dir < 0 ? end : start;
		}

		T& Get()
		{
			return *current;
		}

		void Set(const T& value)
		{
			*current = value;
		}

		void Set(T&& value)
		{
			*current = std::move(value);
		}

		void Next()
		{
			if (HasNext())
				current += direction;
		}

		bool HasNext() const
		{
			return direction < 0 ? current != start : current != end;
		}
	};

	class ConstIterator
	{
	private:
		T* current;
		T* start;
		T* end;
		int direction;

	public:
		ConstIterator(T* start, T* end, int dir) : start(start), end(end), direction(dir)
		{
			current = dir < 0 ? end : start;
		}

		const T& Get() const
		{
			return *current;
		}

		void Next()
		{
			if (HasNext())
				current += direction;
		}

		bool HasNext() const
		{
			return direction < 0 ? current != start : current != end;
		}
	};

	Iterator GetIterator()
	{
		return Iterator(data, data + count, 1);
	}

	Iterator GetReverseIterator()
	{
		return Iterator(data - 1, data + count - 1, -1);
	}

	ConstIterator GetConstIterator()
	{
		return ConstIterator(data, data + count, 1);
	}

	ConstIterator GetReverseConstIterator()
	{
		return ConstIterator(data - 1, data + count - 1, -1);
	}
#pragma endregion

	const T& operator[](int index) const
	{
		if (!CheckIndex(index))
		{
			T* val = nullptr;
			return *val;
		}
		return data[index];
	}

	T& operator[](int index)
	{
		if (!CheckIndex(index))
		{
			T* val = nullptr;
			return *val;
		}
		return data[index];
	}

	int GetSize() const
	{
		return count;
	}

	int GetCapacity() const
	{
		return capacity;
	}


private:
	void MakeIndexValidForInsert(int *index)
	{
		*index = *index < 0 ? 0 : *index;
		*index = *index > count ? count : *index;
	}	

	bool CheckIndex(int index)
	{
		return index >= 0 && index < count;
	}
};