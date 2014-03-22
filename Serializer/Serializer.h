#ifndef STERILISER_H
#define STERILISER_H

#include <string>
#include <iostream>
#include <typeinfo>

#include <tinyxml2.h>

using std::wstring;
using std::cout;
using std::cin;
using std::endl;

class Serializer;

namespace loader{
	static int beginSerialize(const char* typeName, const Serializer& parent);

	//here defined all posible overloads for loading different types of variables
	template<typename T>
	static int task(T& var, const Serializer& parent);

	static int endSerialize(const char* typeName, const Serializer& parent);
};

namespace saver{
	static int beginSerialize(const char* typeName, const Serializer& parent);

	//here defined all posible overloads for saving different types of variables
	template<typename T>
	static int task(const T& var, const Serializer& parent);

	static int endSerialize(const char* typeName, const Serializer& parent);
};

class Serializer {
	mutable tinyxml2::XMLDocument serialiser_document;

	friend int saver::beginSerialize(const char* typeName, const Serializer& parent);
	template<typename T> friend static int saver::task(const T& var, const Serializer& parent);
	friend int saver::endSerialize(const char* typeName, const Serializer& parent);

	friend int loader::beginSerialize(const char* typeName, const Serializer& parent);
	template<typename T> friend static int loader::task(T& var, const Serializer& parent);
	friend int loader::endSerialize(const char* typeName, const Serializer& parent);
public:
	virtual int save(const wstring& save_where) = 0;

	virtual int load(const wstring& save_from) = 0;
};

namespace loader{
	static int beginSerialize(const char* typeName, const Serializer& parent) {
		parent.serialiser_document.LoadFile("dream.xml");
		return 0;
	}

	//here defined all posible overloads for loading different types of variables
	template<typename T>
	static int task(T& var, const Serializer& parent) {
		cin >> var;
		return 0;
	}

	static int endSerialize(const char* typeName, const Serializer& parent) {
		return 0;
	}
};

namespace saver{
	static int beginSerialize(const char* typeName, const Serializer& parent) {
		parent.serialiser_document.LoadFile("dream.xml");
		return 0;
	}


	//here defined all posible overloads for saving different types of variables
	template<typename T>
	static int task(const T& var, const Serializer& parent) {
		cout << var << endl;
		return 0;
	}

	static int endSerialize(const char* typeName, const Serializer& parent) {
		return 0;
	}
};

#define REG_MEMBER(x) task(x, *this);

#define SAVER(membs) \
int save(const wstring& save_where) { \
	using namespace saver; \
	beginSerialize(typeid(*this).name(), *this);\
	membs \
	endSerialize(typeid(*this).name(), *this); \
	return 0; \
}

#define LOADER(membs) \
int load(const wstring& save_from) { \
	using namespace loader; \
	beginSerialize(typeid(*this).name(), *this); \
	membs \
	endSerialize(typeid(*this).name(), *this); \
	return 0; \
}

#define SAVE(var) SAVER(var) LOADER(var)


#endif //STERILISER_H