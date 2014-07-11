#ifndef __HASHTABLE_HH__
#define __HASHTABLE_HH__

#include <cstdlib>

#include <iostream>
#include <vector>

using namespace std;

template <class K, class V> class HashTable {
private:
    class HashEntry {
    private:
        K key;
        V value;

    public:
        HashEntry(K k, V v) {
            this->setKey(k);
            this->setValue(v);
        }

        void setKey(K key) {
            this->key = key;
        }

        void setValue(V value) {
            this->value = value;
        }

        K getKey() {
            return this->key;
        }

        V getValue() {
            return this->value;
        }
    };

    unsigned size;
    unsigned empty;
    vector<HashEntry *> *table;

    void setSize(unsigned size) {
        this->size = size;
    }

    void setEmpty(unsigned empty) {
        this->empty = empty;
    }

    void setTable(vector<HashEntry *> *table) {
        this->table = table;
    }

    vector<HashEntry *> *getTable() {
        return this->table;
    }

    unsigned hashKey(K key) {
        return ((size_t) key) % this->getSize();
    }

    unsigned getIndex(K key) {
        unsigned hash = this->hashKey(key);
        HashEntry *entry = this->getTable()->at(hash);

        while (entry != NULL && entry->getKey() != key) {
            hash = (hash + 1) % this->getSize();
        }

        return hash;
    }

    void resizeTable() {
        unsigned size = this->getSize();
        unsigned empty = this->getEmpty();

        if (empty * 3 < size) {
            this->getTable()->resize(size * 2, NULL);
            this->setSize(size * 2);
            this->setEmpty(empty + size);
        }
    }

public:
    HashTable(unsigned size) {
        if (size == 0) {
            cerr << "Error: size must be > 0" << endl;
            exit(EXIT_FAILURE);
        }

        this->setSize(size);
        this->setEmpty(size);
        this->setTable(new vector<HashEntry *>(size, NULL));
    }

    unsigned getSize() {
        return this->size;
    }

    unsigned getEmpty() {
        return this->empty;
    }

    void insert(K key, V value) {
        unsigned hash = this->getIndex(key);
        HashEntry *entry = this->getTable()->at(hash);

        if (entry != NULL) {
            entry->setValue(value);
        } else {
            HashEntry *new_entry = new HashEntry(key, value);
            
            (*this->getTable())[hash] = new_entry;
            this->setEmpty(this->getEmpty() - 1);
            this->resizeTable();
        }
    }

    void remove(K key) {
        unsigned hash = this->getIndex(key);
        HashEntry *entry = this->getTable()->at(hash);

        if (entry != NULL) {
            delete entry;
            (*this->getTable())[hash] = NULL;
        }
    }

    bool hasKey(K key) {
        unsigned hash = this->getIndex(key);
        HashEntry *entry = this->getTable()->at(hash);

        return entry != NULL;
    }

    V getValue(K key) {
        unsigned hash = this->getIndex(key);
        HashEntry *entry = this->getTable()->at(hash);

        if (entry == NULL) {
            cerr << "Error: key not found" << endl;
            exit(EXIT_FAILURE);
        } else {
            return entry->getValue();
        }
    }

    void clear() {
        vector<HashEntry *> *table = this->getTable();

        for (unsigned i = 0; i < this->getSize(); i++) {
            HashEntry *entry = table->at(i);

            if (entry != NULL) {
                delete entry;
                (*table)[i] = NULL;
            }
        }
    }

    void print() {
        vector<HashEntry *> *table = this->getTable();

        for (unsigned i = 0; i < this->getSize(); i++) {
            HashEntry *entry = table->at(i);

            if (entry != NULL) {
                cout << entry->getKey() << ": " << entry->getValue() << endl;
            }
        }
    }
};

#endif