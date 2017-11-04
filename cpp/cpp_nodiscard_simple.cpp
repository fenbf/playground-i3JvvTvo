#include <iostream>

class SimpleType
{
private:
	int a { 1 };    // << wow!
	int b { 1 };    // << wow2!
	std::string name { "string" }; // wow3!

public:
	SimpleType() {
		std::cout << "SimpleType::ctor, {" << a << ", " << b << ", \"" << name << "\"}\n";
	}
	SimpleType(int aa, int bb) : a(aa), b(bb) {
		std::cout << "SimpleType::ctor(aa, bb), {" << a << ", " << b << ", \"" << name << "\"}\n";
	}
	~SimpleType() { std::cout << "SimpleType::destructor\n"; }
};

class AdvancedType
{
private:
	SimpleType simple;
	
public:
	AdvancedType() {
		std::cout << "AdvancedType::ctor\n";
	}
	AdvancedType(int a) : simple(a, a) {
		std::cout << "AdvancedType::ctor(a)\n";
	}
	~AdvancedType() { std::cout << "AdvancedType::destructor\n"; }
};

int main()
{
	SimpleType simpleObj;
	AdvancedType advObj;
	AdvancedType advObj2(10);
}