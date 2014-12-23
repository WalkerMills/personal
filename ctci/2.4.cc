#include <iostream>

#include "Node.hh"

using namespace std;

Node<int> *addNodes(Node<int> *n1, Node<int> *n2) {
    if (n1 == NULL || n2 == NULL) {
        return NULL;
    } else {
        return new Node<int>(n1->getData() + n2->getData());
    }
}

int truncateNode(Node<int> *n) {
    if (n->getData() > 9) {
        n->setData(n->getData() % 10);
        return 1;
    } else {
        return 0;
    }
}

Node<int> *addLists(Node<int> *n1, Node<int> *n2) {
    Node<int> *head = addNodes(n1, n2);

    if (head == NULL) return NULL;

    Node<int> *tail = head;
    Node<int> *n;

    int buffer = truncateNode(head);

    n1 = n1->getNext();
    n2 = n2->getNext();

    while (n1 != NULL && n2 != NULL) {
        n = addNodes(n1, n2);
        n->setData(n->getData() + buffer);
        buffer = truncateNode(n);

        tail->insert(n);
        tail = n;
        n1 = n1->getNext();
        n2 = n2->getNext();
    }

    if (n1 != NULL) {
        n = n1;
    } else if (n2 != NULL) {
        n = n2;
    } else {
        n = NULL;
    }

    for ( ; n != NULL && buffer > 0; n = n->getNext()) {
        n->setData(n->getData() + buffer);
        buffer = truncateNode(n);

        tail->setNext(n);
        tail = n;
    }

    if (buffer > 0) {
        tail->append(new Node<int>(buffer));
    }

    return head;
}

int main() {
    int init1[] = {8, 2, 5};
    int init2[] = {4, 3, 7};

    Node<int> *n1 = new Node<int>(init1, 3);
    Node<int> *n2 = new Node<int>(init2, 3);

    Node<int> *n3 = new Node<int>(init1[2]);
    Node<int> *n4 = new Node<int>(init2[2]);

    n1->print();
    cout << "+" << endl;
    n2->print();
    cout << "=" << endl;
    addLists(n1, n2)->print();
    cout << endl;

    n3->print();
    cout << "+" << endl;
    n4->print();
    cout << "=" << endl;
    addLists(n3, n4)->print();
    cout << endl;

    n2->print();
    cout << "+" << endl;
    n4->print();
    cout << "=" << endl;
    addLists(n2, n4)->print();
    cout << endl;

    return 0;
}
