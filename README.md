# Лабораторная работа 2
Dynamic Array
## Array class
```c++
#pragma once
#include <cstdlib>  // malloc и free
#include <utility>  // std::move

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
```
## Movable class
Для тестирования перемещаемых объектов.
```c++
using namespace std;

template<typename T>
class Movable final
{
private:
	T* data;
public:
	Movable()
	{
		data = new T();
	}

	Movable(T value)
	{
		data = new T(value);
	}

	Movable(const Movable& other)
	{
		data = new T(*(other.data));
	}

	Movable(Movable&& other) noexcept : data(other.data)
	{
		other.data = nullptr;
	}

	~Movable()
	{
		try
		{
			free(data);
		}
		catch (const std::exception&) { }

		data = nullptr;
	}

	Movable& operator=(const Movable& other) 
	{
		free(data);
		data = new T(*(other.data));
		return *this;
	}

	Movable& operator=(Movable&& other) noexcept
	{
		free(data);
		data = other.data;
		other.data = nullptr;
		return *this;
	}

	const T* const Get() const
	{
		return data;
	}
};
```
## Тесты
```c++
#include "pch.h"
#include "../AlgoLab2_2/MyArrayClass.h"
#include "../AlgoLab2_2/Movable.h"

TEST(Constructor, DefaultConstructor)
{
	Array<int> a;
	ASSERT_EQ(a.GetCapacity(), 8);
	ASSERT_EQ(a.GetSize(), 0);
};

TEST(Constructor, DefaultConstructorWithMovable)
{
	Array<Movable<int>> b;
	ASSERT_EQ(b.GetCapacity(), 8);
	ASSERT_EQ(b.GetSize(), 0);
};

TEST(Constructor, ConstructorByValue)
{
	Array<int> a(100);
	ASSERT_EQ(a.GetCapacity(), 100);
	ASSERT_EQ(a.GetSize(), 0);
};

TEST(Constructor, ConstructorByValueWithMovable)
{
	Array<Movable<int>> b(100);
	ASSERT_EQ(b.GetCapacity(), 100);
	ASSERT_EQ(b.GetSize(), 0);
};

TEST(Get, GetByInvalidIndex)
{
	Array<int> a;
	ASSERT_EQ(&a[0], nullptr);
	ASSERT_EQ(&a[10], nullptr);	
	for (int i = 0; i < 10; i++)
	{
		a.Insert(i);
	}
	ASSERT_EQ(&a[-1], nullptr);
	ASSERT_EQ(&a[10], nullptr);
};

TEST(Insert, Push)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(i);
	}
	for (int i = 0; i < 10; i++)
	{
		ASSERT_EQ(a[i], i);
	}
	ASSERT_EQ(a.GetCapacity(), 17);
	ASSERT_EQ(a.GetSize(), 10);
};

TEST(Insert, PushWithMovable)
{
	Array<Movable<int>> b;
	for (int i = 0; i < 10; i++)
	{
		Movable<int> v(i);
		b.Insert(v);
		ASSERT_EQ(v.Get(), nullptr);
	}
	for (int i = 0; i < 10; i++)
	{
		ASSERT_EQ(*(b[i].Get()), i);
	}
	ASSERT_EQ(b.GetCapacity(), 17);
	ASSERT_EQ(b.GetSize(), 10);
};

TEST(Insert, InsertToValidIndex)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(0, i);
	}
	for (int i = 9; i >= 0; i--)
	{
		ASSERT_EQ(a[i], 9 - i);
	}
	ASSERT_EQ(a.GetCapacity(), 17);
	ASSERT_EQ(a.GetSize(), 10);
};

TEST(Insert, InsertToValidIndexWithMovable)
{
	Array<Movable<int>> b;
	for (int i = 0; i < 10; i++)
	{
		Movable<int> v(i);
		b.Insert(0, v);
		ASSERT_EQ(v.Get(), nullptr);
	}
	for (int i = 9; i >= 0; i--)
	{
		ASSERT_EQ(*(b[i].Get()), 9 - i);
	}
	ASSERT_EQ(b.GetCapacity(), 17);
	ASSERT_EQ(b.GetSize(), 10);
};

TEST(Insert, InsertToInvalidIndex)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(0, i);
	}
	a.Insert(-200, 11111);
	a.Insert(200, 99999);

	ASSERT_EQ(a[0], 11111);
	ASSERT_EQ(a[a.GetSize() - 1], 99999);
};

TEST(Remove, RemoveByValidIndex)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(i);
	}
	a.Remove(5);
	for (int i = 0; i < 10; i++)
	{
		if (i < 5)
		{
			ASSERT_EQ(a[i], i);
		}
		else if (i == 9)
		{
			ASSERT_EQ(&a[i], nullptr);
		}
		else
		{
			ASSERT_EQ(a[i], i + 1);
		}
	}
	ASSERT_EQ(a.GetCapacity(), 17);
	ASSERT_EQ(a.GetSize(), 9);
};

TEST(Remove, RemoveByValidIndexWithMovable)
{
	Array<Movable<int>> a;
	for (int i = 0; i < 10; i++)
	{
		Movable<int> v(i);
		a.Insert(v);
		ASSERT_EQ(v.Get(), nullptr);
	}

	a.Remove(5);
	for (int i = 0; i < 9; i++)
	{
		if (i < 5)
		{
			ASSERT_EQ(*(a[i].Get()), i);
		}
		else
		{
			ASSERT_EQ(*(a[i].Get()), i + 1);
		}
	}

	ASSERT_EQ(&a[9], nullptr);

	ASSERT_EQ(a.GetCapacity(), 17);
	ASSERT_EQ(a.GetSize(), 9);
};

TEST(Remove, RemoveByInvalidIndex)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(i);
	}
	a.Remove(-1);
	a.Remove(10);
	ASSERT_EQ(a.GetCapacity(), 17);
	ASSERT_EQ(a.GetSize(), 10);
};

TEST(Iterator, ForwardIterator)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(i);
	}
	
	int i = 0;
	for (auto it = a.GetIterator(); it.HasNext(); it.Next())
	{
		ASSERT_EQ(it.Get(), i);
		i++;
	}
};

TEST(Iterator, ReverseIterator)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(i);
	}

	int i = 9;
	for (auto it = a.GetReverseIterator(); it.HasNext(); it.Next())
	{
		ASSERT_EQ(it.Get(), i);
		i--;
	}
};

TEST(Iterator, Set)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(i);
	}

	int i = 0;
	for (auto it = a.GetIterator(); it.HasNext(); it.Next())
	{
		it.Set(i + 100);
		i++;
	}
	
	for (int j = 0; j < 10; j++)
	{
		ASSERT_EQ(a[j], j + 100);
	}
}

TEST(Iterator, SetWithMovable)
{
	Array<Movable<int>> a;
	for (int i = 0; i < 10; i++)
	{
		Movable<int> v(i);
		a.Insert(v);
	}

	int i = 0;
	for (auto it = a.GetIterator(); it.HasNext(); it.Next())
	{
		it.Set(std::move(Movable<int>(100 + i)));
		i++;
	}

	for (int j = 0; j < 10; j++)
	{
		ASSERT_EQ(*(a[j].Get()), j + 100);
	}
}

TEST(Iterator, ForwardConstIterator)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(i);
	}

	int i = 0;
	for (auto it = a.GetConstIterator(); it.HasNext(); it.Next())
	{
		ASSERT_EQ(it.Get(), i);
		i++;
	}
};

TEST(Iterator, ReverseConstIterator)
{
	Array<int> a;
	for (int i = 0; i < 10; i++)
	{
		a.Insert(i);
	}

	int i = 9;
	for (auto it = a.GetReverseConstIterator(); it.HasNext(); it.Next())
	{
		ASSERT_EQ(it.Get(), i);
		i--;
	}
};
```
![image](https://github.com/user-attachments/assets/a561ecb6-e1b3-4fbf-acfc-8c0d61478bc3)
