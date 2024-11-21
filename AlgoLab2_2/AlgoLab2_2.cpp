#include <iostream>
#include "MyArrayClass.h"
#include "Movable.h"
#include <string>

using namespace std;

template<typename T>
static void print(Array<T> *a)
{
	cout << (*a).GetSize() << endl << (*a).GetCapacity() << endl;
	for (int i = 0; i < (*a).GetSize(); i++)
	{
		cout << (*a)[i] << "	";
	}
	cout << endl;
	cout << endl;
}

int main()
{
	/*Array<int> a;
	print(&a);

	for (int i = 0; i < 10; )
	{
		a.Insert(++i);
	}	
	print(&a);

	for (auto it = a.GetReverseConstIterator(); it.HasNext(); it.Next())
		cout << it.Get() << std::endl;

	delete &a;

	print(&a);*/
	
	Array<Movable<int>> a;
	for (int i = 0; i < 10; i++)
	{
		Movable<int> v(i);
		a.Insert(v);
		cout << (a[i].Get() == nullptr) << endl;
	}
	a.Remove(5);
	for (int i = 0; i < 10; i++)
	{
		if (i < 5)
		{
			cout << *(a[i].Get()) << endl;
		}
		else if (i == 9)
		{
			cout << (&a[i] == nullptr) << endl;
		}
		else
		{
			cout << *(a[i].Get()) << endl;
		}
	}
}