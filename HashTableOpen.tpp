template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(int i) : M(i), ht(new LinkedList<Record>*[M]) {
    // DONE
    for (int i = 0; i < M; ++i)  ht[i] = new LinkedList<Record>();
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(const HashTableOpen<Key, Val>& copyObj)
: M(0), ht(nullptr) {
    copy(copyObj);
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>& HashTableOpen<Key, Val>::operator=
(const HashTableOpen<Key, Val>& rightObj) {
    if (this != &rightObj) {
        clear();
        copy(rightObj);
    }
    return *this;
}

// is clear actually enough? double check....no, need to del ht ya?
template <typename Key, typename Val>
HashTableOpen<Key, Val>::~HashTableOpen() {
    // DONE
    clear();
    for (int i = 0; i < M; ++i) delete ht[i];
    delete[] ht;
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::clear() {
    for (int i = 0; i < M; i++) {
        ht[i]->clear();
    }
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::copy(const HashTableOpen<Key, Val>& copyObj) {
    if (M != copyObj.M) {
        LinkedList<Record>** tmp = new LinkedList<Record>*[copyObj.M];
        if (M < copyObj.M) {
            for (int i = 0; i < M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = M; i < copyObj.M; i++) {
                tmp[i] = new LinkedList<Record>;
            }
        }
        else if (M > copyObj.M) {
            for (int i = 0; i < copyObj.M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = copyObj.M; i < M; i++) {
                delete ht[i];
            }
        }

        M = copyObj.M;
        delete[] ht;
        ht = tmp;
    }

    LinkedList<Record>* myList      = nullptr;
    LinkedList<Record>* copyList    = nullptr;
    int                 myListLen   = 0;
    int                 copyListLen = 0;

    for (int i = 0; i < M; i++) {
        myList      = ht[i];
        myListLen   = myList->getLength();
        copyList    = copyObj.ht[i];
        copyListLen = copyList->getLength();

        if (myListLen < copyListLen) {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = myListLen; j < copyListLen; j++) {
                if (myList->isEmpty()) {
                    myList->append(copyList->getElement(j));
                }
                else {
                    myList->insert(0, copyList->getElement(j));
                }
            }
        }
        else if (myListLen > copyListLen) {
            for (int j = 0; j < copyListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = copyListLen; j < myListLen; j++) {
                myList->remove(j);
            }
        }
        else {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
        }
    }
}

template <typename Key, typename Val>
int HashTableOpen<Key, Val>::hash(const Key& k) const {
    // how long should each fold be
    // changing this means you should also change the reinterpeted data type
    const int FOLD_LEN = 4;

    // if the fold length is 4, then must treat the string as unsigned numbers
    unsigned* ikey = (unsigned*) k.c_str();

    // calculate how many folds there are
    int ilen = k.length() / FOLD_LEN;

    // accumulator
    unsigned sum = 0;

    // for each fold, now treated as a number, add it to the running total
    for (int i = 0; i < ilen; i++) {
        sum += ikey[i];
    }

    // calculate how many leftover characters there are
    int extra = k.length() - ilen * FOLD_LEN;

    // create a temporary array that will hold those extra characters
    char temp[FOLD_LEN];

    // clear out that array with a 0 value
    ikey    = (unsigned*) temp;
    ikey[0] = 0;

    // copy the extra characters over
    for (int i = 0; i < extra; i++) {
        temp[i] = k[ilen * FOLD_LEN + i];
    }

    // add these final characters as a number to the running total
    sum += ikey[0];

    // calculate the slot position
    int slot = sum % M;

    // display the hashing information
    cout << k << "\thashes to slot " << slot << endl;

    // return the valid slot position
    return slot;
}

template <typename Key, typename Val>
Val HashTableOpen<Key, Val>::find(const Key& k) const {
    // TODO
    int index = hash(k);
    int findLen = ht[index]->getLength();
    for (int i = 0; i < findLen; ++i) {
        const Record& tempRec = ht[index]->getElement(i);
        if (k == tempRec.k) return tempRec.v;
    }

    throw string("Key not found in table");
}

// TODO....check for existing/dupe keys? either refuse or change v?
template <typename Key, typename Val>
void HashTableOpen<Key, Val>::insert(const Key& k, const Val& v) {
    // TODO
    int index = hash(k);
    // Record newRec = new Record(k,v);  cannot convert R* to R?
    ht[index]->append(Record(k,v));
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::remove(const Key& k) {
    // TODO? helper "find if index" function?
    int index = hash(k);
    int listLen = ht[index]->getLength();

    for (int i = 0; i < listLen; ++i) {
        const Record& tempRec = ht[index]->getElement(i);
        if (k == tempRec.k) {
            ht[index]->remove(i);
            return;
        }
    }

    throw string("Key not found in table. Cannot remove.");
}

template <typename Key, typename Val>
int HashTableOpen<Key, Val>::size() const {
    // DONE?
    int length = 0;

    for (int i = 0; i < M; ++i) {
        length += ht[i]->getLength();
    }

    return length;
}
