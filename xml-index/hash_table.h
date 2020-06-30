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
            size_t n = 0;
	        while(dname[n] != '\0')n++;
            int _key = convert(dname, n);

            if(HashMap[_key % debate_.size()].empty()){
                printf("no matching debate found");
                return;
            }
            for(uint64_t c = 0; c < HashMap[_key % debate_.size()].size(); c++){
                printf(" %15s \n",HashMap[_key % debate_.size()].at(c).dheading_);
                printf("-------------------------------------------------------");
                for (auto& x: HashMap[_key % debate_.size()].at(c).speakers_) {
                    //std::cout << x.first << ": " << << '\n';
                    printf("%15s", x.second );
                }
                printf("-------------------------------------------------------");
                printf("\n");
            }      
        }

        void DebatesSpeakerAppears(char* dname); // we need to search through every debate
        // the arguement should be a list of speakers
        // can be one arguement and we will keep on accepting speaker names
        std::vector < Index> SpeakerDebate(); // for finding debates where speaker appears
        std::vector < Index> DebatesSpeakerFound(char* sname); // for finding speakers in ccommon debates

    private:
        uint64_t row_size;
        std::vector < std::vector< Index>> HashMap;
        std::vector < std::vector< Index>> HashMapS; // try another hashmap for speakers
        bool HashGivenData();
};

// use the debates constructor
// and also hash the given data
HashTable::HashTable(const char* fname): Debates(fname){
    HashMap.resize(debate_.size());
    HashMapS.resize(debate_.size());
    HashGivenData();
}

// Deallocate the memory for the names
HashTable::~HashTable(){

}

// Before we search anything on the hash table, we need to hash the data on indexing_xml
bool HashTable::HashGivenData(){
    if(debate_.empty())return false;
    
    // Hash all the debates and speakers in parallel
    #pragma omp parallel for collapse(2)
    for(uint64_t i = 0; i < debate_.size(); i++) {
        // convert the given debate name to a key
        size_t n1 = 0;
	    while(debate_.at(i).dheading_[n1] != '\0') n1++;
        int _key = convert(debate_.at(i).dheading_, n1);
        HashMap[_key % debate_.size()].push_back(debate_.at(i)); // put the debate name in map according to the key

        std::map<int, char*>::iterator it = debate_.at(i).speakers_.begin();
        for (; it != debate_.at(i).speakers_.end(); ++it){
            size_t n = 0;
	        while(it->second[n] != '\0')n++;
            int key = convert(it->second, n);
            HashMapS[key % debate_.size()].push_back(debate_.at(i));
        }
    }

    return true;
}

void HashTable::DebatesSpeakerAppears(char* dname){
    //done with this
    // I will hash the name and find a vector
    // the vector should be read in parallel using MPI// vector of debates which each have a bmap of speakers
    
}

std::vector < Index> HashTable::DebatesSpeakerFound(char* sname){
    size_t n = 0;
    // printf("before while loop\n");
    while(sname[n] != '\0') n++;
    printf("sixe pf the input is %d\n", n);
    int key = convert(sname, n);
    std::vector < Index> temp;

    printf("size of the found speakers %d %d\n", temp.size(), HashMapS[key % debate_.size()].size());

    for(uint64_t i = 0; i < HashMapS[key % debate_.size()].size(); i++){
        // for(int j = 0; j < HashMapS[key].at(i).speakers_.size(); j++){
            
        //     if(HashMapS[key].at(i).speakers_.at(j) == sname){
        //         temp.push_back(HashMapS[key].at(i));
        //         break;
        //     }
        // }
        std::vector < Index> temp;
        std::map<int, char*>::iterator it = HashMapS[key % debate_.size()].at(i).speakers_.find(key);
        if(it == HashMapS[key % debate_.size()].at(i).speakers_.end()){ 
            printf("Name not found\n");
        }else {
            printf("Found it\n");
            if(it->second != sname) HashMapS[key % debate_.size()].at(i).speakers_.erase(it);
            Index krk = HashMapS[key % debate_.size()].at(i);
            temp.push_back(krk);
        }
    }

    printf("size of the found speakers %d %d\n", temp.size(), HashMapS[key % debate_.size()].size());

    // if(temp.empty()){
    //     printf("no matching debate found\n");
    //     return temp;
    // }

    for(uint64_t c = 0; c < HashMapS[key % debate_.size()].size(); c++){
        printf(" %15s \n",HashMapS[key % debate_.size()].at(c).dheading_);
        printf("-------------------------------------------------------");
        for (auto& x: HashMapS[key % debate_.size()].at(c).speakers_) {
            //std::cout << x.first << ": " << << '\n';
            if(x.second != sname)continue;
            printf("%15s ", x.second );
        }
        printf("-------------------------------------------------------");
        printf("\n");
    }      
    return HashMapS[key % debate_.size()];
}

// std::vector<Index> operator+(const std::vector<Index>& v1, const std::vector<Index>& v2)
// {
//     if(v1.empty()) return v2;
//     else if(v2.empty()) return v1;  
//     v1.insert(v1.end(), v2.begin(), v2.end());
//     std::vector<Index> res (v1.begin(), v2.end());
//     return res;
// }
#endif