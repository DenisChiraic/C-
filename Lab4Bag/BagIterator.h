#pragma once
#include "Bag.h"

class BagIterator {
    // DO NOT CHANGE THIS PART
    friend class Bag;

private:
    const Bag& bag;

    int currentSlot;
    Node* currentNode;
    int currentCount;

    // Private constructor
    BagIterator(const Bag& c);

public:
    // Sets the iterator to the first element in the bag
    void first();

    // Moves the iterator to the next element in the bag
    void next();

    // Returns the current element in the bag
    TElem getCurrent() const;

    // Checks if the iterator is valid (points to a valid element)
    bool valid() const;

    // functi care modifica curentul
    void modificaCurrent(int newValue);
};
