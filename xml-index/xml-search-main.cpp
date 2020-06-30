#include <stdio.h> 
#include <ctype.h>
#include "hash_table.h"
#include "mpi.h"

void function(int my_rank, int p, char* mesg, int source, int dest, int val, int sume, int tag, MPI_Status status);

int main(int argc, char* argv[]) {

    const char *small_data = "./data/Hansard-Example.xml";
    const char *big_data = "./data/SenateHansard1979vol2.xml";

    HashTable query(big_data);

    const char *v = "bill";
 
    printf("main inpt %15s with length\n",v);
    
    //query.findDebateName(v);
    const char *n = "J. L. HORAK";
    query.DebatesSpeakerFound(n);

    int my_rank, p; // process rank and number of processes 
     int source, dest; // rank of sender and receiving process 
     int tag = 0; // tag for messages 
     char mesg[100]; // storage for message 
     MPI_Status status; // stores status for MPI_Recv statements
    int sume = 0;
    int val;

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    //function( my_rank,  p,  mesg, source, dest, val, sume, tag, status);
    
    MPI_Finalize(); // shuts down MPI
    
return 0;
}

void function(int my_rank, int p, char* mesg, int source, int dest, int val, int sume, int tag, MPI_Status status){
    if (my_rank!=0) {
        sprintf(mesg, "Greetings from %d!", my_rank); // stores into character array 
        dest = 0; // sets destination for MPI_Send to process 0
        val = my_rank;
        MPI_Send(&val, 1, MPI_INT, dest, tag, MPI_COMM_WORLD); 
    } // sends string to process 0
    
    else  {
        for(source = 1; source < p; source++){
            MPI_Recv(&val, 1, MPI_INT, source,tag, MPI_COMM_WORLD, &status); // recv from each process 
            sume+=val;
            printf("sum is %d\n", sume); // prints out greeting to screen 
        }   
    }

    if(my_rank == 0)printf("otside sum is is %d\n", sume);
}

const std::vector < Index>& SpeakerCorrespondingDebates(int my_rank, int p, char* mesg, int source, int dest, int val, int sume, int tag, MPI_Status status, HashTable& query, char* sname){
     // add those debates where name is found
    std::vector < Index> s = query.DebatesSpeakerFound(sname);
    /***********
     * I will add code for multiple speakers
     * 
     * 
     * *******/
 
    if (my_rank!=0) {

        for(int i = my_rank; i < s.size();i+=p){

        }
        dest = 0; // sets destination for MPI_Send to process 0
        val = my_rank;
   
        int past = val;
        val = my_rank != p ? val + my_rank * 5/(p-1) : val + 5 % p;
        MPI_Send(&val, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }
    
    else  {
        for(source = 1; source < p; source++){
            if(source > s.size()) break;
            MPI_Recv(&val, 1, MPI_INT, source,tag, MPI_COMM_WORLD, &status); // recv from each process 
            sume+=val;

            printf("sum is %d\n", sume); // prints out greeting to screen 
        }   
    }

    if(my_rank == 0)printf("otside sum is is %d\n", sume);
}