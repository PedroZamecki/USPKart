#ifndef DATA_HPP
#define DATA_HPP

#include "object/object.hpp"

class Data
{
public:
	Data() = default;
	~Data() { delete[] objects; }

private:
	Object *objects;
};

#endif // DATA_HPP
