#pragma once
#include <string>

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