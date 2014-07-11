#include <iostream>

#include "Node.hh"

using namespace std;

template <class T> void removeDup(Node<T> *head) {
    if (head == NULL) return;

    for (Node<T> *n = head; n->getNext() != NULL; n = n->getNext()) {
        Node<T> *nn = n;

        for (Node<T> *nn_next = nn->getNext(); nn_next != NULL; 
             nn_next = nn->getNext()) {
            if (n->getData() == nn_next->getData()) {
                nn->setNext(nn_next->getNext());
                delete nn_next;
            } else {
                nn = nn_next;
            }
        }
    }
}

int main() {
    Node<int> *head = new Node<int>(0);
    Node<int> *tail = head;
    int init[] = {0, 1, 5, 87, 12, 0, 3, 1, 12, 0, 0};

    for (int i = 0; i < 11; i++) {
        tail->insert(new Node<int>(init[i]));
    }

    head->print();
    remove_dup(head);
    head->print();
    head->freeList(head);

    return 0;
}