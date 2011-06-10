#include <iostream>
#include "FarmDelegate.h"
#include "FarmDelegateSimple.h"

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

class dummy2 {
	public:
		int omg(int e, int f) {
			cout << "omg " << e << endl;
			return e * f;
		}
};

dummy y;

int main()
{
	// simple delegate - no dynamic objects
    FarmDelegateSimple<int, int, int> s;
    s.Set(foo);
    cout << "s(5, 6) = " << s(5, 6) << endl;
    s.Set<dummy, &y, &dummy::bar>();
    cout << "s(7, 8) = " << s(7, 8) << endl;

    // universal delegate - support for dynamic objects
    FarmDelegate<int, int, int> d;
    d.Set(foo);
    cout << "d(1, 3) = " << d(1, 3) << endl;
    dummy x;
    d.Set<dummy>(&x, &dummy::bar);
    cout << "d(2, 4) = " << d(2, 4) << endl;
    dummy2 z;
    d.Set<dummy2>(&z, &dummy2::omg);
    cout << "d(9, 10) = " << d(9, 10) << endl;

    return 0;
}
