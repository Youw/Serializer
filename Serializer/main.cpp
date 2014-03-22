#include "Serializer.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

//only for print_example
#define makestr4(x) # x
#define makestr3(x) makestr4(x)
#define makestr2(x) makestr3(x)
#define makestr1(x) makestr2(x)
#define makestr(x) makestr1(x)


class Point : Serializer {
public:
	int x;
	int y;
	int z;
public:
	void print_example() {
		cout << makestr(SAVE(
			REG_MEMBER(x)
			REG_MEMBER(y)
			)) << endl;
	}

	// example using Sterilizer
	SAVE(
		REG_MEMBER(x)
		REG_MEMBER(y)
	)

};

void printPoint(const Point& p) {
	cout << "Point " << &p << ":" << endl
		<< "x = " << p.x << ";" << endl
		<< "y = " << p.y << ";" << endl
		<< "z = " << p.z << ";" << endl;
}

int main() {
	Point p;
	p.print_example();

	printPoint(p);

	p.x = 5;
	p.y = 8;
	p.z = 2;
	p.save(L"foo");
	printPoint(p);

	cout << "enter x and y" << endl;
	p.load(L"foo");
	printPoint(p);

	system("pause");
	return 0;
}