#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "indexing_xml.h"

class HashTable: public Debates{
    public:
        HashTable(const char* fname);
        ~HashTable();
        bool HashGivenData();

};

// use the debates constructor
HashTable::HashTable(const char* fname): Debates(fname){}

// Deallocate the memory for the names
HashTable::~HashTable(){

}

// Before we search anything on the hash table, we need to hash the data on indexing_xml
bool HashTable::HashGivenData(){
    if(debate_.empty())return false;
   

    return true;
}

#endif