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

        void DisplayingAllDebates(){
            for(uint64_t c = 0; c < debate_.size(); c++){
                printf("-------------------------------------------------------\n");
                printf("%15s - %15s\n",debate_.at(c).dname_, debate_.at(c).dheading_);

                std::map<char*, char*>::iterator it = debate_.at(c).speakers_.begin();
                for (; it != debate_.at(c).speakers_.end(); ++it){
                    printf("*** %15s ***", it->second);
                }
                printf("-------------------------------------------------------");
                printf("\n");
            }  
        }

        std::vector < char*> DataInFile(FILE* fptr, char* qfile){
            
            std::vector < char*> dname;
            if ((fptr = fopen(qfile, "r")) == NULL) {
            printf("Error! opening file\n");
            // Program exits if file pointer returns NULL.
            exit(1);
            }

            // do{
            // // int n = 0;
            // // while(c[n] != '\0')n++;
            // // printf("Data from the file: %s size %d\n", c, n);
            // char *c = (char *)malloc (100);
            // dname.push_back(c);
            // //printf("%15s",dname.at(dname.size() - 1));
            // }while (fgets (c, 100, fptr) != NULL);
            while (true)
            {
                char *c = (char *)malloc (100);
                if(fgets (c, 100, fptr) != NULL){
                    dname.push_back(c);
                }else{
                    break;
                }
            }
            
            return dname;
        }

        void DebatesSpeakerAppears(char* dname); // we need to search through every debate
        //the arguement should be a list of speakers
        //can be one arguement and we will keep on accepting speaker names
        std::vector < Index> SpeakerDebate(); // for finding debates where speaker appears
        std::vector < char *> DebatesSpeakerFound(const char* sname); // for finding speakers in ccommon debates
        std::vector < char *> SpeakersInDebate(const char* sname);

    private:
        uint64_t row_size;
        std::unordered_map< int, Index> list_;
        std::vector < std::vector< Index> > HashMap;
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
        int _key = convert(debate_.at(i).dname_, debate_.at(i).nlen_);
        HashMap[_key % debate_.size()].push_back(debate_.at(i));
        
    }

    return true;
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

    // for(int i = 0; i < temp.size(); i++){
    //     for(int k = 0; k < temp.size(); k++){
    //         if(i == k) continue;
    //         else{

    //         }
    //     }
    // }

    //remove(temp);
  
    return temp;
}

std::vector < char *> HashTable::SpeakersInDebate(const char* sname){
    size_t n = 0;
    while(sname[n] != '\0') n++;
    int key = convert(sname, n);
    std::vector < char* > temp;

    for(uint64_t i = 0; i < HashMap[key % debate_.size()].size(); i++){

        // if(strcmp(sname, HashMap[key % debate_.size()].at(i).dheading_) != 0){
        //     printf("%15s\n", HashMap[key % debate_.size()].at(i).dheading_);
        //     continue;
        //     }

        for (auto& x: HashMap[key % debate_.size()].at(i).speakers_) {
            //if(strcmp(sname, x.second) == 0){
            temp.push_back(x.second);
            //}   
        }
    }

     if(temp.empty()){
        printf("no correspomding speakers to that debate\n");
        return temp;
    }

    remove(temp);

    // printf("***********************\n");
    // for(int i = 0; i < temp.size(); i++) printf("%15s\n", temp.at(i));
    // printf("***********************\n");

    return temp;
}

#endif