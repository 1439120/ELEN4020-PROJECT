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
        }

        void DebatesSpeakerAppears(char* dname); // we need to search through every debate
        // the arguement should be a list of speakers
        // can be one arguement and we will keep on accepting speaker names
        std::vector < Index> SpeakerDebate(); // for finding debates where speaker appears
        std::vector < char *> DebatesSpeakerFound(const char* sname); // for finding speakers in ccommon debates

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

bool isDFound(std::vector <Index> debates, Index dname){
    
    if(debates.size() == 0)return false;

    for (auto& x: debates) {
        if(x == dname)return true;
            //printf("%15s ", x.second );
    }

    return false;
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

        if(isDFound(HashMap[_key % debate_.size()], debate_.at(i))) continue;

        HashMap[_key % debate_.size()].push_back(debate_.at(i)); // put the debate name in map according to the key

        std::map<char*, char*>::iterator it = debate_.at(i).speakers_.begin();
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

void remove(std::vector<char*> &v)
{
	auto end = v.end();
	for (auto it = v.begin(); it != end; ++it) {
		end = std::remove(it + 1, end, *it);
	}

	v.erase(end, v.end());
}

std::vector < char *> HashTable::DebatesSpeakerFound(const char* sname){
    size_t n = 0;
    while(sname[n] != '\0') n++;
    int key = convert(sname, n);
    std::vector < char* > temp;

    for(uint64_t i = 0; i < HashMapS[key % debate_.size()].size(); i++){

        for (auto& x: HashMapS[key % debate_.size()].at(i).speakers_) {

            if(strcmp(sname, x.second) == 0){
                    temp.push_back(HashMapS[key % debate_.size()].at(i).dheading_);
                    break;
            }
            
        }
    }

    if(temp.empty()){
        printf("no matching debate found\n");
        return temp;
    }

    for(int i = 0; i < temp.size(); i++){
        for(int k = 0; k < temp.size(); k++){
            if(i == k) continue;
            else{

            }
        }
    }

    //remove(temp);
  
    return temp;
}

#endif