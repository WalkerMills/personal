#include <cstdlib>

#include <iostream>

#include "Node.hh"
#include "Stack.hh"

using namespace std;

template <class T> class ExternalArrayStack : public ArrayStack<T> {
private:
    bool external_array;

protected:
    void setExternal(bool is_external) {
        this->external_array = is_external;
    }

    bool isExternal() {
        return this->external_array;
    }

public:
    ExternalArrayStack(unsigned size) : ArrayStack<T>(size) {
        setExternal(false);
    }

    ExternalArrayStack(T *array, unsigned size) {
        if (size < 1) {
            cerr << "Error: stack size must be > 0" << endl;
            exit(EXIT_FAILURE);
        } else if (array == NULL) {
            cerr << "Error: array is NULL" << endl;
            exit(EXIT_FAILURE);
        }

        setExternal(true);

        this->setSize(size);
        this->setEmpty(size);
        this->setContainer(array);
        this->setTopIndex(0);
    }

    ~ExternalArrayStack() {
        if (!isExternal()) {
            delete [] this->getContainer();
        }
    }

    void push(T value) {
        if (isExternal()) {
            if (this->getEmpty() == 0) {
                cerr << "Error: stack overflow" << endl;
                exit(EXIT_FAILURE);
            }

            if (this->getSize() != this->getEmpty()) {
                this->incTopIndex();
            }

            this->setTop(value);
            this->decEmpty();
        } else {
            ArrayStack<T> *base = dynamic_cast<ArrayStack<T> *>(this);
            base->push(value);
        }
    }
};

template <class T> class StackSet {
private:
    class IndexStack {
    private:
        Node<unsigned> *used;

        void setUsed(Node<unsigned> *used) {
            this->used = used;
        }

    public:
        IndexStack(unsigned i) {
            setUsed(new Node<unsigned>(i));
        }

        ~IndexStack() {
            Node<unsigned>::freeList(getUsed());
        }

        Node<unsigned> *getUsed() {
            return this->used;
        }

        void push(unsigned i) {
            Node<unsigned> *n = new Node<unsigned>(i);
            
            n->setNext(getUsed());
            if (getUsed() != NULL) {
                getUsed()->setPrev(n);
            }

            setUsed(n);  
        }

        void pop() {
            if (getUsed() == NULL) {
                cerr << "Error: stack underflow" << endl;
                exit(EXIT_FAILURE);
            }

            Node<unsigned>::remove(getUsed());
        }

        unsigned top() {
            if (getUsed() == NULL) {
                cerr << "Error: stack underflow" << endl;
                exit(EXIT_FAILURE);
            }

            return getUsed()->getData();
        }

        void print() {
            for (Node<unsigned> *n = getUsed(); n != NULL; n = n->getNext()) {
                cout << n->getData() << " ";
            }
            cout << endl;
        }
    };

    Node<unsigned> *head;
    unsigned nstacks;
    IndexStack **stacks;    
    T *array;

protected:
    void setNStacks(unsigned nstacks) {
        this->nstacks = nstacks;
    }

    void setStacks(IndexStack **stacks) {
        this->stacks = stacks;
    }

    void setStack(unsigned i, IndexStack *stack) {
        getStacks()[i] = stack;
    }

    void setArray(T *array) {
        this->array = array;
    }

    void setValue(unsigned i, T value) {
        getArray()[i] = value;
    }

    void setHead(Node<unsigned> *head) {
        this->head = head;
    }

    Node<unsigned> *getHead() {
        return this->head;
    }

    IndexStack **getStacks() {
        return this->stacks;
    }

    IndexStack *getStack(unsigned stack) {
        return getStacks()[stack];
    }

    T *getArray() {
        return this->array;
    }

    T getValue(unsigned i) {
        return getArray()[i];
    }

public:
    StackSet(unsigned nstacks, T *array, unsigned size) {
        if (size < 1) {
            cerr << "Error: array size must be > 0" << endl;
            exit(EXIT_FAILURE);
        } else if (nstacks < 1) {
            cerr << "Error: number of stacks must be > 0" << endl;
            exit(EXIT_FAILURE);
        } else if (array == NULL) {
            cerr << "Error: array is NULL" << endl;
            exit(EXIT_FAILURE);
        }

        setNStacks(nstacks);
        setStacks(new IndexStack*[size]);
        for (unsigned i = 0; i < nstacks; i++) {
            setStack(i, NULL);
        }

        setArray(array);

        setHead(new Node<unsigned>(0));
        Node<unsigned> *tail = getHead();
        for (unsigned i = 1; i < size; i++) {
            Node<unsigned> *n = new Node<unsigned>(i);
            tail->insert(n);

            tail = tail->getNext();
        }
    }

    ~StackSet() {
        Node<unsigned>::freeList(getHead());
        delete [] getStacks();
    }

    unsigned getNStacks() {
        return this->nstacks;
    }

    void push(unsigned stack, T value) {
        if (stack >= getNStacks()) {
            cerr << "Error: index out of bounds" << endl;
            exit(EXIT_FAILURE);
        } else if (getHead() == NULL) {
            cerr << "Error: stack overflow" << endl;
            exit(EXIT_FAILURE);
        }

        unsigned index = getHead()->getData();
        setHead(getHead()->getNext());
        Node<unsigned>::remove(getHead()->getPrev());

        setValue(index, value);
        if (getStack(stack) == NULL) {
            setStack(stack, new IndexStack(index));
        } else {
            getStack(stack)->push(index);
        }
    }

    void pop(unsigned stack) {
        if (stack >= getNStacks()) {
            cerr << "Error: index out of bounds" << endl;
            exit(EXIT_FAILURE);
        } else if (getStack(stack) == NULL) {
            cerr << "Error: stack underflow" << endl;
            exit(EXIT_FAILURE);
        }

        Node<unsigned> *n = new Node<unsigned>(getStack(stack)->top());
        getStack(stack)->pop();

        if (getHead() != NULL) {
            getHead()->setPrev(n);
            n->setNext(getHead());
        }
        setHead(n);
    }

    T top(unsigned stack) {
        if (stack >= getNStacks()) {
            cerr << "Error: index out of bounds" << endl;
            exit(EXIT_FAILURE);
        } else if (getStack(stack) == NULL) {
            cerr << "Error: stack underflow" << endl;
            exit(EXIT_FAILURE);
        }

        return getValue(getStack(stack)->top());
    }

    void print(unsigned stack) {
        if (stack >= getNStacks()) {
            cerr << "Error: index out of bounds" << endl;
            exit(EXIT_FAILURE);
        } else if (getStack(stack) == NULL) {
            cerr << "Error: stack underflow" << endl;
            exit(EXIT_FAILURE);
        }

        for (Node<unsigned> *n = getStack(stack)->getUsed(); n != NULL;
             n = n->getNext()) {
            cout << getValue(n->getData()) << " ";
        }
        cout << endl;
    }
};

int main() {
    int *array = new int[9];
    StackSet<int> *stack_set = new StackSet<int>(3, array, 9);

    for (unsigned i = 0; i < 3; i++) {
        stack_set->push(i, i);
        stack_set->push(i, i + 1);
        stack_set->push(i, i + 2);
        stack_set->pop(i);
        stack_set->push(i, i + 3);

        stack_set->print(i);
    }

    return 0;
}