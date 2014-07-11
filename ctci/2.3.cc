#include <iostream>

#include "Node.hh"

template <class T> void removeNode(Node<T> *n) {
    Node<T> *tmp = n->getNext();

    n->setData(tmp->getData());
    n->setNext(tmp->getNext());

    delete tmp;
}

int main() {
    int init[] = {0, 1, 2, 3, 4};
    Node<int> *head = new Node<int>(init, 5);

    head->print();
    removeNode(head->getNext());
    head->print();

    return 0;
}