#include <iostream>

#include "Node.hh"
#include "HashTable.hh"

#define TABLE_SIZE 96

using namespace std;

template <class T> Node<T> *loopStart(Node<T> *head) {
    if (head == NULL) return NULL;

    HashTable<T, int> *table = 
        new HashTable<T, int>(TABLE_SIZE);

    table->insert(head->getData(), 1);

    Node<T> *n = head->getNext();

    while (n != NULL) {
        if (!table->hasKey(n->getData())) {
            table->insert(n->getData(), 1);
            n = n->getNext();
        } else {
            break;
        }
    }

    return n;
}

int main() {
    char init[] = {'A', 'B', 'C', 'D', 'E'};
    Node<char> *head = new Node<char>(init, 5);
    Node<char> *tail = head;
    head->print();

    if (loopStart(head) != NULL) {
        cout << "this shouldn't happen" << endl;
    }

    while (tail->getNext() != NULL) {
        tail = tail->getNext();
    }
    tail->setNext(head->getNext());

    Node<char> *n = loopStart(head);
    cout << "Loop starts at node " << n->getData() << endl;

    return 0;
}