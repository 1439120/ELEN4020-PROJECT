#include <stdio.h> 
#include <ctype.h>
#include "hash_table.h"

int main(int argc, char* argv[]) {

    const char *small_data = "./data/Hansard-Example.xml";
    const char *big_data = "./data/SenateHansard1979vol2.xml";

    HashTable query(big_data);

    char *v = "bill";

    printf("main inpt %15s with length %d\n",v);
    
    query.findDebateName(v);
return 0;
}