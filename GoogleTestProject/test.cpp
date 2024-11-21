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