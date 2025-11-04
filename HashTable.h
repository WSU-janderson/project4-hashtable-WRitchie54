#include <string>
#include <vector>
#include <optional>
#include <ostream>
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
        size_t& getValueRef();
        size_t getValue() const;
};


class HashTable {
    private:
        std::vector<HashTableBucket> tableData;
        std::vector<size_t> probeOffsets;
        size_t numCapacity;
        size_t numSize;

    public:
        explicit HashTable(size_t initCapacity = 8);
        bool insert(std::string key, size_t value);
        bool remove(std::string key);
        bool contains(const std::string& key) const;
        std::optional<int> get(const std::string& key) const;
        size_t capacity() const;
        size_t& operator[](const std::string& key);
        std::vector<std::string> keys() const;
        double alpha() const;
        size_t size() const;
        size_t hash(std::string key) const;
        std::optional<int> getIndex(const std::string& key) const;
        std::vector<size_t> setUpProbeOffsets(bool init);

    /**
     *
     * << operator: this operator prints to stream the filled buckets of the hashTable
     *      Format of each bucket is: Bucket (index): <key, value>
     *
     *      paramaters:
     *          os: refrence to output stream used
     *          hashTable: reference to the hashTable that shall be used
     *
    */
    friend std::ostream& operator<<(std::ostream& os, const HashTable& hashTable) {
        //Get list of filled buckets
        std::vector<std::string> curKeyList = hashTable.keys();

        //For each filled bucket get its key, index and value
        for (size_t i = 0; i < curKeyList.size(); i++) {
            std::string curKey = curKeyList[i];
            size_t curIndex = hashTable.getIndex(curKey).value();
            size_t curValue = hashTable.get(curKey).value();

            //Output values to stream
            os << "Bucket " << curIndex << ": <" << curKey << ", " << curValue << ">" << std::endl;
        }

        return os;
    }
};
