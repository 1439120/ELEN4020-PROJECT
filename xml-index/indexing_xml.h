#ifndef INDEXING_XML_H
#define INDEXING_XML_H

#include "definitions.h"

// Holds the location of the debate and the debate name
struct Index{
    uint64_t start_;
    uint64_t end_;
    char* dname_;
};

class Debates{
    public:
        Debates(const char* fname);
        ~Debates() {
            free(data_);
        }
        void DisplayDebates(){
            for(uint64_t i = 0; i < debate_.size(); i++) printf("start %ld - end %ld\n", debate_.at(i).start_, debate_.at(i).end_);
            printf("\n/***The number of debates are: %ld*****/\n", debate_.size());
        }

    protected:
        std::vector <Index> debate_;
        char* data_;
        uint64_t datasize;
        const char* DEBATETAG = "<debateSection name=\"opening\">";

    private:
        void CreateIndex(FILE* f);
};

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
    for (uint64_t i = 0; i < datasize; i++){
        data_[i] = fgetc(f);

        //while recieving the data from file take the debates indexes 
        if(data_[i] == DEBATETAG[dcount])  
            dcount++;
        else
            dcount = 0;
        
        if(dcount > 17 && data_[i] == '='){
            Index dbt;
            dbt.start_ = i;
            dbt.end_ = datasize;

            // if the debates container is not empty then the end of the last debate is the start of the new one
            if(!debate_.empty()){
                debate_.at(debate_.size() - 1).end_ = i;
            }
            debate_.push_back(dbt);
        }

    }

}

#endif