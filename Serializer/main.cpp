#include <iostream>

#include "Serializer.h"

class SerialTest: public Serializer {
public:
  double x, X;

  SAVE(
      REG_MEMBER(x)
      REG_MEMBER(X)
      )
};

class SerialParentTest: public Serializer {
public:
  double lol1, lol2;
  SerialTest child;

  SAVE(
      REG_MEMBER(lol1)
      REG_MEMBER(lol2)
      REG_MEMBER(child)
      )
};


int main()
{
	try {
		SerialParentTest t;
		t.child.x = 1;
		t.child.X = 2;
		t.lol1 = 3;
		t.lol2 = 4;
		t.save("serial.xml");
		t.load("serial.xml");
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}
  return 0;
}

