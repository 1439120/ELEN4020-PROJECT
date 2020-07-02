#include <stdio.h> 
#include <ctype.h>
#include "hash_table.h"
#include "mpi.h"

void SpeakerCorrespondingDebates(int my_rank, int p, HashTable& query, std::vector < char*> sname);
void DebatesCorrespondingSpeakers(int my_rank, int p, HashTable& query, std::vector < char*> sname);

int main(int argc, char* argv[]) {

    const char *small_data = "./data/Hansard-Example.xml";
    const char *big_data = "./data/SenateHansard1979vol2.xml";

    HashTable query(big_data);

    //const char *v = "LIQUOR AMENDMENT BILL";
 
    //printf("main inpt %15s with length\n",v);
    
    //query.SpeakersInDebate(v);
    //const char *n = "J. L. HORAK";
    //std::vector < char*> sname = {"L. E. D. WINCHESTER","A. J. KOCH","W. T. WEBBER"};
    //query.DebatesSpeakerFound(n);

    std::vector < char*> dname = {"LIQUOR AMENDMENT BILL","ESTIMATE OF EXPENDITURE, 1979-&#x2019;80","FINANCIAL INSTITUTIONS AMENDMENT BILL"};

    int my_rank, p; // process rank and number of processes 

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    //SpeakerCorrespondingDebates(my_rank, p, query,sname);
    DebatesCorrespondingSpeakers(my_rank, p, query,dname);
    
    MPI_Finalize(); // shuts down MPI
    
return 0;
}

void SpeakerCorrespondingDebates(int my_rank, int p, HashTable& query, std::vector < char*> sname){
    // we wiil mark every indices that applies to all the speakers
    if(sname.empty())return;
    std::vector < char* > com = query.DebatesSpeakerFound(sname.at(0));
    //remove(com);
    std::vector < int> results(com.size(), 1);
    std::vector < int> res(com.size(), 1);
    int source, dest, tag = 0;
    MPI_Status status;
    
    for(uint64_t i = my_rank; i < sname.size();i+=p){

        std::vector < char* > corrDeb = query.DebatesSpeakerFound(sname.at(i));
        for(int j = 0; j < com.size(); j++){
            bool found = false;
            for(int k = 0; k < corrDeb.size(); k++){
                
                if(strcmp(com.at(j), corrDeb.at(k)) == 0){
                    found = true;
                    break;
                }
            }

            if(!found) results.at(j) = 0;
            
        }
    }

    if (my_rank!=0) {
        dest = 0;
        MPI_Send(&results[0], results.size(), MPI_INT, dest, tag, MPI_COMM_WORLD);
    }
    
    else  {
        for(source = 1; source < p; source++){
            if(source > int (sname.size())) break;
            MPI_Recv(&res[0], res.size(), MPI_INT, source,tag, MPI_COMM_WORLD, &status); // recv from each process 

            //printf("-%d-", source);
            for(int r = 0; r < results.size(); r++){
                if(res.at(r) == 0) results[r] = 0;
              //  printf("%d", res[r]);
            }
            //printf("\n");

            //printf("size of recieved %d\n", results.size()); // prints out greeting to screen 
        }   
    }

    if(my_rank == 0){
        // printf("final solution-%d-", source);
        // for(int r = 0; r < results.size(); r++){
        //     //if(res.at(r) == 0) results[r] = 0;
        //     printf("%d", results[r]);
        // }
        // printf("\n");
        // printf("otside sum is %d\n", sume);
        printf("***************************************************\n");
        printf("These speakers: \n");
        for(int i = 0; i < sname.size(); i++) printf("-%15s\n", sname[i]);
        printf("\nShare these debates: \n");
        for(int i = 0; i < results.size(); i++){
            if(results[i] == 1) printf("-%15s\n",com[i]);
        }
        printf("\n");
        printf("***************************************************\n");
    }
}

void DebatesCorrespondingSpeakers(int my_rank, int p, HashTable& query, std::vector < char*> sname){
    if(sname.empty())return;
    std::vector < char* > com = query.SpeakersInDebate(sname.at(0));
    std::vector < int> results(com.size(), 1);
    std::vector < int> res(com.size(), 1);
    int source, dest, tag = 0;
    MPI_Status status;
    
    for(uint64_t i = my_rank; i < sname.size();i+=p){

        std::vector < char* > corrDeb = query.SpeakersInDebate(sname.at(i));
        for(int j = 0; j < com.size(); j++){
            bool found = false;
            for(int k = 0; k < corrDeb.size(); k++){
                
                if(strcmp(com.at(j), corrDeb.at(k)) == 0){
                    found = true;
                    break;
                }
            }

            if(!found) results.at(j) = 0;
            
        }
    }

    if (my_rank!=0) {
        dest = 0;
        MPI_Send(&results[0], results.size(), MPI_INT, dest, tag, MPI_COMM_WORLD);
    }
    
    else  {
        for(source = 1; source < p; source++){
            if(source > int (sname.size())) break;
            MPI_Recv(&res[0], res.size(), MPI_INT, source,tag, MPI_COMM_WORLD, &status); // recv from each process 

            for(int r = 0; r < results.size(); r++){
                if(res.at(r) == 0) results[r] = 0;
            }
        }   
    }

    if(my_rank == 0){
        printf("***************************************************\n");
        printf("These debates: \n");
        for(int i = 0; i < sname.size(); i++) printf("-%15s\n", sname[i]);
        printf("\nHave these common speakers: \n");
        for(int i = 0; i < results.size(); i++){
            if(results[i] == 1) printf("-%15s\n",com[i]);
        }
        printf("\n");
        printf("***************************************************\n");
    }
}