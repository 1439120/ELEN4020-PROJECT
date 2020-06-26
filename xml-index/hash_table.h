#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <unordered_map>
#include <utility>
#include "indexing_xml.h"

class HashTable: public Debates{
    public:
        HashTable(const char* fname);
        ~HashTable();
        void findDebateName(char *dname){
            printf("the size is %ld \n",list_.size());
            size_t n = 0;
	        while(dname[n] != '\0')n++;
            int _key = convert(dname, n);

            if(HashMap[_key % debate_.size()].empty()){
                printf("no matching debate found");
                return;
            }
            for(uint64_t c = 0; c < HashMap[_key % debate_.size()].size(); c++){
                printf(" %15s \n",HashMap[_key % debate_.size()].at(c).dheading_);
            }      
        }

    private:
        uint64_t row_size;
        std::unordered_map< int, Index> list_;
        std::vector < std::vector< Index>> HashMap;
        bool HashGivenData();
        int convert(char* dname, uint64_t len){
            int sum = -1;
            for(uint64_t i = 0; i < len; i++){
                sum += (int) dname[i];
            }
            return sum;
        }
};

// use the debates constructor
// and also hash the given data
HashTable::HashTable(const char* fname): Debates(fname){
    HashMap.resize(debate_.size());
    HashGivenData();
}

// Deallocate the memory for the names
HashTable::~HashTable(){

}

// Before we search anything on the hash table, we need to hash the data on indexing_xml
bool HashTable::HashGivenData(){
    if(debate_.empty())return false;
    
    // Hash all the debates in parallel
    #pragma omp parallel
    #pragma omp for
    for(uint64_t i = 0; i < debate_.size(); i++) {
        int _key = convert(debate_.at(i).dname_, debate_.at(i).nlen_);
        HashMap[_key % debate_.size()].push_back(debate_.at(i));
        list_.insert ({_key, debate_.at(i)});
    }

    return true;
}
#endif