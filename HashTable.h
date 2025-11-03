#include <string>
#include <vector>
#include <optional>
/**
 * HashTable.h
 */
enum class BucketType {NORMAL, ESS, EAR};

class HashTableBucket{
    private:
    mutable BucketType type;
        std::string key;
        size_t value;

    public:
        HashTableBucket();
        HashTableBucket(std::string key, int value);
        void load(std::string key, int value);
        bool isEmpty() const;
        bool isEmptySinceStart() const;
        void setBucketType(BucketType type) const;
        std::string getKey() const;
        // size_t getValueRef();
        size_t getValue() const;
        friend std::ostream& operator<<(std::ostream& os, const HashTableBucket& bucket) {
            return os;
        };
};


class HashTable {
    private:
        std::vector<HashTableBucket> tableData;
        std::vector<size_t> probeOffsets;

    public:
        explicit HashTable(size_t initCapacity = 8);
        bool insert(std::string key, size_t value);
        bool remove(std::string key);
        bool contains(const std::string& key) const;
        std::optional<int> get(const std::string& key) const;
        size_t capacity() const;
        // int& operator[](const std::string& key);
        std::vector<std::string> keys() const;
        double alpha() const;
        size_t size() const;
        size_t hash(std::string key) const;
        std::optional<int> getIndex(const std::string& key) const;
        void setUpProbeOffsets(size_t startNumber);

        /**
* operator<< is another example of operator overloading in C++, similar to
* operator[]. The friend keyword only needs to appear in the class declaration,
* but not the definition. In addition, operator<< is not a method of HashTable,
* so do not put HashTable:: before it when defining it. operator<< will allow us
* to print the contents of our hash table using the normal syntax:
cout <<
* myHashTable << endl;
You should only print the buckets which are occupied,
* and along with each item you will print which bucket (the index of the bucket)
* the item is in. To make it easy, I suggest creating a helper method called
* something like printMe() that returns a string of everything in the table. An
* example which uses open addressing for collision resolution could print
* something like:
Bucket 5: <James, 4815>
Bucket 2: <Juliet, 1623>
Bucket
* 11: <Hugo, 42108>
*/
    friend std::ostream& operator<<(std::ostream& os, const HashTable& hashTable) {
        return os;
    }
};
