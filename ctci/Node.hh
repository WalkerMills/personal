#ifndef __NODE_HH__
#define __NODE_HH__

#include <cstdlib>
#include <iostream>

using namespace std;

template <class T> class Node {
private:
    Node<T> *prev;
    Node<T> *next;
    T data;

public:
    Node(T input) {
        setData(input);
        setPrev(NULL);
        setNext(NULL);
    }

    Node(T *input, unsigned length) {
        if (length < 1) {
            cerr << "Error: must specify at least one value" << endl;
            exit(EXIT_FAILURE);
        }

        setData(input[0]);
        setPrev(NULL);
        setNext(NULL);
        Node<T> *tail = this;

        for (int i = 1; i < length; i++) {
            tail->insert(new Node<T>(input[i]));
            tail = tail->getNext();
        }
    }

    T getData() {
        return this->data;
    }

    Node<T> *getPrev() {
        return this->prev;
    }

    Node<T> *getNext() {
        return this->next;
    }

    int length() {
        int count = 0;

        for (Node<T> *n = this; n != NULL; n = n->getNext()) {
            count++;
        }

        return count;
    }

    void setData(T input) {
        data = input;
    }

    void setPrev(Node<T> *n) {
        prev = n;
    }

    void setNext(Node<T> *n) {
        next = n;
    }

    static void freeList(Node<T> *head) {
        if (head != NULL) {
            for (Node<T> *n = head->getNext(); n != NULL; n = n->getNext()) {
                delete head;
                head = n;
            }

            delete head;
        }
    }

    void insert(Node<T> *n) {
        n->setNext(getNext());
        setNext(n);

        n->setPrev(this);
    }

    static void remove(Node<T> *n) {
        if (n->getPrev() == NULL && n->getNext() == NULL) {
        } else if (n->getPrev() == NULL) {
            n->getNext()->setPrev(NULL);
        } else if (n->getNext() == NULL) {
            n->getPrev()->setNext(NULL);
        } else {
            n->getNext()->setPrev(n->getPrev());
            n->getPrev()->setNext(n->getNext());
        }
    }

    void append(Node<T> *n) {
        Node<T> *tail = this;

        while (tail->getNext() != NULL) {
            tail = tail->getNext();
        }

        tail->insert(n);
    }

    void print() {
        for (Node<T> *n = this; n != NULL; n = n->getNext()) {
            cout << n->getData() << " ";
        }

        cout << endl;
    }
};

#endif