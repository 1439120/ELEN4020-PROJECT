#ifndef INDEXING_XML_H
#define INDEXING_XML_H
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#include <stdio.h> 
#include <algorithm>
#include <sys/types.h>
#include <cstdint>
#include <vector>
#include "pthread.h"

#include <stdio.h>

// Holds the location of the debate and the debate name
struct Index{
    uint64_t start_;
    uint64_t end_;
    char* dname_;
    char* dheading_;
    uint64_t nlen_;
    std::vector <char* > speakers_;

    bool operator==(Index const& other) const {
        return strcmp(dname_, other.dname_) == 0;
    }

    Index& operator = (Index& other){
        start_ = other.start_;
        end_ = other.end_;
        dname_ = other.dname_;
        nlen_ = other.nlen_;
    }

};

class Debates{
    public:
        Debates(const char* fname);
        ~Debates();
        void DisplayDebatesN(){
            for(uint64_t i = 0; i < debate_.size(); i++) 
                printf("debate name = %15s && length = %ld\n", debate_.at(i).dname_,debate_.at(i).nlen_);    
        }

    protected:
        std::vector <Index> debate_;
        char* data_;
        uint64_t datasize;
        const char* DEBATETAG = "<debateSection name=\"opening\">";
        //const char* SPEAKERTAG = "<person refersTo=\"hansard\">";

    private:
        void CreateIndex(FILE* f);
        char* findname(uint64_t start, uint64_t end){
            char* dname = (char *)malloc ((end-start + 1)*sizeof(char));
            for(uint64_t i= start; i < end; i++) dname[i-start] = data_[i];
            dname[end-start] = '\0';
            return dname;
        }
        void FindHeadingSpeaker();
        const char* SPEAKERTAG = "<person refersTo=\"hansard\">";
        const char* HEADINGTAG = "<heading>";
};

Debates::~Debates(){
    free(data_);
    for(uint64_t i = 0; i < debate_.size(); i++){
        free(debate_.at(i).dname_);
        free(debate_.at(i).dheading_);
    }
}

Debates::Debates(const char* fname){
    
    struct stat st;
    uint64_t fsize = 0;
    FILE* input_f;
    input_f = fopen(fname, "r");
    
    //checking if the file is opened suxxessfully
    if (!input_f) {
        printf("Unable to open file Hansard-Example.xml\n");
        exit(1);   
    }

    //finding the size of the data
    if (stat(fname, &st) == 0)
        fsize = (uint64_t)st.st_size;

    datasize = fsize;

    //function for indexing the debates names;
    CreateIndex(input_f);

    fclose(input_f);
}

void Debates::CreateIndex(FILE* f){
    printf("The size of the data is %ld \n",datasize);
    // Allocating memory to store data in file
    data_ = (char *)malloc (datasize);

    short dcount = 0;  //for finding if the tag is for debate name
    short hcount = 0; // for finding the tag for heading
    //these variables are for taking the debates names
    uint64_t marks = 0;
    uint64_t hstart = 0;
    uint64_t hend = 0;
    bool hkey = false;
    //start reading the file
    for (uint64_t i = 0; i < datasize; i++){
        data_[i] = fgetc(f);
        //while recieving the data from file take the debates indexes 
        if(data_[i] == DEBATETAG[dcount])  
            dcount++;
        else
            dcount = 0;
        
        if(dcount > 17 && data_[i] == '=') marks = i;

        if(marks > 0){
            if(data_[i] == '>'){
            Index dbt;
            dbt.start_ = i;
            dbt.end_ = datasize;
            dbt.nlen_ = i - marks - 3;
            dbt.dname_ = findname(marks + 2,i - 1);
            // if the debates container is not empty then the end of the last debate is the start of the new one
            if(!debate_.empty()){
                debate_.at(debate_.size() - 1).end_ = i;
            }
            marks = 0;
            debate_.push_back(dbt);
            hkey =  true;
            }
            
        }

        if(hkey){
            
            if(data_[i] == HEADINGTAG[hcount])
                hcount++;
            else
                hcount = 0;
                
            if(hcount >= 9)
                hstart = i + 1;

            if(hstart > 0)
                if(data_[i] == '<') hend = i;

            if(hend > 0){
                debate_.at(debate_.size() - 1).dheading_ = findname(hstart, hend);
                hend = 0;
                hstart = 0;
                hkey = false;
            }
        }

    }

}

void Debates::FindHeadingSpeaker(){
    // #pragma omp parallel
    // #pragma omp for
    for(uint64_t i = 0; i < debate_.size(); i++) {

        // uint64_t hstart = 0;    // mark the start of the heading
        // uint64_t sstart = 0;    //
        for(uint64_t j = debate_.at(j).start_; j < debate_.size(); j++){
            
        }
    }
}

#endif