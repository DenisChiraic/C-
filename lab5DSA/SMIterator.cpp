#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;
///theta(1) cand nu are subarbore stang
///theta(logn)
///theta(n)
SMIterator::SMIterator(SortedMap& d) :map(d) {
    stack_capacity = map.size();
    stack = new int[stack_capacity];
    stack_size = 0;
    current = -1;

    first();
}
///theta(1)
SMIterator::~SMIterator() {
    delete[] stack;
}

///theta(1) cand nu are subarbore stang
///theta(logn)
///theta(n) parcurgem tot arborele
void SMIterator::first() {
    stack_size = 0; // Inițializăm dimensiunea stivei la zero
    current = map.root; // Începem de la rădăcina arborelui

    // Adăugăm toți descendenții din stânga ai rădăcinii în stivă
    while (current != -1) {
        stack[stack_size++] = current;
        current = map.elements[current].left;
    }

    // Dacă stiva nu este goală, setăm current la ultimul element adăugat în stivă
    if (stack_size > 0) {
        current = stack[--stack_size];
    } else {
        current = -1; // Dacă stiva este goală, arborele este gol și iterarea este invalidă
    }
}

///theta(1) cand nu are subarbore drept
///theta(logn)
///theta(n)
void SMIterator::next() {
    // Verificăm dacă iteratorul este valid; dacă nu, aruncăm o excepție
    if (!valid()) {
        throw exception();
    }

    // Ne deplasăm la fiul drept al nodului curent
    current = map.elements[current].right;

    // Adăugăm toți descendenții din stânga ai nodului curent în stivă
    while (current != -1) {
        stack[stack_size++] = current;
        current = map.elements[current].left;
    }

    // Dacă stiva nu este goală, setăm current la ultimul element adăugat în stivă
    if (stack_size > 0)
        current = stack[--stack_size];
    else
        current = -1; // Dacă stiva este goală, am terminat iterarea
}

///theta(1)
bool SMIterator::valid() const {
    return current != -1;
}
///theta(1)
TElem SMIterator::getCurrent() const {
    if (!valid()) {
        throw exception();
    }
    return map.elements[current].info;
}

/// in iterator o functie de remove current!!!!
/// Elimină elementul curent
///theta(1)
TElem SMIterator::removeCurrent() {
    if (!valid()) {
        throw exception();
    }

    int nodeToRemove = current;

    // Avansăm iteratorul la următorul element înainte de a elimina nodul curent
    next();

    // Eliminăm nodul curent din SortedMap
    map.remove(map.elements[nodeToRemove].info.first);

    // Reinitializam stiva si pozitionam iteratorul la elementul corect
    stack_size = 0;
    first();
    while (valid() && map.elements[current].info.first != map.elements[current].info.first) {
        next();
    }
}