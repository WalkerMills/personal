#include <cstdlib>

#include <iostream>

#include "Node.hh"

using namespace std;

template <class T> Node<T> *nthToLast(Node<T> *head, int n) {
    int length = head->length();

    if (n > length - 1) {
        cerr << "Error: index out of bounds" << endl;
        exit(EXIT_FAILURE);
    } else if (n < 0) {
        cerr << "Error: n must be positive" << endl;
        exit(EXIT_FAILURE);
    }

    Node<T> *node = head;

    for (int i = 0; i < length - 1 - n; i++) {
        node = node->getNext();
    }

    return node;
}

int main() {
    int init[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    Node<int> *head = new Node<int>(init, 10);
    head->print();
    int x;

    while (cin >> x) {
        cout << x << " before last: " << nthToLast(head, x)->getData() << endl;
    }

    return 0;
}