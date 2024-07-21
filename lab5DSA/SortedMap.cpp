#include "SortedMap.h"
#include <exception>
#include "SMIterator.h"
using namespace std;
///theta(n)
// Constructorul implicit al clasei SortedMap
SortedMap::SortedMap(Relation r) {
    this->relation = r;
    this->capacity = 10;
    this->elements = new Node[capacity];
    this->root = -1;
    this->length = 0;
    this->firstEmpty = 0;

    // Inițializarea listei de locații libere
    for (int i = 0; i < capacity - 1; ++i) {
        elements[i].left = i + 1;
    }
    elements[capacity - 1].left = -1;
}
///theta(n)
// Funcția de redimensionare a array-ului
void SortedMap::resize() {
    int newCapacity = capacity * 2;
    Node* newElements = new Node[newCapacity];

    // Copierea elementelor existente în noul array
    for (int i = 0; i < capacity; ++i) {
        newElements[i] = elements[i];
    }

    // Inițializarea noilor locații libere
    for (int i = capacity; i < newCapacity - 1; ++i) {
        newElements[i].left = i + 1;
    }
    newElements[newCapacity - 1].left = -1;

    // Eliberarea memoriei vechi și actualizarea pointerilor
    delete[] elements;
    elements = newElements;
    firstEmpty = capacity;
    capacity = newCapacity;
}

///theta(1) add copil la radacina
///theta(logn) balanciert
///theta(n) no balanciert
///O(n)
// Funcția de adăugare a unei perechi (cheie, valoare)
TValue SortedMap::add(TKey k, TValue v) {
    // Verificăm dacă nu mai avem noduri libere și redimensionăm dacă este necesar
    if (firstEmpty == -1) {
        resize();
    }

    // Alocăm un nou nod folosind primul nod liber
    int newNode = firstEmpty;
    firstEmpty = elements[firstEmpty].left;

    // Inițializăm noul nod cu valorile primite
    elements[newNode].info = {k, v};
    elements[newNode].left = -1;
    elements[newNode].right = -1;
    elements[newNode].parinte = -1;

    // Dacă arborele este gol, noul nod devine rădăcina arborelui
    if (root == -1) {
        root = newNode;
        length++;
        return NULL_TVALUE;
    } else {
        int current = root;
        int parent = -1;

        // Parcurgem arborele pentru a găsi locul potrivit pentru noul nod
        while (current != -1) {
            parent = current;
            // Dacă găsim un nod cu aceeași cheie, actualizăm valoarea și eliberăm nodul nou alocat
            if (elements[current].info.first == k) {
                TValue old_val = elements[current].info.second;
                elements[current].info.second = v;
                elements[newNode].left = firstEmpty;
                firstEmpty = newNode;
                return old_val;
            }
            // Decidem să mergem pe ramura stângă sau dreaptă în funcție de relația dintre chei
            if (relation(k, elements[current].info.first)) {
                current = elements[current].left;
            } else {
                current = elements[current].right;
            }
        }

        // Setăm părintele noului nod și atașăm noul nod la părintele potrivit
        elements[newNode].parinte = parent;
        if (relation(k, elements[parent].info.first)) {
            elements[parent].left = newNode;
        } else {
            elements[parent].right = newNode;
        }
        length++;
        return NULL_TVALUE;
    }
}

///theta(1) cautam radacina
///theta(logn)balanciert
///theta(n)no balaciert
///O(n)
// Funcția de căutare a unei chei
TValue SortedMap::search(TKey k) const {
    int current = root;

    // Parcurgem arborele binar de căutare
    while (current != -1) {
        // Dacă am găsit nodul cu cheia căutată, returnăm valoarea asociată
        if (elements[current].info.first == k) {
            return elements[current].info.second;
        }
        // Dacă cheia căutată este mai mică decât cheia curentă, mergem la fiul stâng
        if (relation(k, elements[current].info.first)) {
            current = elements[current].left;
        } else { // Altfel, mergem la fiul drept
            current = elements[current].right;
        }
    }

    // Dacă nu am găsit cheia, returnăm valoarea NULL_TVALUE
    return NULL_TVALUE;
}

///theta(1)
///theta(logn)
///theta(n)
// Funcția de ștergere a unei chei
TValue SortedMap::remove(TKey k){
    int current = root;
    int parent = -1;

    // Parcurgem arborele pentru a găsi nodul care trebuie eliminat
    while (current != -1 && elements[current].info.first != k) {
        parent = current;
        if (relation(k, elements[current].info.first)) {
            current = elements[current].left;
        } else {
            current = elements[current].right;
        }
    }

    // Dacă nodul nu a fost găsit, returnăm NULL_TVALUE
    if (current == -1) {
        return NULL_TVALUE;
    }

    TValue old_val = elements[current].info.second;

    // Cazul când nodul are cel mult un copil
    if (elements[current].left == -1 || elements[current].right == -1) {
        int child;
        // Determinăm copilul nodului curent
        if (elements[current].left != -1)
            child = elements[current].left;
        else
            child = elements[current].right;

        // Ajustăm legăturile părintelui nodului curent
        if (parent == -1) {
            root = child; // Nodul curent este rădăcina
        } else if (elements[parent].left == current) {
            elements[parent].left = child;
        } else {
            elements[parent].right = child;
        }

        // Actualizăm părintele copilului dacă nu este nul
        if (child != -1) {
            elements[child].parinte = parent;
        }
    } else {
        // Cazul când nodul are doi copii
        int nextParent = current;
        int nextChild = elements[current].right;

        // Găsim succesorul (cel mai mic nod din subarborele drept)
        while (elements[nextChild].left != -1) {
            nextParent = nextChild;
            nextChild = elements[nextChild].left;
        }

        // Copiem informația succesorului în nodul curent
        elements[current].info = elements[nextChild].info;

        // Ajustăm legăturile părintelui succesorului
        if (nextParent != current) {
            elements[nextParent].left = elements[nextChild].right;
        } else {
            elements[nextParent].right = elements[nextChild].right;
        }

        // Actualizăm părintele fiului drept al succesorului, dacă există
        if (elements[nextChild].right != -1) {
            elements[elements[nextChild].right].parinte = nextParent;
        }

        // Ne asigurăm că nodul curent este succesorul pentru a-l putea elibera
        current = nextChild;
    }

    // Eliberăm nodul curent
    elements[current].left = firstEmpty;
    firstEmpty = current;
    length--;
    return old_val;
}

///theta(1)
// Funcția care returnează dimensiunea mapei
int SortedMap::size() const {
    return length;
}
///theta(1)
// Funcția care verifică dacă mapa este goală
bool SortedMap::isEmpty() const {
    return root == -1;
}
///theta(1)
// Funcția care returnează iteratorul pentru mapă
SMIterator SortedMap::iterator()  {
    return SMIterator(*this);
}
///theta(1)
// Destructorul clasei SortedMap
SortedMap::~SortedMap() {
    delete[] elements;
}
