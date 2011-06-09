#include <iostream>
#include "FarmDelegate.h"

using namespace std;

int foo(int a, int c) {
	cout << "foo " << a << endl;
	return a * c;
}

class dummy {
	public:
		int bar(int b, int d) {
			cout << "bar " << b << endl;
			return b * d;
		}
};

dummy x;

int main()
{
    FarmDelegate<int, int, int> d;
    d.Set(foo);
    cout << "d(1, 3) = " << d(1, 3) << endl;
    d.Set<dummy, &x, &dummy::bar>();
    cout << "d(2, 4) = " << d(2, 4) << endl;
    return 0;
}
