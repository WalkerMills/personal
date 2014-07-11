#ifndef __GENERIC_STACK__
#define __GENERIC_STACK__

#include <cstdlib>

#include <algorithm>
#include <deque>
#include <iostream>

using namespace std;

template <class T, class C = deque<T> > class Stack {
private:
    C *container;

protected:
    void setContainer(C *container) {
        this->container = container;
    }

    C *getContainer() {
        return this->container;
    }

public:
    Stack() {
        setContainer(new C);
    }

    unsigned getSize() {
        return getContainer()->size();
    }

    void push(T value) {
        getContainer()->push_back(value);
    }

    void pop() {
        getContainer()->pop_back();
    }

    T top() {
        return getContainer()->back();
    }

    void print() {
        C *container = getContainer();

        for (unsigned i = 0; i < getSize(); i++) {
            cout << container->at(i) << " ";
        }
        cout << endl;
    }
};

template <class T> class ArrayStack {
private:
    unsigned size;
    unsigned empty;
    unsigned top_index;
    T *base_ptr;

    void resizeContainer() {
        unsigned size = getSize();
        unsigned empty = getEmpty();

        if (empty * 3 < size) {
            T *base_ptr = getContainer();
            T *new_base_ptr = new T[size * 2];

            copy(base_ptr, base_ptr + size - empty, new_base_ptr);
            setContainer(new_base_ptr);
            delete [] base_ptr;

            setSize(size * 2);
            setEmpty(empty + size);
        }
    }

protected:
    void setSize(unsigned size) {
        this->size = size;
    }
    void incSize() {
        setSize(getSize() + 1);
    }
    void decSize() {
        setSize(getSize() - 1);
    }

    void setEmpty(unsigned empty) {
        this->empty = empty;
    }
    void incEmpty() {
        setEmpty(getEmpty() + 1);
    }
    void decEmpty() {
        setEmpty(getEmpty() - 1);
    }

    void setTopIndex(unsigned top_index) {
        this->top_index = top_index;
    }
    void incTopIndex() {
        setTopIndex(getTopIndex() + 1);
    }
    void decTopIndex() {
        setTopIndex(getTopIndex() - 1);
    }

    void setContainer(T *base_ptr) {
        this->base_ptr = base_ptr;
    }

    void setTop(T value) {
        getContainer()[getTopIndex()] = value;
    }

    unsigned getEmpty() {
        return this->empty;
    }
    unsigned getTopIndex() {
        return this->top_index;
    }
    T *getContainer() {
        return this->base_ptr;
    }

public:
    ArrayStack() {
        setSize(0);
        setEmpty(0);
        setContainer(NULL);
        setTopIndex(0);
    }

    ArrayStack(unsigned size) {
        if (size < 1) {
            cerr << "Error: stack size must be > 0" << endl;
            exit(EXIT_FAILURE);
        }

        setSize(size);
        setEmpty(size);
        setContainer(new T[size]);
        setTopIndex(0);
    }

    ~ArrayStack() {
        delete [] getContainer();
    }

    unsigned getSize() {
        return this->size;
    }

    void push(T value) {
        if (getSize() != getEmpty()) {
            incTopIndex();
        }

        setTop(value);
        decEmpty();
        resizeContainer();
    }

    void pop() {
        if (getSize() == getEmpty()) {
            cerr << "Error: stack underflow" << endl;
            exit(EXIT_FAILURE);
        }

        decTopIndex();
        incEmpty();
    }

    T top() {
        if (getSize() == getEmpty()) {
            cerr << "Error: stack underflow" << endl;
            exit(EXIT_FAILURE);
        }

        return getContainer()[getTopIndex()];
    }

    void print() {
        T *base_ptr = getContainer();

        for (unsigned i = 0; i < getSize() - getEmpty(); i++) {
            cout << *(base_ptr + i) << " ";
        }
        cout << endl;
    }
};

#endif