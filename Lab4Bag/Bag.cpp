#include "Bag.h"
#include "BagIterator.h"
#include <exception>
#include <iostream>

using namespace std;

///theta(1)
Bag::Bag() {
    hash_size = 10; ///dim tabel
    hash_table = new Node*[hash_size]();
    elements_count = 0;
    prag = 0.75;/// prag pentru resize
}

///theta(1)
int Bag::hash(TElem e) const {
    return abs(e) % hash_size;
}

///theta(n) trebuie rearenjate toate elem din vechiul tabel
void Bag::resize() {
    int old_size = hash_size;
    hash_size *= 2;
    Node** new_table = new Node*[hash_size];
    for (int i = 0; i < hash_size; i++){
        new_table[i] = nullptr;
    }

    for (int i = 0; i < old_size; i++) {
        Node* current = hash_table[i];///curent = capul listei de noduri
        while (current != nullptr) {
            Node* next = current->next;
            int new_index = hash(current->elem);
            current->next = new_table[new_index];
            new_table[new_index] = current;
            current = next;
        }
    }

    delete[] hash_table;
    hash_table = new_table;}


///best case:theta(1)frecventa este actalizata fara resize
///average case:theta(1)
///worst case:theta(n) avem nevoie de resize
///allgemeine O(n)
void Bag::add(TElem e) {
    if (float(elements_count) / float(hash_size) > prag) {
        resize();
    }

    int index = hash(e);
    Node* current = hash_table[index];
    Node* prev = nullptr;

    // Căutăm elementul în lista de la idex ul calculat
    while (current != nullptr){
        if (current->elem == e){
            break; //Elementul a fost gasit
        }
        prev = current;
        current = current->next;
    }

    // Dacă elementul a fost găsit, actualizăm frecvența acestuia
    if (current != nullptr) {
        current->freq++;
    } else {
        // Dacă elementul nu a fost găsit, adăugăm un nod nou
        Node* new_node = new Node{ e, 1, nullptr };
        if(prev == nullptr) {
            hash_table[index] = new_node;
        } else {
            prev->next = new_node;
        }
    }

    // Incrementăm numărul total de elemente
    elements_count++;
}
///best case:theta(1) elementul este gasit la inceputul listei
///average case:theta(1) listele sunt scurte
///worst case:theta(n) cand toate elementele sunt hashuite pe aceasi pozitie in hashtable
///allgemeine O(n)
bool Bag::remove(TElem e) {
    // Calculăm indexul pentru elementul `e` folosind funcția hash
    int index = hash(e);
    Node* current = hash_table[index];
    Node* prev = nullptr;

    // Parcurgem lista de la indexul calculat pentru a găsi elementul
    while (current != nullptr) {
        if (current->elem == e) {
            break; // Elementul a fost găsit
        }
        prev = current;
        current = current->next;
    }

    // Dacă elementul nu a fost găsit, returnăm false
    if (current == nullptr) {
        return false;
    }

    // Gestionăm cazul în care frecvența elementului este mai mare de 1
    if (current->freq > 1) {
        current->freq--;
    } else {
        // Dacă frecvența elementului este 1, îl eliminăm din listă
        if (prev == nullptr) {
            // Elementul este primul nod din lista la indexul dat
            hash_table[index] = current->next;
        } else {
            // Legăm nodul anterior cu următorul nod
            prev->next = current->next;
        }
        delete current;
    }

    // Decrementăm numărul total de elemente
    elements_count--;
    return true;
}


///best case:theta(1) elem este la inceputul listei
///average case:theta(1)
///worst case:theta(n) din cauza coliziunilor(elementele sunt in aceasi lista)
///allgemeine O(n)
bool Bag::search(TElem e) const {
    int index = hash(e);
    Node* current = hash_table[index];

    while (current != nullptr) {
        if (current->elem == e) {
            return true;
        }
        current = current->next;
    }
    return false;
}

///best case:theta(1) este la inceputul listei
///average case:theta(1)
///worst case:theta(n) din cauza coliziunilor
///allgemeine O(n)
int Bag::nrOccurrences(TElem e) const {
    int index = hash(e);
    Node* current = hash_table[index];

    while (current != nullptr) {
        if (current->elem == e) {
            return current->freq;
        }
        current = current->next;
    }
    return 0;

}
///theta(1)
int Bag::size() const {
    return elements_count;
}

///theta(1)
bool Bag::isEmpty() const {
    return elements_count == 0;
}

BagIterator Bag::iterator() const {
    return BagIterator(*this);
}

///best case: theta(m) m-dimensiunea tabelei daca tabelul este gol
///average case: theta(n+m) n-nr nod
///worst case:theta(n+m) -toate sloturile sunt oucpate
///allgemeine O(n+m);';
Bag::~Bag() {
    // Parcurgem fiecare slot din hash_table
    for (int i = 0; i < hash_size; ++i) {
        Node* current = hash_table[i];

        // Eliberăm nodurile din lista de la fiecare slot folosind o abordare diferită
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp; // Ștergem nodul curent după actualizarea pointerului
        }
    }

    // Eliberăm memoria alocată pentru tabela hash
    delete[] hash_table;
}



