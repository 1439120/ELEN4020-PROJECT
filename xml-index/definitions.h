#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h> 
#include <ctype.h>
#include <algorithm>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdint>
#include <vector>
#include "pthread.h"

//defines the index
struct index{
    uint64_t start_;
    uint64_t end_;
    char* date_;
};

//defines the key
struct key
{
    char* dname_;
    bool operator==(key const& other) const {
        return dname_ == other.dname_;
    }
};

// key values pair
class map{
    key key_;
    std::vector <char* > value_;

    public:
    map(key k, char* v):key_(k){
        value_.push_back(v);
    }

    void add(char* value){
        value_.push_back(value); 
    }

    const std::vector <char* > value(){
        return value_;
    }
};

//defines the days
struct dataset{
    index day_;
    std::vector <map > pair_;
};

#endif