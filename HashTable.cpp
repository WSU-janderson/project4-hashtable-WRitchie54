/**
 * HashTable.cpp
 */

#include "HashTable.h"

#include <algorithm>
#include <optional>
#include <string>

/**
*  HashTableBucket default constructor: Sets the bucket created to empty since start
*/
HashTableBucket::HashTableBucket() {
    this->setBucketType(BucketType::ESS);
};
/**
* HashTableBucket parameterized constructor: Initialize the key and value
* and set bucketType to normal via load method
*
* params:
*   key: the key you want to insert into the bucket
*   value: the value that should be in the bucket
*/
HashTableBucket::HashTableBucket(std::string key, int value) {
    this->load(key, value);
};

/**
* Load: load the given key and value into the bucket's fields
*
* params:
*   key: the key you want to have in the bucket
*   value: the value that should be in the bucket
*/
void HashTableBucket::load(std::string key, int value) {
    this->setBucketType(BucketType::NORMAL);
    this->key = key;
    this->value= value;
};

/**
* isEmpty: checks the bucket type to see if it is empty or not
*
* return :
*   bool is the bucket not of type empty since start or empty after remove
*/
bool HashTableBucket::isEmpty() const {
    if ((this->type == BucketType::ESS) or (this->type == BucketType::EAR)){
        return true;
    }
    else {
        return false;
    }
}

/**
* isEmptySinceStart: checks the bucket type to see if it is emptySinceStart
*
* return :
*   bool is the bucket not of type empty since start
*/
bool HashTableBucket::isEmptySinceStart() const {
    if (this->type == BucketType::ESS){
        return true;
    }
    else {
        return false;
    }
}

/**
* setBucketType: sets the bucket type
*
* param :
*   BucketType: is the bucket type you are going to set the bucket to
*/
void HashTableBucket::setBucketType(BucketType type) const{
    this->type = type;
}

/**
* getKey: gets value of the key field
*
* return :
*   std:string: is the buckets key field value
*/
std::string HashTableBucket::getKey() const{
    return this->key;
}

/**
* getValueRef: gets reference to the value field
*
* return :
*   size_t&: is the buckets value field reference
*/
size_t& HashTableBucket::getValueRef(){
    return this->value;
}

/**
* getValue: gets value of the value field
*
* return :
*   size_t: is the buckets value field value
*/
size_t HashTableBucket::getValue() const{
    return this->value;
}

/**
* HashTable constructor: Takes a capacity and initializes the size, capacity values. Also initalizes the
*   probeOffsets and tableData vectors.
*
* param :
*   initCapacity: defaults to 8 but is the base HashTable capacity otherwise
*/
HashTable::HashTable(size_t initCapacity) {
    this->numCapacity = initCapacity;
    this->numSize = 0;
    tableData.resize(initCapacity);
    probeOffsets = this->setUpProbeOffsets(true);
}

/**
* insert: Inserts a new key-value pair into the hashTable. If the key is already in the table
*   than it is not inserted. If the alpha of the table (size/capacity) exceeds .5 the table
*   is resized to double its current size and the elements are reinput under a new probe
*   offset vector.
*
* param :
*   key: the key to input into the table
*   value: the value associated with the key
*/
bool HashTable::insert(std::string key, size_t value) {
    bool result = false;

    //return false
    if (this->contains(key)) {
        return result;
    }

    //Hash current key to find home position of this key
    size_t home = hash(key);
    //Probe for proper location to insert key value pair
    for (size_t i = 0; i < this->capacity(); i++) {
        size_t vectorIndex = (home + probeOffsets[i]) % this->capacity();

        //If the current buckey is = to the current key and is not marked as break return out of loop
        if (tableData.at(vectorIndex).getKey() == key && !tableData.at(vectorIndex).isEmpty()) {
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

        //Go through current list and find where the key is and find a new location for each key in list
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

        //Swap out old tables for new ones
        this->tableData.clear();
        this->tableData = newDataTable;
        this->probeOffsets.clear();
        this->probeOffsets = newProbeOffsets;
        this->numCapacity *= 2;
    }
    return result;
}

/**
* remove: Check if key is in table if it is set the bucket it was in to empty after removal
*
* param :
*   key: the key to check for in the table
*/
bool HashTable::remove(std::string key) {
    //Check if current key is in list
    if (std::optional<int> curKey = this->getIndex(key); curKey != std::nullopt) {
        //Set bucket type to empty after removal
        this->tableData.at(curKey.value()).setBucketType(BucketType::EAR);
        //Lower current size
        numSize--;
        return true;
    }
    else {
        return false;
    }
}

/**
* contains: Check if key is in table
*
* param :
*   key: reference to the key to check for in the table
*
* returns:
*   bool: true if it is in container false if it is not
*/
bool HashTable::contains(const std::string& key) const {
    //Find index of current key if it is not nullopt the bucket is in the lust
    if (this->getIndex(key) != std::nullopt) {
        return true;
    }
    return false;
}

/**
* get: Check if key is in table and return the value
*
* param :
*   key: reference to the key to check for in the table
*
* returns:
*   std::optional<int>: Value of key if it is in the table or nullopt if key is not in table
*/
std::optional<int> HashTable::get(const std::string& key) const {
    //Grab keys current index and make sure it is not nullopt
    if (std::optional<int> curKey = this->getIndex(key); curKey != std::nullopt) {
        //Return keys value
        return this->tableData.at(curKey.value()).getValue();
    }
    else {
        return std::nullopt;
    }
}

/**
* operator []: Check if key is in table and return the reference to the value for assignment purpouses
*
* param :
*   key: reference to the key to check for in the table
*
* returns:
*   size_t&: Reference to value of key if it is in the table or nothing if it is not in the table
*/
size_t& HashTable::operator[](const std::string& key) {
    //Check if key is in list
    if (std::optional<int> curKey = this->getIndex(key); curKey != std::nullopt) {
        //If key is in list return the reference to the current value for assignment or getter
        size_t& ref = this->tableData.at(curKey.value()).getValueRef();
        return ref;
    }
}

/**
* keys: Get list of all buckets that are normal in table
*
* returns:
*   std::vector<std::string>: List of all non empty buckets keys
*/
std::vector<std::string> HashTable::keys() const {
    std::vector<std::string> curKeyList;
    curKeyList.resize(this->size());
    int curKeyIndex = 0;
    //Search length of vector for if a buckey is empty or not
    for (size_t i = 0; i < this->capacity(); i++) {
        if (!this->tableData.at(i).isEmpty()) {
            //If not empty add to list and up the index
            curKeyList[curKeyIndex] = this->tableData.at(i).getKey();
            curKeyIndex++;
        }
    }
    return curKeyList;
}

/**
* alpha: get the load factor of the vector table comprised of size/capacity
*
* returns:
*   double: size/capacity
*/
double HashTable::alpha() const {
    return static_cast<double>(this->size())/static_cast<double>(this->capacity());
}

/**
* capacity: Get capacity of bucket (total number of buckets)
*
* returns:
*   size_t: Number of total buckets
*/
size_t HashTable::capacity() const {
    return this->numCapacity;
}

/**
* size: Returns number of buckets that are not empty
*
* returns:
*   size_t: List of all non empty buckets keys
*/
size_t HashTable::size() const {
    return this->numSize;
}

/**
* hash: Use internal hash function to convert string into hashed value
*
* returns:
*   size_t: Hashed value of key
*/
size_t HashTable::hash(std::string key) const {
    return std::hash<std::string>{}(key);
}

/**
* getIndex: get index returns the index of where a key should be placed
*
* returns:
*   std::optional<int>: Possible index of key
*/
std::optional<int> HashTable::getIndex(const std::string& key) const {
    size_t home = hash(key);
    //Probe for proper location to remove key value pair
    for (size_t i = 0; i < this->capacity(); i++) {
        size_t vectorIndex = (home + probeOffsets[i]) % this->capacity();

        //If key matches and bucket is not empty then return index
        if ((this->tableData.at(vectorIndex).getKey() == key) and (!this->tableData.at(vectorIndex).isEmpty())) {
            return vectorIndex;
        }

        //If you found an empty bucket return nullopt the key is not in the vector table
        if (this->tableData[vectorIndex].isEmptySinceStart()) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

/**
* setupProbeOffsets: create a random list of probe offsets to use when inserting into list
*
* returns:
*   std::vector<size_t>: List of random numbers to use when probing
*/
std::vector<size_t> HashTable::setUpProbeOffsets(bool init) {
    std::vector<size_t> newProbeOffsets;
    size_t newCapacity = 0;

    //If first time don't multiply capacity by 2
    if (init) {
        newCapacity = this->capacity() - 1;
    }
    else {
        newCapacity = (this->capacity() * 2) - 1;
    }

    //Resize offsets to be new capacity
    newProbeOffsets.resize(newCapacity, -1);

    //Go through each index of list and insert random number until all numbers in range are found
    for (size_t i = 0; i < newCapacity;  i++) {
        while (newProbeOffsets.at(i) == -1) {
            size_t num = rand() % size_t(newCapacity) + 1;
            bool found = false;

            //search vector to see if current random number has been used
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