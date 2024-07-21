#include <exception>
#include "BagIterator.h"
#include "Bag.h"

using namespace std;

///theta(1)
BagIterator::BagIterator(const Bag& c): bag(c) {
    first();
}

///best case:theta(1), worst case:theta(m) m- dimensiunea tabelei, average case:theta(1)
void BagIterator::first() {
    currentSlot = 0;
    currentNode = nullptr;
    currentCount = 0;
    ///verificam primul slot care nu este gol
    while (currentSlot < bag.hash_size && bag.hash_table[currentSlot] == nullptr) {
        currentSlot++;
    }
    ///verificam daca slot < dim
    if (currentSlot < bag.hash_size) {
        currentNode = bag.hash_table[currentSlot];
        currentCount = currentNode->freq;
    }
}
///best case:theta(1), worst case:theta(m) m- dimensiunea tabelei, average case:theta(1)
void BagIterator::next() {
    // Dacă iteratorul nu este valid, nu putem avansa
    if (!valid()) {
        throw exception();
    }

    // Dacă frecvența elementului curent este mai mare decât 1, decrementăm frecvența și rămânem pe același nod
    if (currentCount > 1) {
        currentCount--;
        return;
    }

    // Dacă există un nod următor în lista curentă, avansăm la acel nod
    if (currentNode->next != nullptr) {
        currentNode = currentNode->next;
        currentCount = currentNode->freq;
        return;
    }

    // Altfel, căutăm următorul slot non-gol și ne mutăm acolo
    for (int i = currentSlot + 1; i < bag.hash_size; ++i) {
        if (bag.hash_table[i] != nullptr) {
            currentSlot = i;
            currentNode = bag.hash_table[i];
            currentCount = currentNode->freq;
            return;
        }
    }

    // Dacă nu am găsit un alt nod sau slot non-gol, setăm iteratorul la invalid
    currentNode = nullptr;
}

///theta(1)
bool BagIterator::valid() const {
    return currentNode != nullptr;
}

///theta(1)
TElem BagIterator::getCurrent() const {
    if (!valid()) {
        throw exception();
    }
    return currentNode->elem;
}
///best theta(1)
///average theta(1)
///worst theta(n)
void BagIterator::modificaCurrent(int newValue) {
    if (currentNode == nullptr) {
        throw exception();
    }

    // Calculează slotul noului element folosind funcția de hash
    int newSlot = bag.hash(newValue);
    Node* newNode = bag.hash_table[newSlot];
    Node* prevNode = nullptr;

    // Verifică dacă noul element există deja în tabelul de dispersie
    while (newNode != nullptr && newNode->elem != newValue) {
        prevNode = newNode;
        newNode = newNode->next;
    }

    if (newNode != nullptr) {
        // Cazul 1: Noul element există deja în tabelul de dispersie
        newNode->freq += 1;
    } else {
        // Cazul 2: Noul element nu există în tabelul de dispersie, creează un nou nod
        newNode = new Node{newValue, 1, bag.hash_table[newSlot]};
        bag.hash_table[newSlot] = newNode;
    }
    // Scade frecvența elementului curent
    currentNode->freq -= 1;

    // Verifică dacă trebuie șters nodul curent
    if (currentNode->freq == 0) {
        // Trebuie să ștergem nodul curent
        if (prevNode != nullptr) {
            prevNode->next = currentNode->next;
        } else {
            bag.hash_table[currentSlot] = currentNode->next;
        }
        delete currentNode;
    }

    // Actualizează currentNode și currentCount pentru a reflecta noul element curent
    currentNode = newNode;
    currentCount = newNode->freq;
}
