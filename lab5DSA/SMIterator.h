#pragma once
#include "SortedMap.h"

//DO NOT CHANGE THIS PART
class SMIterator{
	friend class SortedMap;
private:
    SortedMap& map;
	SMIterator(SortedMap& mapionar);

    int* stack;
    int stack_capacity;
    int stack_size;
    int current;
public:
	void first();
	void next();
	bool valid() const;
    TElem getCurrent() const;
    ~SMIterator();

    TElem removeCurrent();
};

