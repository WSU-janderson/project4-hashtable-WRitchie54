/**
 * HashTable.cpp
 */

#include "HashTable.h"

#include <algorithm>
#include <optional>
#include <string>

/**
* The default constructor can simply set the bucket type to ESS.
*
*/
HashTableBucket::HashTableBucket() {
    this->setBucketType(BucketType::ESS);
};
/**
* A parameterized constructor could initialize the key and value, as
* well as set the bucket type to NORMAL.
*/
HashTableBucket::HashTableBucket(std::string key, int value) {
    this->load(key, value);
};
/**
* A load method could load the key-value pair into the bucket, which
* should then also mark the bucket as NORMAL.
*/
void HashTableBucket::load(std::string key, int value) {
    this->setBucketType(BucketType::NORMAL);
    this->key = key;
    this->value= value;
};
/**
* This method would return whether the bucket is empty, regardless of
* if it has had data placed in it or not.
*/
bool HashTableBucket::isEmpty() const {
    if ((this->type == BucketType::ESS) or (this->type == BucketType::EAR)){
        return true;
    }
    else {
        return false;
    }
}

bool HashTableBucket::isEmptySinceStart() const {
    if (this->type == BucketType::ESS){
        return true;
    }
    else {
        return false;
    }
}

/**
* This method would set bucket type.
*/
void HashTableBucket::setBucketType(BucketType type) const{
    this->type = type;
}

/*
 *Gets key value from bucket
 */
std::string HashTableBucket::getKey() const{
    return this->key;
}

// /*
//  *Gets value from bucket
//  */
// size_t &HashTableBucket::getValueRef(){
//     return this->value;
// }

/*
 *Gets value from bucket
 */
size_t HashTableBucket::getValue() const{
    return this->value;
}


/**
* Only a single constructor that takes an initial capacity for the table is
* necessary. If no capacity is given, it defaults to 8 initially
*/
HashTable::HashTable(size_t initCapacity) {
    this->numCapacity = initCapacity;
    this->numSize = 0;
    tableData.resize(initCapacity);
    probeOffsets = this->setUpProbeOffsets(true);
    // srand(time(nullptr));
}
/**
* Insert a new key-value pair into the table. Duplicate keys are NOT allowed. The
* method should return true if the insertion was successful. If the insertion was
* unsucessful, such as when a duplicate is attempted to be inserted, the method
* should return false
*/
bool HashTable::insert(std::string key, size_t value) {
    bool result = false;
    if (this->contains(key)) {
        return result;
    }

    size_t home = hash(key); //currently using a multiplcative string hash function similar to described in zybooks
    //Probe for proper location to insert key value pair
    for (size_t i = 0; i < this->capacity(); i++) {
        size_t vectorIndex = (home + probeOffsets[i]) % this->capacity();

        if (tableData.at(vectorIndex).getKey() == key) {
            result = false;
            break;
        }

        //If empty load data into bucket and return out of fuction
        if (tableData.at(vectorIndex).isEmpty()) {
            tableData.at(vectorIndex).load(key, value);
            this->numSize++;
            result = true;
            break;
        }
    }

    //Resize vector if load rating is greater than 0.5 NEED TO REMAP INTS STILL
    if (this->alpha() > 0.5) {
        //Make new vector table and insert current keys into it with new locations based off resize.
        //Set tableData to this new vector at end so that it has the new locations
        std::vector<size_t> newProbeOffsets = this->setUpProbeOffsets(false);
        std::vector<HashTableBucket> newDataTable;
        newDataTable.resize(this->capacity() * 2);
        std::vector<std::string> curKeyList = this->keys();

        for (int i = 0; i < curKeyList.capacity(); i++) {
            size_t curKeyIndex = this->getIndex(curKeyList[i]).value();
            std::string curKey = this->tableData.at(curKeyIndex).getKey();
            int curValue = this->tableData.at(curKeyIndex).getValue();

            size_t home = hash(curKey);
            //Probe for proper location to insert key value pair
            for (size_t i = 0; i < newDataTable.capacity(); i++) {
                size_t vectorIndex = (home + newProbeOffsets[i]) % newDataTable.capacity();

                //If empty load data into bucket and return out of fuction
                if (newDataTable.at(vectorIndex).isEmpty()) {
                    newDataTable.at(vectorIndex).load(curKey, curValue);
                    break;
                }
            }
        }

        this->tableData.clear();
        this->tableData = newDataTable;
        this->probeOffsets.clear();
        this->probeOffsets = newProbeOffsets;
        this->numCapacity *= 2;
    }
    return result;
}
/**
* If the key is in the table, remove will “erase” the key-value pair from the
* table. This might just be marking a bucket as empty-after-remove
*/
bool HashTable::remove(std::string key) {
    if (std::optional<int> curKey = this->getIndex(key); curKey != std::nullopt) {
        this->tableData.at(curKey.value()).setBucketType(BucketType::EAR);
        return true;
    }
    else {
        return false;
    }
}
/**
* contains returns true if the key is in the table and false if the key is not in
* the table.
*/
bool HashTable::contains(const std::string& key) const {
    if (this->getIndex(key) != std::nullopt) {
        return true;
    }
    return false;
}
/**
* If the key is found in the table, find will return the value associated with
* that key. If the key is not in the table, find will return something called
* nullopt, which is a special value in C++. The find method returns an
* optional<int>, which is a way to denote a method might not have a valid value
* to return. This approach is nicer than designating a special value, like -1, to
* signify the return value is invalid. It's also much better than throwing an
* exception if the key is not found.
*/
std::optional<int> HashTable::get(const std::string& key) const {
    if (std::optional<int> curKey = this->getIndex(key); curKey != std::nullopt) {
        return this->tableData.at(curKey.value()).getValue();
    }
    else {
        return std::nullopt;
    }
}
/**
* The bracket operator lets us access values in the map using a familiar syntax,
* similar to C++ std::map or Python dictionaries. It behaves like get, returnin
* the value associated with a given key:
int idNum = hashTable[“James”];
* Unlike get, however, the bracker operator returns a reference to the value,
* which allows assignment:
hashTable[“James”] = 1234;
If the key is not
* in the table, returning a valid reference is impossible. You may choose to
* throw an exception in this case, but for our implementation, the situation
* results in undefined behavior. Simply put, you do not need to address attempts
* to access keys not in the table inside the bracket operator method.
*/
// int& HashTable::operator[](const std::string& key) {
//     size_t home = hash(key); //
//     //Probe for proper location to remove key value pair
//     for (size_t i = 0; i < this->capacity()-1; i++) {
//         size_t vectorIndex = (home + probeOffsets[i]) % this->capacity();
//
//         if (this->tableData.at(vectorIndex).getKey() == key) {
//             int& ref = this->tableData.at(vectorIndex).getValue();
//             return ref;
//         }
//     }
//
// }

/**
* keys returns a std::vector (C++ version of ArrayList, or simply list/array)
* with all of the keys currently in the table. The length of the vector should be
* the same as the size of the hash table.
*/
std::vector<std::string> HashTable::keys() const {
    std::vector<std::string> curKeyList;
    curKeyList.resize(this->size());
    int curKeyIndex = 0;
    for (size_t i = 0; i < this->capacity(); i++) {
        if (!this->tableData.at(i).isEmpty()) {
            curKeyList[curKeyIndex] = this->tableData.at(i).getKey();
            curKeyIndex++;
        }
    }
    return curKeyList;
}
/**
* alpha returns the current load factor of the table, or size/capacity. Since
* alpha returns a double,make sure to properly cast the size and capacity, which
* are size_t, to avoid integer division. You can cast a size_t num to a double in
* C++ like:
static_cast<double>(num)
The time complexity for
* this method must be O(1).
*/
double HashTable::alpha() const {
    return static_cast<double>(this->size())/static_cast<double>(this->capacity());
}

/**
* capacity returns how many buckets in total are in the hash table. The time
* complexity for this algorithm must be O(1).
*/
size_t HashTable::capacity() const {
    return this->numCapacity;
}
/**
* The size method returns how many key-value pairs are in the hash table. The
* time complexity for this method must be O(1)
*/
size_t HashTable::size() const {
    return this->numSize;
}

//Multiplicative hash function found idea from the zybooks
size_t HashTable::hash(std::string key) const {

    size_t hashedValue = 0;

    for(char character : key) {
        hashedValue = (hashedValue * 33) +  character;
    }

    return hashedValue % this->capacity();
}

//get index returns the index of where a key should've been place
std::optional<int> HashTable::getIndex(const std::string& key) const {
    size_t home = hash(key);
    //Probe for proper location to remove key value pair
    for (size_t i = 0; i < this->capacity(); i++) {
        size_t vectorIndex = (home + probeOffsets[i]) % this->capacity();

        if (this->tableData.at(vectorIndex).getKey() == key) {
            return vectorIndex;
        }

        if (this->tableData[vectorIndex].isEmptySinceStart()) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

std::vector<size_t> HashTable::setUpProbeOffsets(bool init) {
    std::vector<size_t> newProbeOffsets;
    size_t newCapacity = 0;
    if (init) {
        newCapacity = this->capacity();
    }
    else {
        newCapacity = this->capacity() * 2;
    }

    newProbeOffsets.resize(newCapacity, -1);
    for (size_t i = 0; i < newCapacity;  i++) {
        while (newProbeOffsets.at(i) == -1) {
            size_t num = rand() % size_t(newCapacity);
            bool found = false;

            //search vector to see if current rand has been used
            for (size_t j = 0;  j < i; j++) {
                if (num == newProbeOffsets[j]) {
                    found = true;
                    break;
                }
            }

            //if not already in use assign random number
            if (!found) {
                newProbeOffsets[i] = num;
            }
        }
    }
    return newProbeOffsets;
}